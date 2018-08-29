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

constexpr MiniG::dds_params_t dds_params_0 = {.mult = 16,
                                              .clk_transition_hex = 0x1435f916,
                                              .chirp_start_hex = 0x155ce28e,
                                              .chirp_stop_hex = 0x1eeda2a3,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x30,
                                              .chirp_time_hex = 0x7b,
                                              .chirp_freq_hex = 0x76a,
                                              .actual_chirp = 22.993824};

constexpr MiniG::dds_params_t dds_params_1 = {.mult = 16,
                                              .clk_transition_hex = 0x1435f916,
                                              .chirp_start_hex = 0x155ce28e,
                                              .chirp_stop_hex = 0x1eeda2a3,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x30,
                                              .chirp_time_hex = 0xf6,
                                              .chirp_freq_hex = 0xed4,
                                              .actual_chirp = 22.993824};

constexpr MiniG::dds_params_t dds_params_2 = {.mult = 20,
                                              .clk_transition_hex = 0x102b2dab,
                                              .chirp_start_hex = 0x11171ba5,
                                              .chirp_stop_hex = 0x18be1bea,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x1e,
                                              .chirp_time_hex = 0xa1,
                                              .chirp_freq_hex = 0x636,
                                              .actual_chirp = 22.993834};

constexpr MiniG::dds_params_t dds_params_3 = {.mult = 17,
                                              .clk_transition_hex = 0x13059f23,
                                              .chirp_start_hex = 0x141b2f95,
                                              .chirp_stop_hex = 0x1d1be5a7,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x2a,
                                              .chirp_time_hex = 0x94,
                                              .chirp_freq_hex = 0x7e7,
                                              .actual_chirp = 22.993875};

constexpr MiniG::dds_params_t dds_params_4 = {.mult = 19,
                                              .clk_transition_hex = 0x110507a6,
                                              .chirp_start_hex = 0x11fd6078,
                                              .chirp_stop_hex = 0x1a0b7ca8,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x22,
                                              .chirp_time_hex = 0xc0,
                                              .chirp_freq_hex = 0x835,
                                              .actual_chirp = 22.993875};

constexpr MiniG::dds_params_t dds_params_5 = {.mult = 16,
                                              .clk_transition_hex = 0x1435f916,
                                              .chirp_start_hex = 0x155ce28e,
                                              .chirp_stop_hex = 0x1eeda476,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x30,
                                              .chirp_time_hex = 0xb5,
                                              .chirp_freq_hex = 0xae9,
                                              .actual_chirp = 22.993891};

constexpr MiniG::dds_params_t dds_params_6 = {.mult = 19,
                                              .clk_transition_hex = 0x110507a6,
                                              .chirp_start_hex = 0x11fd6078,
                                              .chirp_stop_hex = 0x1a0b7dc3,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x22,
                                              .chirp_time_hex = 0xe3,
                                              .chirp_freq_hex = 0x9b4,
                                              .actual_chirp = 22.993924};

constexpr MiniG::dds_params_t dds_params_7 = {.mult = 16,
                                              .clk_transition_hex = 0x1435f916,
                                              .chirp_start_hex = 0x155ce28e,
                                              .chirp_stop_hex = 0x1eeda567,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x30,
                                              .chirp_time_hex = 0xef,
                                              .chirp_freq_hex = 0xe68,
                                              .actual_chirp = 22.993926};

constexpr MiniG::dds_params_t dds_params_8 = {.mult = 20,
                                              .clk_transition_hex = 0x102b2dab,
                                              .chirp_start_hex = 0x11171ba5,
                                              .chirp_stop_hex = 0x18be1dfa,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x1e,
                                              .chirp_time_hex = 0x99,
                                              .chirp_freq_hex = 0x5e7,
                                              .actual_chirp = 22.993928};

constexpr MiniG::dds_params_t dds_params_9 = {.mult = 17,
                                              .clk_transition_hex = 0x13059f23,
                                              .chirp_start_hex = 0x141b2f95,
                                              .chirp_stop_hex = 0x1d1be7aa,
                                              .mw_time_hex = 0x1,
                                              .mw_freq_hex = 0x2a,
                                              .chirp_time_hex = 0x91,
                                              .chirp_freq_hex = 0x7be,
                                              .actual_chirp = 22.993953};

constexpr MiniG::dds_params_t fringe_params[10] = {
    dds_params_0, dds_params_1, dds_params_2, dds_params_3, dds_params_4,
    dds_params_5, dds_params_6, dds_params_7, dds_params_8, dds_params_9,
};

constexpr float NS_MOT = 0.07;
constexpr float WE_MOT = 0.28;
constexpr float BIAS_MOT = 0.77;
constexpr float AO1_MOT = 6.75;
constexpr float AO2_MOT = 1.75;
constexpr float AO3_MOT = 5;
constexpr float EO_MOT = 8.97;

