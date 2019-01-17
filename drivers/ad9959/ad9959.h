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

/*
 * Simple (and incomplete) driver for Analog Devices AD9959 DDS chip
 *
 * This library assumes that you are pulling down the power down pin externally.
 * If you are not doing so, nothing will work because the chip won't be turned out.
 * Refer to the hardware schematic for more details.
 *
 * So far linear sweep is only supported for up to two channels via the Profile pins.
 */

#ifndef _DRIVERS_AD9959_AD9959_H_
#define _DRIVERS_AD9959_AD9959_H_

#include "mbed.h"
#include <SPI.h>

namespace drivers {
namespace ad9959 {

class AD9959 {

public:
  enum Channel {
    ChannelNone = 0x0,
    Channel0 = 1 << 0,
    Channel1 = 1 << 1,
    Channel2 = 1 << 2,
    Channel3 = 1 << 3,
    ChannelAll = Channel0 | Channel1 | Channel2 | Channel3,
  };

  struct Pins {
    PinName ChipSelect;
    PinName ResetPin;
    PinName UpdatePin;
    PinName Profile0Pin;
    PinName Profile1Pin;
  };

  AD9959(SPI &spi_bus, const Pins &pins, uint32_t ref_freq, uint8_t mult);

  void init();
  void reset();
  void io_update();
  void set_freq(Channel ch, double freq_hz);
  void set_phase(Channel ch, double phase_deg);

  void set_freq_linear_sweep_params(Channel ch, double start_freq_hz,
                                    double end_freq_hz, size_t steps_up,
                                    size_t steps_down, double step_time_up_s,
                                    double step_time_down_s);
  inline void set_freq_linear_sweep_params(Channel ch, double start_freq_hz,
                                           double end_freq_hz, size_t steps,
                                           double step_time_s) {
    set_freq_linear_sweep_params(ch, start_freq_hz, end_freq_hz, steps, steps,
                                 step_time_s, step_time_s);
  }

  void
  set_freq_linear_sweep_params(AD9959::Channel ch, uint8_t mult,
                               uint32_t start_freq_word, uint32_t end_freq_word,
                               uint32_t step_word_up, uint32_t step_word_down,
                               uint8_t time_word_up, uint8_t time_word_down);

  inline void set_freq_linear_sweep_params(AD9959::Channel ch, uint8_t mult,
                                           uint32_t start_freq_word,
                                           uint32_t end_freq_word,
                                           uint32_t step_word,
                                           uint8_t time_word) {
    set_freq_linear_sweep_params(ch, mult, start_freq_word, end_freq_word,
                                 step_word, step_word, time_word, time_word);
  }

  void start_linear_sweep_up(Channel ch);
  void start_linear_sweep_down(Channel ch);

private:
  uint32_t ref_freq_;
  uint8_t mult_;

  SPI &spi_bus_;
  DigitalOut cs_;
  DigitalOut reset_;
  DigitalOut update_;
  DigitalOut p0_;
  DigitalOut p1_;

  // DO NOT CHANGE ORDERING OR VALUE. USED AS REGISTER ADDRESS.
  // Lengths found in register_width
  enum Register {
    CSR = 0x00, // 1 byte, Channel Select Register
    FR1 = 0x01, // 3 bytes, Function Register 1
    FR2 = 0x02, // 2 bytes, Function Register 2

    // The following registers are duplicated for each channel.
    // A write goes to any and all registers enabled in channel select (CSR)
    // To read successfully you must first select one channel
    CFR = 0x03,  // 3 bytes, Channel Function Register (one for each channel!)
    CFTW = 0x04, // 4 bytes, Channel Frequency Tuning Word
    CPOW = 0x05, // 2 bytes, Channel Phase Offset Word (aligned to LSB, top 2
                 // bits unused)
    ACR = 0x06, // 3 bytes, Amplitude Control Register (rate byte, control byte,
                // scale byte)
    LSRR = 0x07, // 2 bytes, Linear Sweep Rate Register (falling, rising)
    RDW = 0x08,  // 4 bytes, Rising Delta Word
    FDW = 0x09,  // 4 bytes, Falling Delta Word

