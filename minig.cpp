#include "bsm_delay.h"
#include "minig.h"
#include "pinmap.h"

using drivers::ad9959::AD9959;
using drivers::max11300::MAX11300;

#define WRITE_IO(PORT, ON_BITS, OFF_BITS)                                      \
  PORT->ODR = ((PORT->ODR & ~(OFF_BITS)) | (ON_BITS))
#define BITS_NONE (0)

#define ARRAYSIZE(a)                                                           \
  ((sizeof(a) / sizeof(*(a))) /                                                \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
namespace {

constexpr AD9959::Pins dds_pins = {
    PA_4, /* CS */
    D7,   /* reset */
    D8,   /* update */
    PE_9, /* p0 */
    PE_11 /* p1 */
};

constexpr int16_t to_dac(double volts) {
  return static_cast<int16_t>(volts / 10.0 * 0x0fff);
}

constexpr size_t num_samples = 1200;
uint16_t samples[num_samples];

#include "declare_fringes.h"

} // namepsace

// clang-format off
MiniG::MiniG() :
      coils_{GPIO_PIN_0}, //GPIOE
      liquid_crystal_1_{GPIO_PIN_2}, //GPIOE
      under_vac_shutter_{GPIO_PIN_3}, //GPIOG
      ao_3_{GPIO_PIN_3}, //GPIOE
      ao_2_{GPIO_PIN_4}, //GPIOE
      cooling_shutter_{GPIO_PIN_5}, //GPIOE
      mot_eo_{GPIO_PIN_6}, //GPIOE
      raman_eo_{GPIO_PIN_7}, //GPIOE
      m_lock_{GPIO_PIN_8}, //GPIOE
      analog_trigger_{GPIO_PIN_12}, //GPIOE
      mw_dds_profile_pin_{GPIO_PIN_11}, //GPIOE
      dds_switch_{GPIO_PIN_10}, //GPIOE
      inter_dds_profile_pin_{GPIO_PIN_9}, //GPIOE
      scope_{GPIO_PIN_2}, //GPIOG
      camera_ttl_{GPIO_PIN_13}, //GPIOE
      laser_jump_{GPIO_PIN_14}, //GPIOE
      m_horn_switch_{GPIO_PIN_15}, //GPIOE
      
      // Analog Outputs
      ao1_freq_{MAX11300::PORT10},
      ao2_atten_{MAX11300::PORT11},
      ao3_atten_{MAX11300::PORT12},
      z_field_{MAX11300::PORT13},
      ns_field_{MAX11300::PORT14},
      we_field_{MAX11300::PORT15},
      eo_freq_{MAX11300::PORT16},
      bias_field_{MAX11300::PORT17},

      // Analog Input
      photodiode_{MAX11300::PORT0},
      
      // DDS
      dds_spi_{PB_5_ALT0, PB_4_ALT0, PB_3_ALT0},
      dds_{dds_spi_, dds_pins, 10000000 /* ref_freq */, 20 /* mult */},

      // PIXI
      pixi_spi_{SPI_MOSI, SPI_MISO, SPI_SCK},
      pixi_{pixi_spi_, SPI_CS} {}
// clang-format on

void MiniG::set_dds_params(double chirp_rate_kHz_p_ms,
                           double raman_detuning_kHz) {

  constexpr double free_fall_time_s = .260;
  constexpr double num_steps = 500000;
  // constexpr double num_steps = 130;
  constexpr double num_steps_mw = 50000000;
  constexpr double clock_transition = 12631770;
  double chirp_start_hz = raman_detuning_kHz * 1000 + clock_transition;
  double chirp_stop_hz =
      chirp_start_hz + free_fall_time_s * 1000 * chirp_rate_kHz_p_ms * 1000;
  dds_.reset();
  dds_.set_freq_linear_sweep_params(AD9959::Channel0, chirp_start_hz,
                                    chirp_stop_hz, free_fall_time_s * num_steps,
                                    1 / num_steps);
  dds_.set_freq_linear_sweep_params(AD9959::Channel1, clock_transition,
                                    chirp_start_hz, .01 * num_steps_mw,
                                    1 / num_steps_mw);
  dds_.io_update();
}

void MiniG::set_dds_params(dds_params_t params) {
  dds_.reset();
  dds_.set_freq_linear_sweep_params(
      AD9959::Channel0, params.mult, params.chirp_start_hex,
      params.chirp_stop_hex, params.chirp_freq_hex, params.chirp_time_hex);

  dds_.set_freq_linear_sweep_params(
      AD9959::Channel1, params.mult, params.clk_transition_hex,
      params.chirp_start_hex, params.mw_freq_hex, params.mw_time_hex);
  dds_.io_update();
}

void MiniG::init() {
  __HAL_RCC_GPIOE_CLK_ENABLE();

  __HAL_RCC_GPIOG_CLK_ENABLE();

  for (int pin = 0; pin < 16; pin++) {
    pin_function(port_pin(PortE, pin),
                 STM_PIN_DATA(STM_MODE_OUTPUT_PP, GPIO_NOPULL, 0));
  }
  pin_function(port_pin(PortG, 2),
               STM_PIN_DATA(STM_MODE_OUTPUT_PP, GPIO_NOPULL, 0));
  pin_function(port_pin(PortG, 3),
               STM_PIN_DATA(STM_MODE_OUTPUT_PP, GPIO_NOPULL, 0));

  constexpr float NS_MOT = 0.04;
  constexpr float WE_MOT = 0.25;
  constexpr float BIAS_MOT = 0.77;
  // Ramps for Analog IO
  // clang-format off
  MAX11300::Ramp mot_on_ramps[] = {
      {ns_field_, to_dac(0), to_dac(NS_MOT)},
      {we_field_, to_dac(0), to_dac(WE_MOT)},
      {bias_field_, to_dac(0), to_dac(BIAS_MOT)},
  };
  mot_on_ramp_.configured = 0;
  mot_on_ramp_.num_ramps = ARRAYSIZE(mot_on_ramps);
  mot_on_ramp_.num_steps = 30;
  mot_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&mot_on_ramp_, mot_on_ramps);

  MAX11300::Ramp pgc_on_ramps[] = {
      {ao1_freq_, to_dac(6.75), to_dac(6.6)},
      {ao2_atten_, to_dac(1.75), to_dac(2.4)},
      {ao3_atten_, to_dac(5), to_dac(1.88)},
      {eo_freq_, to_dac(8.97), to_dac(9.85)},
  };
  pgc_on_ramp_.configured = 0;
  pgc_on_ramp_.num_ramps = ARRAYSIZE(pgc_on_ramps);
  pgc_on_ramp_.num_steps = 5;
  pgc_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&pgc_on_ramp_, pgc_on_ramps);

  constexpr float WE_MW = 2.25;
  MAX11300::Ramp mw_on_ramps[] = {
      {ao1_freq_, to_dac(6.6), to_dac(7.5)},
      {ao2_atten_, to_dac(2.4), to_dac(0)},
      {ao3_atten_, to_dac(1.88), to_dac(10)},
      {we_field_, to_dac(WE_MOT), to_dac(WE_MW)},
      {eo_freq_, to_dac(9.85), to_dac(9.21)},
  };
  mw_on_ramp_.configured = 0;
  mw_on_ramp_.num_ramps = ARRAYSIZE(mw_on_ramps);
  mw_on_ramp_.num_steps = 30;
  mw_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&mw_on_ramp_, mw_on_ramps);

  constexpr float WE_RAMAN = 0.25;
  constexpr float BIAS_INTER = 10;
  MAX11300::Ramp raman_on_ramps[] = {
    //2ms
      {ao1_freq_, to_dac(7.5), to_dac(8.0)},
      {ao2_atten_, to_dac(0), to_dac(2.9)},
      // Should be 5ms
      {ns_field_, to_dac(NS_MOT), to_dac(0)},
      {we_field_, to_dac(WE_MW), to_dac(WE_RAMAN)},
      // 2 ms
      {eo_freq_, to_dac(9.21), to_dac(6.5)},
      // should be 5 ms again
      {bias_field_, to_dac(BIAS_MOT), to_dac(BIAS_INTER)},
  };
  raman_on_ramp_.configured = 0;
  raman_on_ramp_.num_ramps = ARRAYSIZE(raman_on_ramps);
  raman_on_ramp_.num_steps = 20;
  raman_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&raman_on_ramp_, raman_on_ramps);

  MAX11300::Ramp image_on_ramps[] = {
      {ao1_freq_, to_dac(8), to_dac(7.1)},
      {ao2_atten_, to_dac(2.9), to_dac(1.75)},
      {eo_freq_, to_dac(6.5), to_dac(8.93)},
  };
  image_on_ramp_.configured = 0;
  image_on_ramp_.num_ramps = ARRAYSIZE(image_on_ramps);
  image_on_ramp_.num_steps = 30;
  image_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&image_on_ramp_, image_on_ramps);
  // clang-format on

  // Note: Do not reset after initializing, or you'll waste time debugging for
  // no reason.
  dds_.init();
  pixi_.init();
  set_dds_params(22.9, 790);
  reset();
}

