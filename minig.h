#ifndef _EXPERIMENT_MINIG_H_
#define _EXPERIMENT_MINIG_H_

#include "mbed.h"

#include "drivers/ad9959/ad9959.h"
#include "drivers/max11300/max11300.h"
#include "TimerOc.h"

class MiniG {
public:
  MiniG();
  void init();
  void run();

private:
  void reset();
  void mot();
  void pgc();
  void mw(int pulse);
  void interferometry(uint32_t T, uint32_t fall);
  void image();

  // Precise Timing
  TimerOc ao_2_raman_;

  DigitalOut coils_;
  DigitalOut liquid_crystal_1_;
  DigitalOut under_vac_shutter_;
  DigitalOut ao_3_;
  DigitalOut ao_2_;
  DigitalOut cooling_shutter_;
  DigitalOut mot_eo_;
  DigitalOut raman_eo_;
  DigitalOut m_lock_;
  DigitalOut analog_trigger_;

  // Just in case of external DDS
  DigitalOut dds_switch_;
  DigitalOut dds_trigger_1_;
  DigitalOut dds_trigger_2_;

  // Debugging Utils
  DigitalOut scope_;

  DigitalOut camera_ttl_;

  // Analog Outputs
  drivers::max11300::MAX11300::MAX11300_Ports ao1_freq_;
  drivers::max11300::MAX11300::MAX11300_Ports ao2_atten_;
  drivers::max11300::MAX11300::MAX11300_Ports ao3_atten_;
  drivers::max11300::MAX11300::MAX11300_Ports z_field_;
  drivers::max11300::MAX11300::MAX11300_Ports ns_field_;
  drivers::max11300::MAX11300::MAX11300_Ports we_field_;
  drivers::max11300::MAX11300::MAX11300_Ports eo_freq_;
  drivers::max11300::MAX11300::MAX11300_Ports bias_field_;
  drivers::max11300::MAX11300::MAX11300_Ports laser_jump_;
  drivers::max11300::MAX11300::MAX11300_Ports m_horn_switch_;

  // Analog Input
  drivers::max11300::MAX11300::MAX11300_Ports photodiode_;

  SPI dds_spi_;
  drivers::ad9959::AD9959 dds_;

  DigitalOut pixi_cs_;
  SPI pixi_spi_;
  drivers::max11300::MAX11300 pixi_;
  
  // Timer for debug 
  Timer timer_;
};

#endif // _EXPERIMENT_MINIG_H_
