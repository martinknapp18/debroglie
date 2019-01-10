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

#define STOP                                                                   \
  while (1)                                                                    \
    ;

#define SPECTROSCOPY 0
#define RAMAN_RABI 0
#define MW_RABI 0
#define INTER 1
#define DEBUG_PD 0
#define USE_CAMERA 1
#define CENTER_PD 0
#define TUNE 0

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

// #include "dds_params/declare_T_80_fringes.h"
// #include "dds_params/declare_T_120_fringes.h"
// #include "dds_params/declare_T_120_16_fringes.h"
#include "dds_params/T_130_fringes.h"
// #include "dds_params/declare_T_115_fringes_17e_7.h"
#include "dds_params/declare_spectroscopy.h"

constexpr float AO1_MOT = 6.6;
constexpr float AO2_MOT = 1.76;
constexpr float AO3_MOT = 5;
constexpr float EO_MOT = 8.93;
constexpr float NS_MOT = 0.7;
constexpr float WE_MOT = 1.2;
constexpr float BIAS_MOT = 0.11;

constexpr float AO1_PGC = AO1_MOT;
constexpr float AO2_PGC = 2.20;
constexpr float AO3_PGC = 1.95;
constexpr float EO_PGC = 9.78;

constexpr float AO1_MW = 7.8;
constexpr float AO2_MW = 0;
constexpr float AO3_MW = 10;
constexpr float NS_MW = NS_MOT;
constexpr float WE_MW = 2.2;
constexpr float EO_MW = 9.21;
constexpr float BIAS_MW = BIAS_MOT;

constexpr float AO1_RAMAN = 8.1;
constexpr float AO2_RAMAN = 2.5;
constexpr float NS_RAMAN = 0;
constexpr float WE_RAMAN = 0;
constexpr float EO_RAMAN = 6.5;
constexpr float BIAS_RAMAN = 3;

constexpr float AO1_IMAGE = 7.30;
constexpr float AO2_IMAGE = 1.75;
constexpr float NS_IMAGE = 0.04;
constexpr float WE_IMAGE = 0.25;
constexpr float EO_IMAGE = 8.88;

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
      // {ao1_freq_, to_dac(0O1_MOT), to_dac(AO1_PGC)},
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
      {ao1_freq_, to_dac(AO1_PGC), to_dac(AO1_MW)},
      {ao2_atten_, to_dac(AO2_PGC), to_dac(AO2_MW)},
      {ao3_atten_, to_dac(AO3_PGC), to_dac(AO3_MW)},
      {we_field_, to_dac(WE_MOT), to_dac(WE_MW)},
      {eo_freq_, to_dac(EO_PGC), to_dac(EO_MW)},
  };
  mw_on_ramp_.configured = 0;
  mw_on_ramp_.num_ramps = ARRAYSIZE(mw_on_ramps);
  mw_on_ramp_.num_steps = 30;
  mw_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&mw_on_ramp_, mw_on_ramps);

  MAX11300::Ramp raman_on_ramps[] = {
    //2ms
      {ao1_freq_, to_dac(AO1_MW), to_dac(AO1_RAMAN)},
      {ao2_atten_, to_dac(AO2_MW), to_dac(AO2_RAMAN)},
      // Should be 5ms
      {ns_field_, to_dac(NS_MOT), to_dac(NS_RAMAN)},
      {we_field_, to_dac(WE_MW), to_dac(WE_RAMAN)},
      // 2 ms
      {eo_freq_, to_dac(EO_MW), to_dac(EO_RAMAN)},
      // should be 5 ms again
      {bias_field_, to_dac(BIAS_MOT), to_dac(BIAS_RAMAN)},
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
      {ns_field_, to_dac(NS_RAMAN), to_dac(NS_IMAGE)},
      {we_field_, to_dac(WE_RAMAN), to_dac(WE_IMAGE)},
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
  // set_dds_params(22.9, 720);
  set_dds_params(fringes[0]);
  reset(AO1_MOT);
}

