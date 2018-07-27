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

// Weird DMA thing: the first value needs to be the last one in the array then
// it will rotate through as usual.
// uint32_t raman_times_[3] = {8, 4, 4};

uint8_t samples[2049];
uint16_t samples16[1024];
uint8_t write_buf[2049];
void print_samples() {
  for (int i = 1; i < 2049; i += 2) {
    printf("%u,", static_cast<uint16_t>(samples[i] << 8) | samples[i + 1]);
  }
  printf("\n\n");
}
void print_samples16(int offset) {
  for (int i = offset; i < offset + 100; i += 1) {
    printf("%u,", samples16[i]);
  }
  printf("\n\n");
}


enum { TRANSFER_WAIT, TRANSFER_COMPLETE, TRANSFER_ERROR };
__IO uint32_t wTransferState = TRANSFER_WAIT;
void callme(int event) { wTransferState = TRANSFER_COMPLETE; }

int bg_end, bg_start, sample_1_start, sample_1_end, sample_2_start, sample_2_end;

} // namepsace

MiniG::MiniG()
    : ao_2_raman_{PB_0_ALT0}, coils_{D43, 0}, liquid_crystal_1_{D44, 0},
      under_vac_shutter_{D46, 0}, ao_3_{D48, 0}, ao_2_{PB_0_ALT0},
      cooling_shutter_{D49, 0}, mot_eo_{D50, 0}, raman_eo_{D51, 0},
      m_lock_{D53, 0}, analog_trigger_{D54, 0}, dds_switch_{D56, 0},
      dds_trigger_1_{D57, 0}, dds_trigger_2_{D58, 0}, scope_{D59, 0},
      camera_ttl_{D60, 0}, ao1_freq_{MAX11300::PORT10},
      ao2_atten_{MAX11300::PORT11}, ao3_atten_{MAX11300::PORT12},
      z_field_{MAX11300::PORT13}, ns_field_{MAX11300::PORT14},
      we_field_{MAX11300::PORT15}, eo_freq_{MAX11300::PORT16},
      bias_field_{MAX11300::PORT17}, laser_jump_{MAX11300::PORT18},
      m_horn_switch_{MAX11300::PORT19}, photodiode_{MAX11300::PORT0},
      dds_spi_{PB_5_ALT0, PB_4_ALT0, PB_3_ALT0},
      dds_{dds_spi_, dds_pins, 10000000 /* ref_freq */, 20 /* mult */},
      pixi_cs_{SPI_CS, 1}, pixi_spi_{SPI_MOSI, SPI_MISO, SPI_SCK},
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
  for(int i =0; i< 2049; i++) {
    write_buf[i] = 0;
  }

  reset();
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
  m_lock_ = 1;
  analog_trigger_ = 0;
  dds_switch_ = 1;
  dds_trigger_1_ = 0;
  dds_trigger_2_ = 0;
  scope_ = 0;
  camera_ttl_ = 0;

  AnalogOperations::set_voltage(pixi_, m_horn_switch_, 5);
  AnalogOperations::set_voltage(pixi_, laser_jump_, 0);
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
  scope_ = 0;
  wait_ms(10);
  printf("start sweep\n");
  // for (int pulse = 0; pulse <= 800; pulse += 20) {
  // for (int pulse = 0; pulse <= 800; pulse += 20) {
  for (uint32_t fall = 30; fall <= 160; fall += 2) {
  mot();
  pgc();

  int pulse = 600;
  mw(pulse);
  uint32_t T = 1;
  fall = 88;
  interferometry(T, fall);

  image();

  // TODO(bsm): Figure oute how to deal with this when looping
  reset();
  printf("T = %lu, fall = %lu, pulse = %d\n", T, fall, pulse);
  wait_ms(100);
  // print_samples();
  printf("sample 1 start: %d\n", sample_1_start);
  print_samples16(0);
  printf("sample 1 end: %d\n", sample_1_end);
  printf("sample 2 start: %d\n", sample_2_start);
  print_samples16(100);
  printf("sample 2 end: %d\n", sample_2_end);
  printf("bg start: %d\n", bg_start);
  print_samples16(200);
  printf("bg_end: %d\n", bg_end);
  }
  printf("end sweep\n");
}

