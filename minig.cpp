#include "minig.h"

#include "utils/analog_operations.h"

using drivers::ad9959::AD9959;
using drivers::max11300::MAX11300;

using utils::AnalogOperations;

namespace {

constexpr AD9959::Pins dds_pins = {
    PA_4, /* CS */
    D6,   /* reset */
    D5,   /* update */
    D4,   /* p0 */
    D3    /* p1 */
};

} // namepsace

MiniG::MiniG()
    : coils_{D43, 0}, liquid_crystal_1_{D44, 0}, under_vac_shutter_{D46, 0},
      ao_2_{D47, 0}, ao_3_{D48, 0}, cooling_shutter_{D49, 0}, mot_eo_{D50, 0},
      raman_eo_{D51, 0}, m_lock_{D53, 0},
      analog_trigger_{D54, 0}, dds_switch_{D56, 0},
      dds_trigger_1_{D57, 0}, dds_trigger_2_{D58, 0},
      oscillscope_trigger_{D59, 0}, camera_ttl_{D60, 0},
      ao1_freq_{MAX11300::PORT10}, ao2_atten_{MAX11300::PORT11},
      ao3_atten_{MAX11300::PORT12}, z_field_{MAX11300::PORT13},
      ns_field_{MAX11300::PORT14}, we_field_{MAX11300::PORT15},
      eo_freq_{MAX11300::PORT16}, bias_field_{MAX11300::PORT17},
      laser_jump_{MAX11300::PORT18}, m_horn_switch_{MAX11300::PORT19}, 
      photodiode_{MAX11300::PORT0}, dds_spi_{PB_5, PB_4, PB_3},
      dds_{dds_spi_, dds_pins, 24000000 /* ref_freq */, 10 /* mult */},
      pixi_spi_{SPI_MOSI, SPI_MISO, SPI_SCK},
      pixi_{pixi_spi_, SPI_CS /* cs */, NC, NC} {}

void MiniG::init() {
  // Note: Do not reset after initializing, or you'll waste time debugging for
  // no
  // reason.
  dds_.init();
  pixi_.init();
  dds_.set_freq_linear_sweep_params(AD9959::Channel0, 5893365.1, 11871770,
                                    2600000, 2600000, .0000001, .0000001);
  dds_.set_freq_linear_sweep_params(AD9959::Channel1, 11871770, 12631770,
                                    100000, 100000, .0000001, .0000001);
  dds_.io_update();

  reset();
  wait_ms(2);
}

void MiniG::reset() {
  coils_ = 0;
  liquid_crystal_1_ = 1;
  under_vac_shutter_ = 1;
  ao_2_ = 0;
  ao_3_ = 1;
  cooling_shutter_ = 0;
  mot_eo_ = 0;
  raman_eo_ = 1;
  m_lock_ = 0;
  analog_trigger_ = 0;
  dds_switch_ = 1;
  dds_trigger_1_ = 0;
  dds_trigger_2_ = 0;
  oscillscope_trigger_ = 0;
  camera_ttl_ = 0;

  AnalogOperations::set_voltage(pixi_, m_horn_switch_, 5);
  AnalogOperations::set_voltage(pixi_, laser_jump_ , 0);
  AnalogOperations::set_voltage(pixi_, ao1_freq_, 6.6);
  AnalogOperations::set_voltage(pixi_, ao2_atten_, 1.78);
  AnalogOperations::set_voltage(pixi_, ao3_atten_, 5);
  AnalogOperations::set_voltage(pixi_, z_field_, 0);
  AnalogOperations::set_voltage(pixi_, ns_field_, 0);
  AnalogOperations::set_voltage(pixi_, we_field_, 0);
  AnalogOperations::set_voltage(pixi_, eo_freq_, 8.95);
  AnalogOperations::set_voltage(pixi_, bias_field_, 0);

  dds_.start_linear_sweep_up(AD9959::Channel0);
  dds_.start_linear_sweep_up(AD9959::Channel1);
  wait_ms(2);
}

void MiniG::run() {
  oscillscope_trigger_ = 0;
  wait_ms(10);
  printf("start sweep\n");
  // for (int T = 78; T <= 90; T++) {
    mot();
    pgc();

    mw();
    // interferometry(T);
    interferometry(80);

    image();

    // TODO(bsm): Figure oute how to deal with this when looping
    reset();
    wait_ms(100);
  // }
  printf("end sweep\n");
}

