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

#ifndef _UTIL_DDS_H_
#define _UTIL_DDS_H_

#include "drivers/ad9959/ad9959.h"

namespace util {

class DDSConfig
{
  public:
    // TODO(get rid of this old struct)
    struct params_t
    {
        uint8_t mult;
        uint32_t clk_transition_hex;
        uint32_t chirp_start_hex;
        uint32_t chirp_stop_hex;
        uint32_t mw_time_hex;
        uint32_t mw_freq_hex;
        uint32_t chirp_time_hex;
        uint32_t chirp_freq_hex;
        double actual_chirp;
        double detuning;
    };
    
    void configure_up(drivers::ad9959::AD9959 dds, params_t params);
    void configure_down(drivers::ad9959::AD9959 dds, params_t params);

};

} // namespace util
#endif // _UTIL_DDS_H_
