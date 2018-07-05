#include "ad9959.h"

namespace {
constexpr uint32_t SpiRate = 2000000; // 2Mb/s
void lower(DigitalOut pin) {
  pin = 0;
}

void raise(DigitalOut pin) {
  pin = 1;
}

void pulse(DigitalOut pin) {
  raise(pin);
  wait_us(1);
  lower(pin);
}

}  // namespace

namespace drivers {
namespace ad9959 {
constexpr int AD9959::register_width[AD9959::NUM_REGISTERS];

AD9959::AD9959(SPI & spi_bus, const Pins & pins, uint32_t ref_freq, uint8_t mult) :
  sys_clk_{ref_freq * mult},
  mult_{mult},
  sync_clk_{sys_clk_ / 4},
  spi_bus_{spi_bus},
  cs_{pins.ChipSelect, 1},
  reset_{pins.ResetPin, 0},
  update_{pins.UpdatePin, 0},
  p0_{pins.Profile0Pin, 0},
  p1_{pins.Profile1Pin, 0} {}

void AD9959::init() {
  spi_bus_.frequency(SpiRate);
  pulse(reset_);
  pulse(update_);

  set_csr(AD9959::ChannelAll);
  set_fr1(AD9959::ModLevel::two);
}

void AD9959::reset() {
  pulse(reset_);
}

void AD9959::io_update() {
  pulse(update_);
}

void AD9959::set_freq(AD9959::Channel ch, double freq_hz) {
  set_csr(ch);

  AD9959::cftw_t val;
  val.freq_tunning_word = freqToHEX(freq_hz);
  write_register(AD9959::CFTW, val.bits);
}

void AD9959::set_phase(AD9959::Channel ch, double phase_deg) {
  set_csr(ch);
  AD9959::cpow_t val;
  val.phase_offset_word = phaseToHex(phase_deg);
  write_register(AD9959::CPOW, val.bits);
}

void AD9959::set_freq_linear_sweep_params(AD9959::Channel ch,
    double start_freq_hz, double end_freq_hz, size_t steps_up,
    size_t steps_down, double step_time_up_s, double step_time_down_s) {
  MBED_ASSERT((ch & Channel0) | (ch & Channel1));
  set_csr(ch);

  set_fr1(AD9959::ModLevel::two);

  AD9959::cfr_t cfr_params { 0 };
  cfr_params.amplitude_frequency_select = 0b10; // Frequency sweep; TODO(bsm): make enum
  cfr_params.linear_sweep_enable = 1;
  cfr_params.linear_sweep_no_dwell = 0;
  cfr_params.load_srr_at_io_update = 1;
  cfr_params.dac_full_scale_current_control = 0b11; // TODO(bsm): make enum
  write_register(AD9959::CFR, cfr_params.bits);

  AD9959::cftw_t val;
  val.freq_tunning_word = freqToHEX(start_freq_hz);
  write_register(AD9959::CFTW, val.bits);

  val.freq_tunning_word = freqToHEX(end_freq_hz);
  write_register(AD9959::CW1, val.bits);

  AD9959::rdw_t rising_val;
  rising_val.rising_delta_word = freqToHEX(
      (end_freq_hz - start_freq_hz) / steps_up);
  write_register(AD9959::RDW, rising_val.bits);

  AD9959::fdw_t falling_val;
  falling_val.falling_delta_word = freqToHEX(
      (end_freq_hz - start_freq_hz) / steps_down);
  write_register(AD9959::FDW, falling_val.bits);

  AD9959::lsrr_t lsrr_val;
  lsrr_val.rising_sweep_ramp_rate = timeToHex(step_time_up_s);
  lsrr_val.falling_sweep_ramp_rate = timeToHex(step_time_down_s);
  write_register(AD9959::LSRR, lsrr_val.bits);
}

void AD9959::start_linear_sweep_up(Channel ch) {
  if (ch & Channel0) {
    p0_ = 1;
  }
  if (ch & Channel1) {
    p1_ = 1;
  }
}

void AD9959::start_linear_sweep_down(Channel ch) {
  if (ch & Channel0) {
    p0_ = 0;
  }
  if (ch & Channel1) {
    p1_ = 0;
  }
}


void AD9959::write_register(AD9959::Register reg, uint32_t value) {
  lower(cs_);
  spi_bus_.write(reg);
  int len = register_width[reg];
  while (len-- > 0) {
    uint8_t byte = (value >> (len * 8)) & 0xFF;
    spi_bus_.write(byte);
  }
  raise(cs_);
}

void AD9959::set_csr(AD9959::Channel ch) {
  AD9959::csr_t csr_val { 0 };

  csr_val.channel = ch;
  csr_val.serial_mode_select = AD9959::IO3Wire;
  csr_val.lsb_first = AD9959::MSB_First;

  write_register(AD9959::CSR, csr_val.bits);
}

void AD9959::set_fr1(AD9959::ModLevel modlevel) {
  AD9959::fr1_t fr1_val = { 0 };

  fr1_val.pll_divider_ratio = mult_;
  fr1_val.charge_pump_control = AD9959::uA75;

  // Looks like its used for rising and falling rates. Don't turn off til I test.
  fr1_val.sync_clk_disable = 0;
  fr1_val.modulation_level = modlevel;
  fr1_val.ru_rd = 0b00;

  write_register(AD9959::FR1, fr1_val.bits);
}

}  // namespace ad9959
}  // namespace drivers
