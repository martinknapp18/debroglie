/***********************************
Copyright ©2019. The Regents of the University of California (Regents). All Rights Reserved.
Permission to use, copy, modify, and distribute this software and its documentation for
educational, research, and not-for-profit purposes, without fee and without a signed licensing
agreement, is hereby granted, provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions. Contact The Office
of Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620,
(510) 643-7201, otl@berkeley.edu, http://ipira.berkeley.edu/industry-info for commercial licensing
opportunities.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
***********************************/

#ifndef _EXPERIMENT_MINIG_H_
#define _EXPERIMENT_MINIG_H_

#include "mbed.h"

#include "drivers/ad9959/ad9959.h"
#include "drivers/max11300/max11300.h"

class MiniG {
public:
  MiniG(bool k_up = true);
  virtual void init();
  virtual void run();

protected:
  virtual void reset(float var);
  virtual void mot();
  virtual void pgc();
  virtual void mw(int pulse);
  virtual void interferometry(uint32_t T, uint32_t fall, uint32_t raman);
  virtual void image();
  virtual void image_with_camera();
  virtual void integrate();

  bool k_up_;
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

  // End of shot storage variables
  float pd_fraction_;
  uint32_t atom_number_;

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
  drivers::max11300::MAX11300::MAX11300_Ports vibration_;
  drivers::max11300::MAX11300::MAX11300_Ports tilt_x_;
  drivers::max11300::MAX11300::MAX11300_Ports tilt_y_;

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