void MiniG::reset() {
  WRITE_IO(GPIOE, liquid_crystal_1_ | ao_3_ | raman_eo_ | m_lock_ |
                      dds_switch_ | m_horn_switch_,
           coils_ | ao_2_ | cooling_shutter_ | mot_eo_ | raman_eo_ |
               analog_trigger_ | camera_ttl_ | laser_jump_ |
               mw_dds_profile_pin_ | inter_dds_profile_pin_);
  WRITE_IO(GPIOG, BITS_NONE, under_vac_shutter_ | scope_);

  constexpr float MOT_FREQ = 6.75;
  constexpr float AO2_MOT = 1.75;
  constexpr float AO3_MOT = 5;
  constexpr float EO_MOT = 8.97;
  pixi_.single_ended_dac_write(ao1_freq_, to_dac(MOT_FREQ));
  pixi_.single_ended_dac_write(ao2_atten_, to_dac(AO2_MOT));
  pixi_.single_ended_dac_write(ao3_atten_, to_dac(AO3_MOT));
  pixi_.single_ended_dac_write(z_field_, to_dac(0));
  pixi_.single_ended_dac_write(ns_field_, to_dac(0));
  pixi_.single_ended_dac_write(we_field_, to_dac(0));
  pixi_.single_ended_dac_write(eo_freq_, to_dac(EO_MOT));
  pixi_.single_ended_dac_write(bias_field_, to_dac(0));

  bsm_delay_ms(2);
}