void MiniG::mot() {
  ao_2_ = 1;
  cooling_shutter_ = 1;
  m_lock_ = 0;

  AnalogOperations::RampParams ramps[3] = {
      {ns_field_, 0, 0.001, 0},
      {we_field_, 0, 0.002, 0},
      {bias_field_, 0, 0.0015, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 3, 30, 3000 /* time in us */);

  // Actual MOT Stage
  coils_ = 1;
  wait_ms(1000);

  // Turn the MOT off
  coils_ = 0;

  wait_ms(8);
}

void MiniG::pgc() {
  AnalogOperations::set_voltage(pixi_, laser_jump_, 5);
  wait_us(200);

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
}

void MiniG::mw(int pulse_duration) {
  AnalogOperations::set_voltage(pixi_, laser_jump_, 0);
  liquid_crystal_1_ = 0;
  ao_2_ = 0;
  ao_3_ = 0;
  cooling_shutter_ = 1;

  AnalogOperations::RampParams ramps[5] = {
      {ao1_freq_, 6.6, 7.5, 0},  {ao2_atten_, 2.4, 0, 0},
      {ao3_atten_, 1.9, 10, 0},  {ns_field_, 0.01, 0.011, 0},
      {eo_freq_, 9.65, 9.21, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 5, 100,
                                    5000 /* time in us */);

  // Actually turn on Microwave
  AnalogOperations::set_voltage(pixi_, m_horn_switch_, 0);
  wait_us(pulse_duration);

  // Stop Microwave
  AnalogOperations::set_voltage(pixi_, m_horn_switch_, 5);
  wait_ms(2);

  // Blow away
  ao_3_ = 1;
  mot_eo_ = 1;
  // Sweep from 12Mhz to 11Mhz
  dds_trigger_2_ = 1;
  dds_.start_linear_sweep_down(AD9959::Channel1);
  wait_ms(10);
  scope_ = 0;

  // Stop external sweep to hold at 11Mhz
  dds_trigger_2_ = 0;
}

// Start Interformetry
void MiniG::interferometry(uint32_t T, uint32_t fall) {
  ao_3_ = 0;
  cooling_shutter_ = 0;
  raman_eo_ = 0;
  dds_switch_ = 0;

  AnalogOperations::RampParams ramps[6] = {
      {ao1_freq_, 7.5, 8.0, 0}, {ao2_atten_, 0, 2.9, 0},
      {ns_field_, 0.126, 0, 0}, {eo_freq_, 9.21, 6.5, 0},
      {we_field_, 0.033, 0, 0}, {bias_field_, 0.034, .5, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 6, 1500,
                                    15000 /* time in us */);

  scope_ = 1;
  // Freefall
  ao_3_ = 1;
  // sweep from 11Mhz to 5Mhz
  dds_trigger_1_ = 1;
  dds_.start_linear_sweep_down(AD9959::Channel0);
  wait_ms(5);

  wait_ms(fall);

  ao_2_ = 1;
  wait_us(4);
  ao_2_ = 0;

  wait_ms(T);

  ao_2_ = 1;
  wait_us(8);
  ao_2_ = 0;

  wait_ms(T);

  ao_2_ = 1;
  wait_us(4);
  ao_2_ = 0;

  wait_ms(fall);

  // Freefall
  ao_2_ = 0;
  wait_ms(7);

  scope_ = 0;

  // Stop sweeping
  dds_trigger_1_ = 0;
}

void MiniG::image() {
  ao_3_ = 0;
  AnalogOperations::RampParams ramps[2] = {
      {ao1_freq_, 8, 6.55, 0}, {ao2_atten_, 2.9, 0, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps, 2, 100,
                                    6000 /* time in us */);

  cooling_shutter_ = 1;
  raman_eo_ = 1;

  AnalogOperations::RampParams ramps1[3] = {
      {ao1_freq_, 6.55, 7.05, 0},
      {ao2_atten_, 0, 1.6, 0},
      {eo_freq_, 9.65, 8.89, 0},
  };
  // Ramp up Magnetic feild
  AnalogOperations::multi_port_ramp(pixi_, ramps1, 3, 570,
                                    5700 /* time in us */);

  // Stabilize
  mot_eo_ = 0;
  m_lock_ = 1;
  dds_switch_ = 1;
  wait_ms(1.5);
  wait_us(50);

  // Turn laser on
  ao_3_ = 1;
  ao_2_ = 0;

  // Take photodector data
  // Read Address for Photodiode

  // write_buf[0] = 0x81;
  // pixi_cs_ = 0;
  // pixi_spi_.transfer(write_buf, 2049, samples, 2049, callme);

  timer_.start();
  sample_1_start = timer_.read_us();
  AnalogOperations::max_speed_sample(pixi_, photodiode_, 100, samples16);
  sample_1_end = timer_.read_us();

  camera_ttl_ = 1;
  analog_trigger_ = 1;
  // TODO(bsm): write code for this in a minute
  wait_us(400);

  // Repumping Stage
  ao_2_ = 1;
  camera_ttl_ = 0;
  analog_trigger_ = 0;
  wait_us(150);

  // Second Sample
  ao_2_ = 0;
  sample_2_start = timer_.read_us();
  analog_trigger_ = 1;
  AnalogOperations::max_speed_sample(pixi_, photodiode_, 100, &samples16[100]);
  sample_2_end = timer_.read_us();
  wait_us(400);

  // Wait before background
  analog_trigger_ = 0;
  wait_ms(5);

  // Third detection for background
  bg_start = timer_.read_us();
  analog_trigger_ = 1;
  AnalogOperations::max_speed_sample(pixi_, photodiode_, 100, &samples16[200]);
  bg_end = timer_.read_us();
  wait_us(400);

  // Wait long time before Camera Background
  analog_trigger_ = 0;
  wait_ms(30);

  scope_ = 0;

  // Take background image
  camera_ttl_ = 1;

  // while (wTransferState == TRANSFER_WAIT);
  // pixi_cs_ = 1;
  wait_ms(10);
  timer_.stop();
  timer_.reset();
}