void MiniG::reset(float var, bool k_up) {
  uint32_t ON_PINS = liquid_crystal_1_ | ao_3_ | raman_eo_ | m_lock_ |
                      dds_switch_ | m_horn_switch_;
  uint32_t OFF_PINS = coils_ | ao_2_ | cooling_shutter_ | mot_eo_ | raman_eo_ |
               analog_trigger_ | camera_ttl_ | laser_jump_;
  if (k_up) {
    OFF_PINS |= mw_dds_profile_pin_ | inter_dds_profile_pin_;
  } else {
    ON_PINS |= mw_dds_profile_pin_ | inter_dds_profile_pin_;
  }
  WRITE_IO(GPIOE, ON_PINS, OFF_PINS);
  WRITE_IO(GPIOG, BITS_NONE, under_vac_shutter_ | scope_);

  pixi_.single_ended_dac_write(ao1_freq_, to_dac(AO1_MOT));
  pixi_.single_ended_dac_write(ao2_atten_, to_dac(AO2_MOT));
  pixi_.single_ended_dac_write(ao3_atten_, to_dac(AO3_MOT));
  pixi_.single_ended_dac_write(z_field_, to_dac(0));
  pixi_.single_ended_dac_write(ns_field_, to_dac(0));
  pixi_.single_ended_dac_write(we_field_, to_dac(0));
  pixi_.single_ended_dac_write(eo_freq_, to_dac(EO_MOT));
  pixi_.single_ended_dac_write(bias_field_, to_dac(0));

#if TUNE
  // pixi_.single_ended_dac_write(ao1_freq_, to_dac(var));
  // MAX11300::Ramp image_on_ramps[] = {
  //     {ao1_freq_, to_dac(AO1_RAMAN), to_dac(var)},
  //     {ao2_atten_, to_dac(AO2_RAMAN), to_dac(AO2_IMAGE)},
  //     {eo_freq_, to_dac(EO_RAMAN), to_dac(EO_IMAGE)},
  //     {ns_field_, to_dac(NS_RAMAN), to_dac(NS_IMAGE)},
  //     {we_field_, to_dac(WE_RAMAN), to_dac(WE_IMAGE)},
  // };
  // image_on_ramp_.configured = 1;
  // image_on_ramp_.num_ramps = ARRAYSIZE(image_on_ramps);
  // image_on_ramp_.num_steps = 30;
  // image_on_ramp_.step_time_us = 100;
  // pixi_.prepare_ramps(&image_on_ramp_, image_on_ramps);
 
   // MAX11300::Ramp pgc_on_ramps[] = {
   //     // {ao1_freq_, to_dac(AO1_MOT), to_dac(AO1_PGC)},
   //     {ao2_atten_, to_dac(AO2_MOT), to_dac(var)},
   //     {ao3_atten_, to_dac(AO3_MOT), to_dac(AO3_PGC)},
   //     {eo_freq_, to_dac(EO_MOT), to_dac(EO_PGC)},
   // };
   // pgc_on_ramp_.configured = 1;
   // pgc_on_ramp_.num_ramps = ARRAYSIZE(pgc_on_ramps);
   // pgc_on_ramp_.num_steps = 5;
   // pgc_on_ramp_.step_time_us = 100;
   // pixi_.prepare_ramps(&pgc_on_ramp_, pgc_on_ramps);
 
#endif
  bsm_delay_ms(2);
}

