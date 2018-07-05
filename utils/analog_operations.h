#ifndef _UTILS_ANALOG_OPERATOINS_H_
#define _UTILS_ANALOG_OPERATOINS_H_

#include "mbed.h"
#include "../drivers/max11300/max11300.h"

namespace utils {
class AnalogOperations {
  public:
    static void set_voltage(drivers::max11300::MAX11300 & pixi,
        drivers::max11300::MAX11300::MAX11300_Ports port,
        double voltage);

    static void ramp_voltage(drivers::max11300::MAX11300 & pixi,
        drivers::max11300::MAX11300::MAX11300_Ports port,
        double start_voltage,
        double end_voltage,
        uint32_t num_steps,
        uint32_t ramp_time_us);

    static void max_speed_sample(drivers::max11300::MAX11300 & pixi,
        drivers::max11300::MAX11300::MAX11300_Ports port,
        size_t num_samples,
        uint16_t * read_buffer);
};


}  // namespace utils
#endif  //_UTILS_ANALOG_OPERATOINS_H_