constexpr float AO1_PGC = 6.6;
constexpr float AO2_PGC = 2.4;
constexpr float AO3_PGC = 1.88;
constexpr float EO_PGC = 9.85;

constexpr float WE_MW = 2.28;
constexpr float NS_MW = 1.07;
constexpr float AO1_BLOW_AWAY = 7.5;
constexpr float AO2_BLOW_AWAY = 0;
constexpr float AO3_BLOW_AWAY = 10;
constexpr float EO_PUMP = 9.21;

constexpr float WE_RAMAN = 0.25;
constexpr float NS_RAMAN = 0.04;
constexpr float BIAS_INTER = 10;
constexpr float AO1_RAMAN = 8.0;
constexpr float AO2_RAMAN = 3.1;
constexpr float EO_RAMAN = 6.5;

constexpr float AO1_IMAGE = 7.1;
constexpr float AO2_IMAGE = 1.75;
constexpr float EO_IMAGE = 8.93;

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
      {ao1_freq_, to_dac(AO1_MOT), to_dac(AO1_PGC)},
      {ao2_atten_, to_dac(AO2_MOT), to_dac(AO2_PGC)},
      {ao3_atten_, to_dac(AO3_MOT), to_dac(AO3_PGC)},
      {eo_freq_, to_dac(EO_MOT), to_dac(EO_PGC)},
  };
  pgc_on_ramp_.configured = 0;
  pgc_on_ramp_.num_ramps = ARRAYSIZE(pgc_on_ramps);
  pgc_on_ramp_.num_steps = 5;
  pgc_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&pgc_on_ramp_, pgc_on_ramps);

  MAX11300::Ramp mw_on_ramps[] = {
      {ao1_freq_, to_dac(AO1_PGC), to_dac(AO1_BLOW_AWAY)},
      {ao2_atten_, to_dac(AO2_PGC), to_dac(AO2_BLOW_AWAY)},
      {ao3_atten_, to_dac(AO3_PGC), to_dac(AO3_BLOW_AWAY)},
      {ns_field_, to_dac(NS_MOT), to_dac(NS_MW)},
      {we_field_, to_dac(WE_MOT), to_dac(WE_MW)},
      {eo_freq_, to_dac(EO_PGC), to_dac(EO_PUMP)},
  };
  mw_on_ramp_.configured = 0;
  mw_on_ramp_.num_ramps = ARRAYSIZE(mw_on_ramps);
  mw_on_ramp_.num_steps = 30;
  mw_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&mw_on_ramp_, mw_on_ramps);

  MAX11300::Ramp raman_on_ramps[] = {
      //2ms
      {ao1_freq_, to_dac(AO1_BLOW_AWAY), to_dac(AO1_RAMAN)},
      {ao2_atten_, to_dac(AO2_BLOW_AWAY), to_dac(AO2_RAMAN)},
      // Should be 5ms
      {ns_field_, to_dac(NS_MOT), to_dac(NS_RAMAN)},
      {we_field_, to_dac(WE_MW), to_dac(WE_RAMAN)},
      // 2 ms
      {eo_freq_, to_dac(EO_PUMP), to_dac(EO_RAMAN)},
      // should be 5 ms again
      {bias_field_, to_dac(BIAS_MOT), to_dac(BIAS_INTER)},
  };
  raman_on_ramp_.configured = 0;
  raman_on_ramp_.num_ramps = ARRAYSIZE(raman_on_ramps);
  raman_on_ramp_.num_steps = 50;
  raman_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&raman_on_ramp_, raman_on_ramps);

  MAX11300::Ramp image_on_ramps[] = {
      {ao1_freq_, to_dac(AO1_RAMAN), to_dac(AO1_IMAGE)},
      {ao2_atten_, to_dac(AO2_RAMAN), to_dac(AO2_IMAGE)},
      {eo_freq_, to_dac(EO_RAMAN), to_dac(EO_IMAGE)},
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
  set_dds_params(dds_params_0);
  reset();
}