void MiniG::run() {
#if TUNE
  for(float var = 6.4; var <= 7.5; var += 0.1) {
#endif
#if CENTER_PD
  for(float d_fall = -3000; d_fall <= 0; d_fall += 500) {
#endif
#if MW_RABI
  for (int pulse = 0; pulse <= 800; pulse += 20) {
#endif
#if RAMAN_RABI
    for (uint32_t raman = 0; raman <= 30; raman += 1) {
#endif
#if SPECTROSCOPY
      for (size_t j = 0; j < NUM_POINTS_SPEC; j++) {
        set_dds_params(specs[j]);
#endif
#if INTER
        for (size_t j = 0; j < NUM_POINTS_INTER; j++) {
          set_dds_params(fringes[j]);
#endif
#if RAMAN_RABI
          set_dds_params(fringes[0]);
#endif
#if TUNE
          reset(var);
#else
          reset(AO1_MOT);
#endif
          mot();
          pgc();

#if !MW_RABI
          int pulse = 300;
#endif
          mw(pulse);
          uint32_t T = 130;
          float fall_ms = 0.5;
          uint32_t fall_us = static_cast<uint32_t>(fall_ms * 1000);
#if CENTER_PD
          fall_us += d_fall;
#endif

#if !RAMAN_RABI
          uint32_t raman = 8;
#endif
          interferometry(T, fall_us, raman);

          image();
#if USE_CAMERA
          bsm_delay_ms(300);
#endif

#if SPECTROSCOPY
          printf("rd: %f\n", specs[j].detuning);
#elif INTER
          printf("rd: %f\n", fringes[j].actual_chirp);
#elif MW_RABI
          printf("rd: %d\n", pulse);
#elif RAMAN_RABI
          printf("rd: %lu\n", raman);
#elif CENTER_PD
          printf("rd: %f\n", d_fall);
#elif TUNE
          printf("rd: %f\n", var);
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
#if MW_RABI | CENTER_PD | TUNE
          printf("fr: %lu\n", f34 - bg);
#elif RAMAN_RABI | SPECTROSCOPY | INTER
          printf("fr: %f\n", detection);
          printf("atom_num: %lu\n\n", f34 - bg);
#endif
#if CENTER_PD | RAMAN_RABI | SPECTROSCOPY | INTER | MW_RABI | TUNE
        }
#endif
      }

      void MiniG::mot() {
        WRITE_IO(GPIOE, ao_2_ | cooling_shutter_, m_lock_);

        pixi_.run_ramps(&mot_on_ramp_);

        // Actual MOT Stage
        WRITE_IO(GPIOE, coils_, BITS_NONE);
        bsm_delay_ms(150);

        // Turn the MOT off
        WRITE_IO(GPIOE, BITS_NONE, coils_);

        bsm_delay_ms(9);
      }

      void MiniG::pgc() {
        WRITE_IO(GPIOE, laser_jump_, BITS_NONE);
        pixi_.run_ramps(&pgc_on_ramp_);
        bsm_delay_us(500);

        // Just hold it to cool
        bsm_delay_ms(10);

        // Turn off PGC
        WRITE_IO(GPIOE, BITS_NONE, cooling_shutter_);
        bsm_delay_ms(6);
      }

      void MiniG::mw(int pulse_duration, bool k_up) {
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
        uint32_t on_bits = ao_3_ | mot_eo_;
        uint32_t off_bits = BITS_NONE;
        if (k_up) {
          on_bits |= mw_dds_profile_pin_;
        } else {
          off_bits |= mw_dds_profile_pin_;
        }
        WRITE_IO(GPIOE, on_bits, off_bits);
#if MW_RABI
        bsm_delay_us(4000);
#else
    bsm_delay_us(4000 - pulse_duration);
#endif
      }

      // Start Interformetry
      void MiniG::interferometry(uint32_t T, uint32_t fall, uint32_t raman, bool k_up) {
        WRITE_IO(GPIOE, BITS_NONE,
                 ao_3_ | cooling_shutter_ | raman_eo_ | dds_switch_);
        pixi_.run_ramps(&raman_on_ramp_);
        // last 5 ms
        bsm_delay_ms(2);

        // Freefall
        if (k_up) {
        WRITE_IO(GPIOE, inter_dds_profile_pin_, BITS_NONE);
        } else {
        WRITE_IO(GPIOE, BITS_NONE, inter_dds_profile_pin_);
        }

#if INTER
        WRITE_IO(GPIOE, ao_2_|ao_3_, BITS_NONE);
        bsm_delay_us(5);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_|ao_3_);

        bsm_delay_ms(T);


        WRITE_IO(GPIOG, scope_, BITS_NONE);
#if INTER | SPECTROSCOPY
        WRITE_IO(GPIOE, ao_2_|ao_3_, BITS_NONE);
        bsm_delay_us(10);
#elif RAMAN_RABI
    WRITE_IO(GPIOE, ao_2_|ao_3_, BITS_NONE);
    bsm_delay_us(raman);
#endif
        WRITE_IO(GPIOE, BITS_NONE, ao_2_|ao_3_);

        bsm_delay_ms(T);

#if INTER
        WRITE_IO(GPIOE, ao_2_|ao_3_, BITS_NONE);
        bsm_delay_us(5);
#endif

        WRITE_IO(GPIOE, BITS_NONE, ao_2_|ao_3_);

        WRITE_IO(GPIOG, BITS_NONE, scope_);
        bsm_delay_us(fall);

        // Stop sweeping
      }

      void MiniG::image() {
        WRITE_IO(GPIOE, cooling_shutter_ | dds_switch_ | raman_eo_, BITS_NONE);
        pixi_.run_ramps(&image_on_ramp_);
        // takes 3 ms
        bsm_delay_ms(7);

        // Stabilize
        WRITE_IO(GPIOE, BITS_NONE, mot_eo_);
        bsm_delay_ms(1);
        bsm_delay_us(50);

        // Turn laser on
        WRITE_IO(GPIOE, m_lock_ | ao_3_
#if USE_CAMERA
                | camera_ttl_
#endif
                 ,
                 BITS_NONE);
        // TODO(bsm): write code for this in a minute
        bsm_delay_us(20);
        pixi_.max_speed_adc_read(photodiode_, samples, 127);

        // Repumping Stage
        WRITE_IO(GPIOE, ao_2_, BITS_NONE
#if USE_CAMERA
                | camera_ttl_
#endif
                 );
        bsm_delay_us(150);

        // Second Sample
        WRITE_IO(GPIOE, BITS_NONE, ao_2_);
        pixi_.max_speed_adc_read(photodiode_, &samples[127], 127);

        // Wait before background
        bsm_delay_ms(5);

        // Third detection for background
        pixi_.max_speed_adc_read(photodiode_, &samples[127 + 127], 127);

#if USE_CAMERA
        // Wait long time before Camera Background
        bsm_delay_ms(40);

        // Take background image
        WRITE_IO(GPIOE, camera_ttl_, BITS_NONE);

        bsm_delay_ms(10);
#endif
      }
