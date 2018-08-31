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

  struct dds_params_t {
    uint8_t mult;
    uint32_t clk_transition_hex;
    uint32_t chirp_start_hex;
    uint32_t chirp_stop_hex;
    uint32_t mw_time_hex;
    uint32_t mw_freq_hex;
    uint32_t chirp_time_hex;
    uint32_t chirp_freq_hex;
    double actual_chirp;
  };


  /*
   *
-void MiniG::set_dds_params(dds_params_t params) {
+void MiniG::set_dds_params(double chirp_rate_kHz_p_ms,
+                           double raman_detuning_kHz) {
+
+  constexpr double free_fall_time_s = .260;
+  constexpr double num_steps = 500000;
+  // constexpr double num_steps = 130;
+  constexpr double num_steps_mw = 50000000;
+  constexpr double clock_transition = 12631770;
+  double chirp_start_hz = raman_detuning_kHz * 1000 + clock_transition;
+  double chirp_stop_hz =
+      chirp_start_hz + free_fall_time_s * 1000 * chirp_rate_kHz_p_ms * 1000;
   dds_.reset();
-  dds_.set_freq_linear_sweep_params(
-      AD9959::Channel0, params.mult, params.chirp_start_hex,
-      params.chirp_stop_hex, params.chirp_freq_hex, params.chirp_time_hex);
-
-  dds_.set_freq_linear_sweep_params(
-      AD9959::Channel1, params.mult, params.clk_transition_hex,
-      params.chirp_start_hex, params.mw_freq_hex, params.mw_time_hex);
+  dds_.set_freq_linear_sweep_params(AD9959::Channel0, chirp_start_hz,
+                                    chirp_stop_hz, free_fall_time_s * num_steps,
+                                    1/ num_steps);
+  dds_.set_freq_linear_sweep_params(AD9959::Channel1, clock_transition,
+                                    chirp_start_hz, .01 * num_steps_mw,
+                                    1 / num_steps_mw);
   dds_.io_update();
 }


   *
   *
   * */
private:
  void reset();
  void mot();
  void pgc();
  void mw(int pulse);
  void interferometry(uint32_t T, uint32_t fall, uint32_t raman);
  void image();
  void set_dds_params(dds_params_t params);
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