void MiniG::mot() {
  ao_2_ = 1;
  cooling_shutter_ = 1;
  m_lock_ = 0;

  AnalogOperations::RampParams ramps[3] = {
      {ns_field_, 0, 0.026, 0},
      {we_field_, 0, 0.033, 0},
      {bias_field_, 0, 0.034, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 3, 30, 3000 /* time in us */);

  // Actual MOT Stage
  coils_ = 1;
  wait_ms(500);

  // Turn the MOT off
  coils_ = 0;

  wait_ms(8);
}

void MiniG::pgc() {

  // pixi_.gpio_write(laser_jump_, 1);
  AnalogOperations::set_voltage(pixi_, laser_jump_ , 5);
  wait_us(200);
  // AnalogOperations::set_voltage(pixi_, ao2_atten_, 2.4);
  // AnalogOperations::set_voltage(pixi_, ao3_atten_, 1.9);
  // AnalogOperations::set_voltage(pixi_, eo_freq_, 9.65);

   AnalogOperations::RampParams ramps[3] = {
       {ao2_atten_, 1.78, 2.4, 0},
       {ao3_atten_, 5, 1.9, 0},
       {eo_freq_, 8.95, 9.65, 0},
   };
 
   // Ramp up Magnetic feild
   AnalogOperations::multi_port_ramp(pixi_, ramps, 3, 100,
                                     1000 /* time in us */);
 
  // Just hold it to cool
  wait_ms(10);

  // Turn off PGC
  cooling_shutter_ = 0;

  wait_ms(6);
  AnalogOperations::set_voltage(pixi_, laser_jump_ , 0);
}

void MiniG::mw() {
  liquid_crystal_1_ = 0;
  ao_2_ = 0;
  ao_3_ = 0;
  cooling_shutter_ = 1;

  AnalogOperations::RampParams ramps[4] = {
      {ao1_freq_, 6.6, 7.5, 0},  {ao2_atten_, 2.4, 0, 0},
      {ao3_atten_, 1.9, 10, 0}, //  {ns_field_, 0.026, 0.126, 0},
      {eo_freq_, 9.65, 9.21, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 4, 100,
                                    5000 /* time in us */);


  AnalogOperations::set_voltage(pixi_, ns_field_, 0);
  AnalogOperations::set_voltage(pixi_, we_field_, 0);
  AnalogOperations::set_voltage(pixi_, bias_field_, 0);
  // Actually turn on Microwave
  AnalogOperations::set_voltage(pixi_, m_horn_switch_, 0);
  wait_us(200);

  // Stop Microwave
  AnalogOperations::set_voltage(pixi_, m_horn_switch_, 5);

  // Sweep from 12Mhz to 11Mhz
  
  dds_trigger_2_ = 1;
  dds_.start_linear_sweep_down(AD9959::Channel1);
  wait_ms(5);

  // Blow away
  // ao_3_ = 1;
  mot_eo_ = 1;
  wait_ms(5);

  // Stop external sweep to hold at 11Mhz
  dds_trigger_2_ = 0;
}

// Start Interformetry
void MiniG::interferometry(uint32_t T) {
  ao_3_ = 0;
  cooling_shutter_ = 0;
  raman_eo_ = 0;
  dds_switch_ = 0;

  AnalogOperations::RampParams ramps[6] = {
      {ao1_freq_, 7.5, 8.0, 0}, {ao2_atten_, 0, 2.7, 0},
      {ns_field_, 0.126, 0, 0}, {eo_freq_, 9.21, 6.5, 0},
      {we_field_, 0.033, 0, 0}, {bias_field_, 0.034, .5, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 6, 1500,
                                    15000 /* time in us */);

  // Freefall
  // ao_3_ = 1;
  // sweep from 11Mhz to 5Mhz
  dds_trigger_1_ = 1;
  dds_.start_linear_sweep_down(AD9959::Channel0);
  wait_ms(5);


  {
  DeepSleepLock lock;
  oscillscope_trigger_ = 1;
  // Raman Pulse 1
  ao_2_ = 1;
  // ao_2_ = 0;
  wait_us(4);

  // First T
  ao_2_ = 0;
  wait_ms(T);

  // Raman Pulse 2
  ao_2_ = 1;
  // ao_2_ = 0;
  wait_us(8);

  // Second T
  ao_2_ = 0;
  wait_ms(T);

  // Raman Pulse 3
  ao_2_ = 1;
  // ao_2_ = 0;
  wait_us(4);
  // Freefall
  ao_2_ = 0;
  oscillscope_trigger_ = 0;
  }
  wait_ms(7);

  // Stop sweeping
  dds_trigger_1_ = 0;
}

void MiniG::image() {
  ao_3_ = 0;
  cooling_shutter_ = 1;
  oscillscope_trigger_ = 0;
  raman_eo_ = 1;

 //  AnalogOperations::set_voltage(pixi_, ao2_atten_, 2.4);
 //  AnalogOperations::set_voltage(pixi_, ao3_atten_, 1.9);
 //  AnalogOperations::set_voltage(pixi_, eo_freq_, 9.65);


   AnalogOperations::RampParams ramps[3] = {
       // {ao1_freq_, 8.0, 7.05, 0},
       {ao2_atten_, 2.4, 1.6, 0},
       {eo_freq_, 9.65, 8.89, 0},
       {ao3_atten_, 1.9, 10.0, 0},
   };
   // Ramp up Magnetic feild
   AnalogOperations::multi_port_ramp(pixi_, ramps, 3, 570,
                                     5700 /* time in us */);
 
  // Stabilize
  raman_eo_ = 0;
  wait_ms(1);

  // Turn laser on
  ao_3_ = 1;
  dds_switch_ = 1;
  wait_us(50);

  // Take photodector data
  camera_ttl_ = 1;
  // m_lock_ = 1;
  m_lock_ = 0;
  analog_trigger_ = 1;
  // TODO(bsm): write code for this in a minute
  // Write sampling here
  wait_us(400);

  // Repumping Stage
  ao_2_ = 1;
  camera_ttl_ = 0;
  analog_trigger_ = 0;
  wait_us(150);

  // Second Sample
  ao_2_ = 0;
  analog_trigger_ = 1;
  wait_us(400);

  // Wait before background
  analog_trigger_ = 0;
  wait_ms(5);

  // Third detection for background
  analog_trigger_ = 1;
  wait_us(400);

  // Wait long time before Camera Background
  analog_trigger_ = 0;
  wait_ms(40);

  // Take background image
  camera_ttl_ = 1;
  wait_ms(4);
}
