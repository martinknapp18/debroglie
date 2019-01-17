/***********************************
Copyright ©2019. The Regents of the University of California (Regents). All
Rights Reserved.
Permission to use, copy, modify, and distribute this software and its
documentation for
educational, research, and not-for-profit purposes, without fee and without a
signed licensing
agreement, is hereby granted, provided that the above copyright notice, this
paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office
of Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620,
(510) 643-7201, otl@berkeley.edu, http://ipira.berkeley.edu/industry-info for
commercial licensing
opportunities.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
INCIDENTAL, OR
CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
SOFTWARE AND ITS
DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
AND ACCOMPANYING
DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO
OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
***********************************/

#include "dds_config.h"

using drivers::ad9959::AD9959;

namespace util {
void DDSConfig::configure_up(AD9959 dds, params_t params) {
  dds.reset();
  dds.set_freq_linear_sweep_params(AD9959::Channel0, params.mult,
                                   params.chirp_start_hex,
                                   params.chirp_stop_hex,
                                   params.chirp_freq_hex,      // up
                                   params.chirp_freq_hex * 10, // down
                                   params.chirp_time_hex,      // up
                                   params.chirp_time_hex       // down
                                   );

  dds.set_freq_linear_sweep_params(AD9959::Channel1, params.mult,
                                   params.clk_transition_hex,
                                   params.chirp_start_hex,
                                   params.mw_freq_hex,      // up
                                   params.mw_freq_hex * 10, // down
                                   params.mw_time_hex,      // up
                                   params.mw_time_hex       // down
                                   );

  dds.io_update();
}
void DDSConfig::configure_down(AD9959 dds, params_t params) {
  dds.reset();
  dds.set_freq_linear_sweep_params(AD9959::Channel0, params.mult,
                                   params.chirp_stop_hex,
                                   params.chirp_start_hex,
                                   params.chirp_freq_hex * 10, // up
                                   params.chirp_freq_hex,      // down
                                   params.chirp_time_hex,      // up
                                   params.chirp_time_hex       // down
                                   );

  dds.set_freq_linear_sweep_params(AD9959::Channel1, params.mult,
                                   params.chirp_start_hex,
                                   params.clk_transition_hex,
                                   params.mw_freq_hex * 10, // up
                                   params.mw_freq_hex,      // down
                                   params.mw_time_hex,      // up
                                   params.mw_time_hex       // down
                                   );

  dds.io_update();
}

} // namespace util
