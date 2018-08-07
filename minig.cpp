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
      dds_switch_{GPIO_PIN_10}, //GPIOE
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

void MiniG::set_dds_params(double delta) {
  dds_.reset();
  dds_.set_freq_linear_sweep_params(AD9959::Channel0, 5893365.1,
                                    12631770 /* + delta * 1000*/, 2600000,
                                    2600000, .0000001, .0000001);
  dds_.set_freq_linear_sweep_params(AD9959::Channel1, 12631770 + delta * 1000,
                                    12631770, 100000, 100000, .0000001,
                                    .0000001);
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
      {ns_field_, to_dac(0), to_dac(0.1)},
      {we_field_, to_dac(0), to_dac(0.3)},
      {bias_field_, to_dac(0), to_dac(0.33)},
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

  MAX11300::Ramp mw_on_ramps[] = {
      {ao1_freq_, to_dac(6.6), to_dac(7.5)},
      {ao2_atten_, to_dac(2.4), to_dac(0)},
      {ao3_atten_, to_dac(1.88), to_dac(10)},
      {we_field_, to_dac(0.3), to_dac(3.5)},
      {eo_freq_, to_dac(9.85), to_dac(9.21)},
  };
  mw_on_ramp_.configured = 0;
  mw_on_ramp_.num_ramps = ARRAYSIZE(mw_on_ramps);
  mw_on_ramp_.num_steps = 30;
  mw_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&mw_on_ramp_, mw_on_ramps);

  MAX11300::Ramp raman_on_ramps[] = {
    //2ms
      {ao1_freq_, to_dac(7.5), to_dac(8.0)},
      {ao2_atten_, to_dac(0), to_dac(2.8)},
      // Should be 5ms
      {ns_field_, to_dac(0.1), to_dac(0)},
      {we_field_, to_dac(3.5), to_dac(0)},
      // 2 ms
      {eo_freq_, to_dac(9.21), to_dac(6.5)},
      // should be 5 ms again
      {bias_field_, to_dac(0.33), to_dac(7)},
  };
  raman_on_ramp_.configured = 0;
  raman_on_ramp_.num_ramps = ARRAYSIZE(raman_on_ramps);
  raman_on_ramp_.num_steps = 20;
  raman_on_ramp_.step_time_us = 100;
  pixi_.prepare_ramps(&raman_on_ramp_, raman_on_ramps);

  MAX11300::Ramp image_on_ramps[] = {
      {ao1_freq_, to_dac(8), to_dac(7.1)},
      {ao2_atten_, to_dac(2.8), to_dac(1.75)},
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
  set_dds_params(-790);
  reset(0);
}

void MiniG::reset(float bias) {
  WRITE_IO(GPIOE, liquid_crystal_1_ | ao_3_ | raman_eo_ | m_lock_ |
                      dds_switch_ | m_horn_switch_,
           coils_ | ao_2_ | cooling_shutter_ | mot_eo_ | raman_eo_ |
               analog_trigger_ | camera_ttl_ | laser_jump_);
  WRITE_IO(GPIOG, under_vac_shutter_, scope_);

  pixi_.single_ended_dac_write(ao1_freq_, to_dac(6.75));
  pixi_.single_ended_dac_write(ao2_atten_, to_dac(1.75));
  pixi_.single_ended_dac_write(ao3_atten_, to_dac(5));
  pixi_.single_ended_dac_write(z_field_, to_dac(0));
  pixi_.single_ended_dac_write(ns_field_, to_dac(0));
  pixi_.single_ended_dac_write(we_field_, to_dac(0));
  pixi_.single_ended_dac_write(eo_freq_, to_dac(8.97));
  pixi_.single_ended_dac_write(bias_field_, to_dac(0));

  // TODO(bsm): move these up with the other digital signals
  dds_.start_linear_sweep_up(AD9959::Channel0);
  dds_.start_linear_sweep_up(AD9959::Channel1);
  bsm_delay_ms(2);
}

void MiniG::run() {
  printf("start sweep\n");
  // for (int pulse = 0; pulse <= 1000; pulse += 20) {
  // for (double delta = -120; delta <= -20; delta += 20) {
  // for(float fall = 40; fall <= 180; fall += 5) {
  // for (float bias = 0; bias <= 5.0; bias += .2) {
    reset(0);
    mot();
    pgc();

    int pulse = 245;
    mw(pulse);
    uint32_t T = 5;
    uint32_t fall = 183;
    interferometry(T, fall);

    image();

    // double delta = -790;
    // set_dds_params(delta);
    // printf("fall: %f\n", fall);
    // printf("delta: %f\n", delta);
    // printf("bias: %f\n", bias);
    // printf("pulse: %d\n", pulse);
  // }
  printf("end sweep\n");
}

