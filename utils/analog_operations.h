#ifndef _UTILS_ANALOG_OPERATOINS_H_
#define _UTILS_ANALOG_OPERATOINS_H_

#include "../drivers/max11300/max11300.h"
#include "mbed.h"

namespace utils {
class AnalogOperations {
public:
  struct RampParams {
    drivers::max11300::MAX11300::MAX11300_Ports port;
    double start_voltage;
    double end_voltage;
    double step_size;
  };

  static void set_voltage(drivers::max11300::MAX11300 &pixi,
                          drivers::max11300::MAX11300::MAX11300_Ports port,
                          double voltage);

  // Assumes set voltage has already been called to  have everything at start_voltage.
  static void multi_port_ramp(drivers::max11300::MAX11300 &pixi,
                              RampParams *ramps, size_t num_ramps, uint32_t num_steps,
                              uint32_t ramp_time_us);

  static void ramp_voltage(drivers::max11300::MAX11300 &pixi,
                           drivers::max11300::MAX11300::MAX11300_Ports port,
                           double start_voltage, double end_voltage,
                           uint32_t num_steps, uint32_t ramp_time_us);

  static void max_speed_sample(drivers::max11300::MAX11300 &pixi,
                               drivers::max11300::MAX11300::MAX11300_Ports port,
                               size_t num_samples, uint16_t *read_buffer);
};

} // namespace utils
#endif //_UTILS_ANALOG_OPERATOINS_H_