void MiniG::reset() {
  WRITE_IO(GPIOE, liquid_crystal_1_ | ao_3_ | raman_eo_ | m_lock_ |
                      dds_switch_ | m_horn_switch_,
           coils_ | ao_2_ | cooling_shutter_ | mot_eo_ | raman_eo_ |
               analog_trigger_ | camera_ttl_ | laser_jump_ |
               mw_dds_profile_pin_ | inter_dds_profile_pin_);
  WRITE_IO(GPIOG, BITS_NONE, under_vac_shutter_ | scope_);

  pixi_.single_ended_dac_write(ao1_freq_, to_dac(AO1_MOT));
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
  for (int pulse = 0; pulse <= 300; pulse += 10) {
#endif
#if RAMAN_RABI
    for (uint32_t raman = 0; raman <= 30; raman += 2) {
#endif
// for (double delta = -120; delta <= -20; delta += 20) {
// for (float bias = 0; bias <= 5.0; bias += .2) {
#if SPECTROSCOPY
      for (double raman_detuning = 00; raman_detuning <= 2000;
           raman_detuning += 10) {
// set_dds_params(23.0, raman_detuning);
#endif
// for(float fall_ms = 4; fall_ms <= 80; fall_ms += 2) {
#if INTER
        for (size_t j = 0; j < 10; j++) {
          j = 2;
          set_dds_params(fringe_params[j]);
#endif
#if RAMAN_RABI
// set_dds_params(23.0, 790);
#endif
          reset();
          mot();
          pgc();

#if !MW_RABI
          int pulse = 220;
#endif
          mw(pulse);
          uint32_t T = 100;
          float fall_ms = 46.4;
          uint32_t fall_us = static_cast<uint32_t>(fall_ms * 1000);
#if !RAMAN_RABI
          uint32_t raman = 8;
#endif
          interferometry(T, fall_us, raman);

          image();
          wait(1);

// double delta = -790;
// printf("fall: %f\n", fall_ms);
// printf("delta: %f\n", delta);
// printf("bias: %f\n", bias);
#if SPECTROSCOPY
          printf("rd: %f\n", raman_detuning);
#endif
#if INTER
          printf("rd: %f\n", fringe_params[j].actual_chirp);
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
        }
        printf("end sweep\n");
      }

      void MiniG::mot() {
        WRITE_IO(GPIOE, ao_2_ | cooling_shutter_, m_lock_);

        pixi_.run_ramps(&mot_on_ramp_);

        // Actual MOT Stage
        WRITE_IO(GPIOE, coils_, BITS_NONE);
        bsm_delay_ms(200);

        // Turn the MOT off
        WRITE_IO(GPIOE, BITS_NONE, coils_);

        bsm_delay_ms(6);
      }

      void MiniG::pgc() {
        WRITE_IO(GPIOE, laser_jump_, BITS_NONE);
        pixi_.run_ramps(&pgc_on_ramp_);

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

        // Actually turn on Microwave
        WRITE_IO(GPIOE, BITS_NONE, m_horn_switch_);
        bsm_delay_us(pulse_duration);

        // Stop Microwave
        WRITE_IO(GPIOE, m_horn_switch_, BITS_NONE);
        bsm_delay_ms(1);

        // Blow away
        WRITE_IO(GPIOE, /* ao_3_ | */ mot_eo_ | mw_dds_profile_pin_, BITS_NONE);
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
        // last 5 ms
        bsm_delay_ms(5);

        // Freefall
        WRITE_IO(GPIOE, ao_3_ | inter_dds_profile_pin_, BITS_NONE);
        bsm_delay_ms(5);

#if INTER | SPECTROSCOPY
        // WRITE_IO(GPIOE, ao_2_ | analog_trigger_, BITS_NONE);
        bsm_delay_us(4);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_ | analog_trigger_);

        bsm_delay_ms(T);

#if INTER
        // WRITE_IO(GPIOE, ao_2_ | analog_trigger_, BITS_NONE);
        bsm_delay_us(8);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_ | analog_trigger_);

        bsm_delay_ms(T);

#if INTER
        // WRITE_IO(GPIOE, ao_2_ | analog_trigger_, BITS_NONE);
        bsm_delay_us(4);
#elif RAMAN_RABI
  WRITE_IO(GPIOE, ao_2_, BITS_NONE);
  bsm_delay_us(raman);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_ | analog_trigger_);

        bsm_delay_us(fall);

        // Stop sweeping
      }

      void MiniG::image() {
        WRITE_IO(GPIOE, cooling_shutter_ | dds_switch_ | raman_eo_, ao_3_);
        pixi_.run_ramps(&image_on_ramp_);
        // takes 3 ms
        bsm_delay_ms(5);

        // Stabilize
        WRITE_IO(GPIOE, BITS_NONE, mot_eo_);
        bsm_delay_us(3600);

        WRITE_IO(GPIOE, m_lock_, BITS_NONE);
        bsm_delay_us(50);

        // Turn laser on
        WRITE_IO(GPIOE, ao_3_ | camera_ttl_, BITS_NONE);
        pixi_.max_speed_adc_read(photodiode_, samples, 127);

        // Repumping Stage
        WRITE_IO(GPIOE, ao_2_ /*BITS_NONE*/, camera_ttl_);
        bsm_delay_us(150);

        // Second Sample
        WRITE_IO(GPIOE, BITS_NONE, ao_2_);
        pixi_.max_speed_adc_read(photodiode_, &samples[127], 127);

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