    // The following registers (per channel) are used to provide 16 modulation
    // values
    // This library doesn't provide modulation. Only CW1 is used, for sweep
    // destination.
    CW1 = 0x0A, // 4 bytes, Channel Word 1-15 (phase & amplitude MSB aligned)
    CW2 = 0x0B,
    CW3 = 0x0C,
    CW4 = 0x0D,
    CW5 = 0x0E,
    CW6 = 0x0F,
    CW7 = 0x10,
    CW8 = 0x11,
    CW9 = 0x12,
    CW10 = 0x13,
    CW11 = 0x14,
    CW12 = 0x15,
    CW13 = 0x16,
    CW14 = 0x17,
    CW15 = 0x18,
    NUM_REGISTERS
  };

  static constexpr int register_width[AD9959::NUM_REGISTERS]{
      1, // CSR,
      3, // FR1
      2, // FR2
      3, // CFR
      4, // CFTW
      2, // CPOW
      3, // ACR
      2, // LSRR
      4, // RDW
      4, // FDW
      4, // CW1
      4, // CW2
      4, // CW3
      4, // CW4
      4, // CW5
      4, // CW6
      4, // CW7
      4, // CW8
      4, // CW9
      4, // CW10
      4, // CW11
      4, // CW12
      4, // CW13
      4, // CW14
      4, // CW15
  };

  enum SerialMode {
    // Serial I/O Modes (default IO2Wire):
    IO2Wire = 0x0,
    IO3Wire = 0x2,
    IO2Bit = 0x4,
    IO4Bit = 0x6,
  };

  enum LsbMode {
    // Bit order selection (default MSB):
    MSB_First = 0,
    LSB_First = 1,
  };

  enum ChargePumpControl {
    // Higher charge pump values decrease lock time and increase phase noise
    uA75 = 0x00,
    uA100 = 0x01,
    uA125 = 0x02,
    uA150 = 0x03,
  };

  enum ModLevel {
    // Middle byte:
    two = 0x00,
    four = 0x01,
    eight = 0x02,
    sixteen = 0x03,
  };

  void write_register(Register reg, uint32_t value);
  void set_csr(Channel ch);
  void set_fr1(ModLevel modlevel, uint8_t mult);

  uint64_t sys_clk() { return ref_freq_ * mult_; }
  uint64_t sync_clk() { return sys_clk() / 4; }
  double freq_res() {
    return static_cast<double>(sys_clk() / static_cast<double>(1LL << 32));
  }

  double phase_res() {
    return static_cast<double>(360 / static_cast<double>(1LL << 14));
  }

  uint32_t freqToHEX(double freq_hz) {
    return static_cast<uint32_t>((freq_hz / freq_res()) + 0.5);
  }

  uint16_t phaseToHex(double phase_deg) {
    return static_cast<uint16_t>(phase_deg / phase_res() + 0.5) % 16384;
  }

  uint8_t timeToHex(double time_s) {
    return static_cast<uint8_t>((time_s * sync_clk()));
  }

  // NOTE: the order of the registers defined below is in reverse order from
  // the datasheet to run properly on a little endian system. There are no
  // checks
  // nor compatible implementation for big endian systems at tihs time.
  // TODO: add edianess check and big endian support.
  typedef union {
    struct {
      uint8_t lsb_first : 1;          // use LsbMode enum
      uint8_t serial_mode_select : 2; // Use SerialMode enum
      uint8_t must_be_zero : 1;       // must_be_zero
      uint8_t channel : 4;
    };
    uint8_t bits;
  } csr_t;

  typedef union {
    struct {
      uint8_t manual_sw_sync : 1; // 0: inactive (default), 0: active
      uint8_t manual_hw_sync : 1; // 0: inactive (default), 0: active
      uint8_t open_sync : 2;      // See Synchronizing Multiple AD9959 Decives
                                  // section for details.
      uint8_t dac_ref_power_down : 1;    // 0 enabled (default), 1:power down
      uint8_t sync_clk_disable : 1;      // 0: active (default), 1: disable
      uint8_t external_pwr_dwn_mode : 1; // See Data Sheet. Keep 0 by default.
      uint8_t ref_clk_input : 1;         // 0 enabled, 1 disabled
      uint8_t modulation_level : 2;      // use ModLevel enum
      uint8_t ru_rd : 2;
      uint8_t ppc : 3;
      uint8_t : 1;                     // Do not use
      uint8_t charge_pump_control : 2; // Use ChragePumpControl enum
      uint8_t pll_divider_ratio : 5;   // value between 4 and 20 to enable PLL.
                                       // Otherwise, disable PLL
      uint8_t vco_gain : 1;            // 0 < 160 MHz, 1 > 255MHz
    };
    uint32_t bits;
  } fr1_t;

