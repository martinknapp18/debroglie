#include "analog_operations.h"

namespace {
constexpr uint16_t to_dac(double volts) {
  return static_cast<uint16_t>(volts / 10.0 * 0x0fff);
}

} // namespace

namespace utils {
using drivers::max11300::MAX11300;

void AnalogOperations::set_voltage(MAX11300 &pixi,
                                   MAX11300::MAX11300_Ports port,
                                   double voltage) {
  auto to_write = to_dac(voltage);
  auto result = pixi.single_ended_dac_write(port, to_write);
  MBED_ASSERT(result == MAX11300::CmdResult::Success);
}

void AnalogOperations::multi_port_ramp(MAX11300 &pixi, RampParams *ramps,
                                       size_t num_ramps, uint32_t num_steps,
                                       uint32_t ramp_time_us) {
  int32_t pre_delay = ramp_time_us / num_steps - num_ramps*3*8*10;
  uint32_t delay = 0;
  if (pre_delay > 0) {
    delay = static_cast<uint32_t>(pre_delay);
  }

  for (size_t i = 0; i < num_ramps; i++) {
    ramps[i].step_size =
        (ramps[i].end_voltage - ramps[i].start_voltage) / num_steps;

    if (ramps[i].step_size == 0) {
      ramps[i].step_size = 1;
    }
  }
  while (num_steps > 0) {
    for (size_t i = 0; i < num_ramps; i++) {
        ramps[i].start_voltage += ramps[i].step_size;
        auto result =
            pixi.single_ended_dac_write(ramps[i].port,to_dac(ramps[i].start_voltage));
        MBED_ASSERT(result == MAX11300::CmdResult::Success);
    }
    wait_us(delay);
    num_steps--;
  }
}

void AnalogOperations::ramp_voltage(MAX11300 &pixi,
                                    MAX11300::MAX11300_Ports port,
                                    double start_voltage, double end_voltage,
                                    uint32_t num_steps, uint32_t ramp_time_us) {
  uint32_t delay = ramp_time_us / num_steps;
  uint16_t start = to_dac(start_voltage);
  uint16_t end = to_dac(end_voltage);
  if (start_voltage < end_voltage) {
    // Ascending loop
    uint16_t step_size = to_dac((end_voltage - start_voltage) / num_steps);
    if (step_size == 0) {
      step_size = 1;
    }
    while (start < end) {
      auto result = pixi.single_ended_dac_write(port, start);
      MBED_ASSERT(result == MAX11300::CmdResult::Success);
      start += step_size;
      wait_us(delay);
    }
  } else {
    // Descending loop
    uint16_t step_size = to_dac((start_voltage - end_voltage) / num_steps);
    if (step_size == 0) {
      step_size = 1;
    }

    while (start > end) {
      auto result = pixi.single_ended_dac_write(port, start);
      MBED_ASSERT(result == MAX11300::CmdResult::Success);
      start -= step_size;
      wait_us(delay);
    }
  }
}

void AnalogOperations::max_speed_sample(MAX11300 &pixi,
                                        MAX11300::MAX11300_Ports port,
                                        size_t num_samples,
                                        uint16_t *read_buffer) {
  for (size_t i = 0; i < num_samples; i++) {
    auto result = pixi.single_ended_adc_read(port, read_buffer[i]);
    MBED_ASSERT(result == MAX11300::CmdResult::Success);
  }
}
}
