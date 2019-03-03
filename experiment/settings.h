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

// Analog values
constexpr float AO1_MOT = 6.6;
constexpr float AO2_MOT = 1.76;
constexpr float AO3_MOT = 5;
constexpr float EO_MOT = 8.93;
constexpr float NS_MOT = 0.7;
constexpr float WE_MOT = 1.2;
constexpr float BIAS_MOT = 0.11;

constexpr float AO1_PGC = AO1_MOT;
constexpr float AO2_PGC = 2.20;
constexpr float AO3_PGC = 1.95;
constexpr float EO_PGC = 9.78;

constexpr float AO1_MW = 7.8;
constexpr float AO2_MW = 0;
constexpr float AO3_MW = 10;
constexpr float NS_MW = NS_MOT;
constexpr float WE_MW = 2.2;
constexpr float EO_MW = 9.21;
constexpr float BIAS_MW = BIAS_MOT;

constexpr float AO1_RAMAN = 8.1;
constexpr float AO2_RAMAN = 2.5;
constexpr float NS_RAMAN = 0;
constexpr float WE_RAMAN = 0;
constexpr float EO_RAMAN = 6.5;
// constexpr float BIAS_RAMAN = 3;
constexpr float BIAS_RAMAN = 6;

constexpr float AO1_IMAGE = 7.30;
constexpr float AO2_IMAGE = 1.75;
constexpr float NS_IMAGE = 0.04;
constexpr float WE_IMAGE = 0.25;
constexpr float EO_IMAGE = 8.88;

#define K_UP 1

// DDS Paramaeters
// TODO(split out for different experiment and maybe pass in as object)

// #include "dds_params/declare_T_80_fringes.h"
// #include "dds_params/declare_T_120_fringes.h"
// #include "dds_params/declare_T_120_16_fringes.h"
// #include "dds_params/declare_T_115_fringes_17e_7.h"
#if K_UP
#include "dds_params/T_130_fringes.h"
#include "dds_params/declare_spectroscopy.h"
#else
#include "dds_params/k_down.h"
#include "dds_params/k_down_spec.h"
#endif