  typedef union {
    struct {
      uint8_t system_clock_offest : 2;
      uint8_t : 2;                            // Open/Unused
      uint8_t multipledevice_sync_master : 1; // See Synchronizing Multiple
                                              // AD9959 Devices
      uint8_t multidevice_sync_status : 1; // See Synchronizing Multiple AD9959
                                           // Devices
      uint8_t multidevice_sync_master_enable : 1; // See Synchronizing Multiple
                                                  // AD9959 Devices
      uint8_t auto_sync_enable : 1; // See Synchronizing Multiple AD9959 Devices
      uint8_t : 4;
      uint8_t all_channels_clear_phase_accumulator : 1;
      uint8_t all_channels_autoclear_phase_accumulator : 1;
      uint8_t all_channels_clear_sweep_accumulator : 1;
      uint8_t all_channels_autoclear_sweep_accumulator : 1;
    };
    uint16_t bits;
  } fr2_t;

  typedef union {
    struct {
      uint8_t sine_wave_output_enable : 1; // 0: Cosine (default), 1: Sine
      uint8_t clear_phase_accumulator : 1; // 0: normal operation (default), 1:
                                           // asynchronous clear
      uint8_t autoclear_phase_accumulator : 1; // 0: no change on io_update
                                               // (default), 1:cleaned and
                                               // synced on io_update
      uint8_t clear_sweep_accumulator : 1; // 0: normal operation (default), 1:
                                           // asynchronous clear
      uint8_t autoclear_sweep_accumulator : 1; // 0: no change on io_update
                                               // (default), 1:cleaned and
                                               // synced on io_update
      uint8_t matched_pipe_delays_active : 1;  // 0: inactive, 1: active
      uint8_t dac_power_down : 1;          // 0: enabled (default), 1: disabled
      uint8_t digital_core_power_down : 1; // 0: enabled (default), 1: disabled.
      uint8_t dac_full_scale_current_control : 2; // 0b11: the DAC is at the
      // largest LSB value (default)
      // See Table 5 for other settings.
      uint8_t must_be_zero : 1;          // Keep at at 0
      uint8_t : 2;                       // Open, do not use.
      uint8_t load_srr_at_io_update : 1; // 0: loaded only on timeout(default),
                                         // 1: loaded on timeout and ioupdate.
      uint8_t linear_sweep_enable : 1;   // 0: disable(defualt), 1: enaled
      uint8_t linear_sweep_no_dwell : 1; // 0 no-dwell inactive (default), 1:
                                         // no-dwell active.
      uint8_t : 6;                       // Open, do not use.
      uint8_t amplitude_frequency_select : 2;
    };
    uint32_t bits;
  } cfr_t;

  typedef union {
    struct {
      uint32_t freq_tunning_word;
    };
    uint32_t bits;
  } cftw_t;

  // TODO(bsm): Handle alignment for phase and amplitude into CW1
  typedef union {
    struct {
      uint16_t phase_offset_word : 14;
      uint16_t : 2; // Do not use
    };
    uint16_t bits;
  } cpow_t;

  typedef union {
    struct {
      uint16_t amplitude_scale_factor : 9;
      uint16_t load_arr_at_io_update : 1; // 0: loaded only on timeout(default),
                                          // 1: loaded on timeout and ioupdate.
      uint16_t ramp_up_down_enable : 1;   // only used when
                                          // amplitude_multiplier_enable is 1
      // 0: enabled manuel ru_rd operation (default), 1:auto ru_rd operation
      uint16_t
          amplitude_multiplier_enable : 1; // 0: disabled(default), 1: enabled
      uint16_t : 1;                        // Open, do not use.
      uint16_t increment_decrement_step_size : 2;
      uint8_t amplitude_ramp_rate;
    };
    uint32_t bits;
  } acr_t;

  typedef union {
    struct {
      uint8_t rising_sweep_ramp_rate;
      uint8_t falling_sweep_ramp_rate;
    };
    uint16_t bits;
  } lsrr_t;

  typedef union {
    struct {
      uint32_t rising_delta_word;
    };
    uint32_t bits;
  } rdw_t;
  typedef union {
    struct {
      uint32_t falling_delta_word;
    };
    uint32_t bits;
  } fdw_t;
};

} // namespace ad9959
} // namespace drivers

#endif /* _DRIVERS_AD9959_AD9959_H_ */