#define SPECTROSCOPY 0
#define RAMAN_RABI 0
#define MW_RABI 0
#define INTER 1
#define DEBUG_PD 0
void MiniG::run() {
  printf("start sweep\n");
#if MW_RABI
  for (int pulse = 0; pulse <= 800; pulse += 10) {
#endif
#if RAMAN_RABI
    for (uint32_t raman = 0; raman <= 30; raman += 2) {
#endif
// for (double delta = -120; delta <= -20; delta += 20) {
// for(float fall_ms = 192; fall_ms <= 300; fall_ms += 5) {
// for (float bias = 0; bias <= 5.0; bias += .2) {
#if SPECTROSCOPY
      for (double raman_detuning = 400; raman_detuning <= 1000;
           raman_detuning += 10) {
        set_dds_params(22.9, raman_detuning);
#endif
#if INTER
        // for (double chirp = 22.5; chirp <= 23.511; chirp += 0.02) {
        // set_dds_params(chirp, 790);
        for (size_t j = 0; j < NUM_POINTS; j++) {
          set_dds_params(fringes[j]);
#endif
#if RAMAN_RABI
          set_dds_params(23.0, 790);
#endif
          reset();
          mot();
          pgc();

#if !MW_RABI
          int pulse = 230;
#endif
          mw(pulse);
          uint32_t T = 100;
          float fall_ms = 45.2;
          uint32_t fall_us = static_cast<uint32_t>(fall_ms * 1000);
#if !RAMAN_RABI
          uint32_t raman = 8;
#endif
          interferometry(T, fall_us, raman);

          image();
          wait_ms(500);

// double delta = -790;
// printf("fall_ms: %f\n", fall_ms);
// printf("delta: %f\n", delta);
// printf("bias: %f\n", bias);
#if SPECTROSCOPY
          printf("rd: %f\n", raman_detuning);
#endif
#if INTER
          // printf("rd: %f\n", chirp);
          printf("rd: %f\n", fringes[j].actual_chirp);
#endif
#if MW_RABI
          printf("rd: %d\n", pulse);
#endif
#if RAMAN_RABI
          printf("rd: %lu\n", raman);
#endif
          uint32_t f4 = 0, f34 = 0, bg = 6;
          size_t i = 0;
#if DEBUG_PD
          printf("ListPlot[{");
#endif
          for (; i < 127; i++) {
            f4 += samples[i];
#if DEBUG_PD
            printf("%d,", samples[i]);
#endif
          }
          for (; i < 127 + 127; i++) {
            f34 += samples[i];
#if DEBUG_PD
            printf("%d,", samples[i]);
#endif
          }
          for (; i < 127 + 127 + 127; i++) {
            bg += samples[i];
#if DEBUG_PD
            printf("%d,", samples[i]);
#endif
          }
#if DEBUG_PD
          printf("}]\n\n");
#endif
          double detection =
              (static_cast<double>(f4) - static_cast<double>(bg)) /
              (static_cast<double>(f34) - static_cast<double>(bg));
// printf("f4: %lu\n", f4);
#if MW_RABI
          printf("fr: %lu\n", f34 - bg);
#endif
// printf("bg: %lu\n", bg);
#if RAMAN_RABI | SPECTROSCOPY | INTER
          printf("fr: %f\n\n", detection);
#endif
#if RAMAN_RABI | SPECTROSCOPY | INTER | MW_RABI
        }
#endif
        printf("end sweep\n");
      }

      void MiniG::mot() {
        WRITE_IO(GPIOE, ao_2_ | cooling_shutter_, m_lock_);

        pixi_.run_ramps(&mot_on_ramp_);

        // Actual MOT Stage
        WRITE_IO(GPIOE, coils_, BITS_NONE);
        bsm_delay_ms(500);

        // Turn the MOT off
        WRITE_IO(GPIOE, BITS_NONE, coils_);

        bsm_delay_ms(6);
      }

      void MiniG::pgc() {
        WRITE_IO(GPIOE, laser_jump_, BITS_NONE);
        pixi_.run_ramps(&pgc_on_ramp_);
        bsm_delay_us(500);

        // Just hold it to cool
        bsm_delay_ms(10);

        // Turn off PGC
        WRITE_IO(GPIOE, BITS_NONE, cooling_shutter_);

        bsm_delay_ms(7);
      }

      void MiniG::mw(int pulse_duration) {
        WRITE_IO(GPIOE, cooling_shutter_,
                 laser_jump_ | liquid_crystal_1_ | ao_2_ | ao_3_);

        pixi_.run_ramps(&mw_on_ramp_);
        bsm_delay_ms(2);

        // Actually turn on Microwave
        WRITE_IO(GPIOE, BITS_NONE, m_horn_switch_);
        bsm_delay_us(pulse_duration);

        // Stop Microwave
        WRITE_IO(GPIOE, m_horn_switch_, BITS_NONE);
        bsm_delay_ms(1);

        // Blow away
        WRITE_IO(GPIOE, ao_3_ | mot_eo_ | mw_dds_profile_pin_, BITS_NONE);
#if MW_RABI
        bsm_delay_us(10000);
#else
    bsm_delay_us(10000 - pulse_duration);
#endif
      }

      // Start Interformetry
      void MiniG::interferometry(uint32_t T, uint32_t fall, uint32_t raman) {
        WRITE_IO(GPIOE, BITS_NONE,
                 ao_3_ | cooling_shutter_ | raman_eo_ | dds_switch_);
        pixi_.run_ramps(&raman_on_ramp_);
        // last 2 ms
        bsm_delay_ms(11);

        // Freefall
        WRITE_IO(GPIOE, ao_3_ | inter_dds_profile_pin_, BITS_NONE);
        bsm_delay_ms(5);

        WRITE_IO(GPIOG, scope_, BITS_NONE);
#if INTER | SPECTROSCOPY
        WRITE_IO(GPIOE, ao_2_, BITS_NONE);
        bsm_delay_us(4);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_);

        bsm_delay_ms(T);

#if INTER
        WRITE_IO(GPIOE, ao_2_, BITS_NONE);
        bsm_delay_us(8);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_);

        bsm_delay_ms(T);

#if INTER
        WRITE_IO(GPIOE, ao_2_, BITS_NONE);
        bsm_delay_us(4);
#elif RAMAN_RABI
    WRITE_IO(GPIOE, ao_2_, BITS_NONE);
    bsm_delay_us(raman);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_);

        WRITE_IO(GPIOG, BITS_NONE, scope_);
        bsm_delay_us(fall);

        // Stop sweeping
      }

      void MiniG::image() {
        WRITE_IO(GPIOE, cooling_shutter_ | dds_switch_ | raman_eo_, ao_3_);
        pixi_.run_ramps(&image_on_ramp_);
        // takes 2 ms
        bsm_delay_ms(3);

        // Stabilize
        WRITE_IO(GPIOE, BITS_NONE, mot_eo_);
        bsm_delay_us(2700);

        WRITE_IO(GPIOE, m_lock_, BITS_NONE);
        bsm_delay_us(50);

        // Turn laser on
        WRITE_IO(GPIOE, ao_3_ | camera_ttl_, BITS_NONE);
        // TODO(bsm): write code for this in a minute
        pixi_.max_speed_adc_read(photodiode_, samples, 127);
        // bsm_delay_us(400);

        // Repumping Stage
        WRITE_IO(GPIOE, ao_2_ /*BITS_NONE*/, camera_ttl_);
        bsm_delay_us(150);

        // Second Sample
        WRITE_IO(GPIOE, BITS_NONE, ao_2_);
        pixi_.max_speed_adc_read(photodiode_, &samples[127], 127);
        // bsm_delay_us(400);

        // Wait before background
        bsm_delay_ms(5);

        // Third detection for background
        pixi_.max_speed_adc_read(photodiode_, &samples[127 + 127], 127);
        // bsm_delay_us(400);

        // Wait long time before Camera Background
        bsm_delay_ms(40);

        // Take background image
        WRITE_IO(GPIOE, camera_ttl_, BITS_NONE);

        bsm_delay_ms(10);
      }
