/***********************************
Copyright ©2019. The Regents of the University of California (Regents). All
Rights Reserved.
Permission to use, copy, modify, and distribute this software and its
documentation for
educational, research, and not-for-profit purposes, without fee and without a
signed licensing
agreement, is hereby granted, provided that the above copyright notice, this
paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office
of Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620,
(510) 643-7201, otl@berkeley.edu, http://ipira.berkeley.edu/industry-info for
commercial licensing
opportunities.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
INCIDENTAL, OR
CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
SOFTWARE AND ITS
DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
AND ACCOMPANYING
DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO
OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
***********************************/

#include "cycle_count_delay.h"
#include "minig.h"
#include "pinmap.h"
#include "util/dds_config.h"
#include "util/macros.h"

using drivers::ad9959::AD9959;
using drivers::max11300::MAX11300;
using util::DDSConfig;

#define SPECTROSCOPY 0
#define RAMAN_RABI 0
#define MW_RABI 0
#define INTER 1
#define DEBUG_PD 0
#define USE_CAMERA 0
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

DDSConfig dds_config;

#include "experiment/settings.h"
} // namepsace

// clang-format off
MiniG::MiniG(bool k_up) :
      k_up_{k_up},
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
  if (k_up_) {
    dds_config.configure_up(dds_, fringes[0]);
  } else {
    dds_config.configure_down(dds_, fringes[0]);
  }
  reset(AO1_MOT);
}

