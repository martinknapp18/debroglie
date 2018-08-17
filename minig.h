#ifndef _EXPERIMENT_MINIG_H_
#define _EXPERIMENT_MINIG_H_

#include "mbed.h"

#include "drivers/ad9959/ad9959.h"
#include "drivers/max11300/max11300.h"

class MiniG {
public:
  MiniG();
  void init();
  void run();

private:
  void reset(float we);
  void mot();
  void pgc();
  void mw(int pulse);
  void interferometry(uint32_t T, uint32_t fall, uint32_t raman);
  void image();
  void set_dds_params(double chirp_rate_kHz_p_ms, double raman_detuning_kHz);

  const uint32_t coils_;
  const uint32_t liquid_crystal_1_;
  const uint32_t under_vac_shutter_;
  const uint32_t ao_3_;
  const uint32_t ao_2_;
  const uint32_t cooling_shutter_;
  const uint32_t mot_eo_;
  const uint32_t raman_eo_;
  const uint32_t m_lock_;
  const uint32_t analog_trigger_;

  // Just in case of external DDS
  const uint32_t mw_dds_profile_pin_;
  const uint32_t dds_switch_;
  const uint32_t inter_dds_profile_pin_;

  // Debugging Utils
  const uint32_t scope_;

  const uint32_t camera_ttl_;
  const uint32_t laser_jump_;
  const uint32_t m_horn_switch_;

  // Analog Outputs
  drivers::max11300::MAX11300::MAX11300_Ports ao1_freq_;
  drivers::max11300::MAX11300::MAX11300_Ports ao2_atten_;
  drivers::max11300::MAX11300::MAX11300_Ports ao3_atten_;
  drivers::max11300::MAX11300::MAX11300_Ports z_field_;
  drivers::max11300::MAX11300::MAX11300_Ports ns_field_;
  drivers::max11300::MAX11300::MAX11300_Ports we_field_;
  drivers::max11300::MAX11300::MAX11300_Ports eo_freq_;
  drivers::max11300::MAX11300::MAX11300_Ports bias_field_;
  // Analog Input
  drivers::max11300::MAX11300::MAX11300_Ports photodiode_;

  SPI dds_spi_;
  drivers::ad9959::AD9959 dds_;

  SPI pixi_spi_;
  drivers::max11300::MAX11300 pixi_;

  drivers::max11300::MAX11300::RampAction mot_on_ramp_;
  drivers::max11300::MAX11300::RampAction pgc_on_ramp_;
  drivers::max11300::MAX11300::RampAction mw_on_ramp_;
  drivers::max11300::MAX11300::RampAction raman_on_ramp_;
  drivers::max11300::MAX11300::RampAction image_on_ramp_;
};

#endif // _EXPERIMENT_MINIG_H_