void MiniG::mot() {
  WRITE_IO(GPIOE, ao_2_ | cooling_shutter_, m_lock_);

  pixi_.run_ramps(&mot_on_ramp_);

  // Actual MOT Stage
  WRITE_IO(GPIOE, coils_, BITS_NONE);
  bsm_delay_ms(1000);

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
  WRITE_IO(GPIOG, scope_, BITS_NONE);
  WRITE_IO(GPIOE, BITS_NONE, m_horn_switch_);
  bsm_delay_us(pulse_duration);

  // Stop Microwave
  WRITE_IO(GPIOE, m_horn_switch_, BITS_NONE);
  WRITE_IO(GPIOG, BITS_NONE, scope_);
  bsm_delay_ms(1);

  // Blow away
  WRITE_IO(GPIOE, ao_3_ |  mot_eo_, BITS_NONE);
  // Sweep from 12Mhz to 11Mhz
  // dds_.start_linear_sweep_down(AD9959::Channel1);
  bsm_delay_ms(10);

  // Stop external sweep to hold at 11Mhz
}

// Start Interformetry
void MiniG::interferometry(uint32_t T, uint32_t fall) {
  WRITE_IO(GPIOE, BITS_NONE,
           ao_3_ | cooling_shutter_ | raman_eo_ | dds_switch_);
  pixi_.run_ramps(&raman_on_ramp_);
  // last 2 ms
  bsm_delay_ms(11);

  // Freefall
  WRITE_IO(GPIOE, ao_3_, BITS_NONE);
  // sweep from 11Mhz to 5Mhz
  // dds_.start_linear_sweep_down(AD9959::Channel0);
  bsm_delay_ms(55);

  // WRITE_IO(GPIOE, ao_2_, BITS_NONE);
  bsm_delay_us(4);
  WRITE_IO(GPIOE, BITS_NONE, ao_2_);

  bsm_delay_ms(T);

  // WRITE_IO(GPIOE, ao_2_, BITS_NONE);
  bsm_delay_us(8);
  WRITE_IO(GPIOE, BITS_NONE, ao_2_);

  bsm_delay_ms(T);

  // WRITE_IO(GPIOE, ao_2_, BITS_NONE);
  bsm_delay_us(4);
  WRITE_IO(GPIOE, BITS_NONE, ao_2_);

  bsm_delay_ms(fall);

  // Stop sweeping
}

void MiniG::image() {
  WRITE_IO(GPIOE, cooling_shutter_ | dds_switch_ | raman_eo_, ao_3_);
  pixi_.run_ramps(&image_on_ramp_);
  // takes 2 ms
  bsm_delay_ms(5);

  // Stabilize
  WRITE_IO(GPIOE, BITS_NONE, mot_eo_);
  bsm_delay_us(1800);

  WRITE_IO(GPIOE, m_lock_, BITS_NONE);
  bsm_delay_us(50);

  // Turn laser on
  WRITE_IO(GPIOE, ao_3_ | camera_ttl_ | analog_trigger_, BITS_NONE);
  // TODO(bsm): write code for this in a minute
  bsm_delay_us(400);

  // Repumping Stage
  WRITE_IO(GPIOE, ao_2_, camera_ttl_ | analog_trigger_);
  bsm_delay_us(150);

  // Second Sample
  WRITE_IO(GPIOE, analog_trigger_, ao_2_);
  bsm_delay_us(400);

  // Wait before background
  WRITE_IO(GPIOE, BITS_NONE, analog_trigger_);
  bsm_delay_ms(5);

  // Third detection for background
  WRITE_IO(GPIOE, analog_trigger_, BITS_NONE);
  bsm_delay_us(400);

  // Wait long time before Camera Background
  WRITE_IO(GPIOE, BITS_NONE, analog_trigger_);
  bsm_delay_ms(40);

  // Take background image
  WRITE_IO(GPIOE, camera_ttl_, BITS_NONE);

  bsm_delay_ms(10);
}