void MiniG::reset(float var) {
  uint32_t ON_PINS = liquid_crystal_1_ | ao_3_ | raman_eo_ | m_lock_ |
                     dds_switch_ | m_horn_switch_;
  uint32_t OFF_PINS = coils_ | ao_2_ | cooling_shutter_ | mot_eo_ | raman_eo_ |
                      analog_trigger_ | camera_ttl_ | laser_jump_;
  if (k_up_) {
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
  cycle_delay_ms(2);
}

void MiniG::run() {
#if TUNE
  for (float var = 6.4; var <= 7.5; var += 0.1) {
#endif
#if CENTER_PD
  for (float d_fall = -3000; d_fall <= 0; d_fall += 500) {
#endif
#if MW_RABI
  for (int pulse = 0; pulse <= 800; pulse += 20) {
#endif
#if RAMAN_RABI
  for (uint32_t raman = 0; raman <= 30; raman += 1) {
    if (k_up_) {
      dds_config.configure_up(dds_, fringes[0]);
    } else {
      dds_config.configure_down(dds_, fringes[0]);
    }
#endif
#if SPECTROSCOPY
  for (size_t j = 0; j < NUM_POINTS_SPEC; j++) {
    if (k_up_) {
      dds_config.configure_up(dds_, specs[j]);
    } else {
      dds_config.configure_down(dds_, specs[j]);
    }

#endif
#if INTER
  for (size_t j = 0; j < NUM_POINTS_INTER; j++) {
    if (k_up_) {
      dds_config.configure_up(dds_, fringes[j]);
    } else {
      dds_config.configure_down(dds_, fringes[j]);
    }

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
    uint32_t T = 129;
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
    cycle_delay_ms(300);
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
  cycle_delay_ms(150);

  // Turn the MOT off
  WRITE_IO(GPIOE, BITS_NONE, coils_);

  cycle_delay_ms(9);
}

void MiniG::pgc() {
  WRITE_IO(GPIOE, laser_jump_, BITS_NONE);
  pixi_.run_ramps(&pgc_on_ramp_);
  cycle_delay_us(500);

  // Just hold it to cool
  cycle_delay_ms(10);

  // Turn off PGC
  WRITE_IO(GPIOE, BITS_NONE, cooling_shutter_);
  cycle_delay_ms(6);
}

void MiniG::mw(int pulse_duration) {
  WRITE_IO(GPIOE, cooling_shutter_,
           laser_jump_ | liquid_crystal_1_ | ao_2_ | ao_3_);

  pixi_.run_ramps(&mw_on_ramp_);
  cycle_delay_ms(2);

  // Actually turn on Microwave
  WRITE_IO(GPIOE, BITS_NONE, m_horn_switch_);
  cycle_delay_us(pulse_duration);

  // Stop Microwave
  WRITE_IO(GPIOE, m_horn_switch_, BITS_NONE);
  cycle_delay_ms(1);

  // Blow away
  uint32_t on_bits = ao_3_ | mot_eo_;
  uint32_t off_bits = BITS_NONE;
  if (k_up_) {
    on_bits |= mw_dds_profile_pin_;
  } else {
    off_bits |= mw_dds_profile_pin_;
  }
  WRITE_IO(GPIOE, on_bits, off_bits);
#if MW_RABI
  cycle_delay_us(4000);
#else
  cycle_delay_us(4000 - pulse_duration);
#endif
}

// Start Interformetry
void MiniG::interferometry(uint32_t T, uint32_t fall,
                           uint32_t raman) {
  WRITE_IO(GPIOE, BITS_NONE,
           ao_3_ | cooling_shutter_ | raman_eo_);
  pixi_.run_ramps(&raman_on_ramp_);
  // last 5 ms
  cycle_delay_ms(2);
  WRITE_IO(GPIOE, BITS_NONE, dds_switch_);

  // Freefall
  if (k_up_) {
    WRITE_IO(GPIOE, inter_dds_profile_pin_, BITS_NONE);
  } else {
    WRITE_IO(GPIOE, BITS_NONE, inter_dds_profile_pin_);
  }
  cycle_delay_ms(2);

  WRITE_IO(GPIOG, scope_, BITS_NONE);
#if INTER
  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(5);
#elif RAMAN_RABI
  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(raman);
#endif
  WRITE_IO(GPIOE, BITS_NONE, ao_2_ | ao_3_);

  cycle_delay_ms(T);

#if INTER | SPECTROSCOPY
  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(10);
#endif
  WRITE_IO(GPIOE, BITS_NONE, ao_2_ | ao_3_);

  cycle_delay_ms(T);

#if INTER
  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(5);
#endif

  WRITE_IO(GPIOE, BITS_NONE, ao_2_ | ao_3_);

  WRITE_IO(GPIOG, BITS_NONE, scope_);
  cycle_delay_us(fall);

  // Stop sweeping
}

void MiniG::image() {
  WRITE_IO(GPIOE, cooling_shutter_ | dds_switch_ | raman_eo_,
           BITS_NONE);
  pixi_.run_ramps(&image_on_ramp_);
  // takes 3 ms
  cycle_delay_ms(7);

  // Stabilize
  WRITE_IO(GPIOE, BITS_NONE, mot_eo_);
  cycle_delay_ms(1);
  cycle_delay_us(50);

  // Turn laser on
  WRITE_IO(GPIOE, m_lock_ | ao_3_
#if USE_CAMERA
  | camera_ttl_
#endif
  , BITS_NONE);
  // TODO(bsm): write code for this in a minute
  cycle_delay_us(20);
  pixi_.max_speed_adc_read(photodiode_, samples, 127);

  // Repumping Stage
  WRITE_IO(GPIOE, ao_2_, BITS_NONE
#if USE_CAMERA
  | camera_ttl_
#endif
  );
  cycle_delay_us(150);

  // Second Sample
  WRITE_IO(GPIOE, BITS_NONE, ao_2_);
  pixi_.max_speed_adc_read(photodiode_, &samples[127], 127);

  // Wait before background
  cycle_delay_ms(5);

  // Third detection for background
  pixi_.max_speed_adc_read(photodiode_, &samples[127 + 127], 127);

#if USE_CAMERA
  // Wait long time before Camera Background
  cycle_delay_ms(40);

  // Take background image
  WRITE_IO(GPIOE, camera_ttl_, BITS_NONE);

  cycle_delay_ms(10);
#endif
}
