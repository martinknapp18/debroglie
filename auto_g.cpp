#include "auto_g.h"

#include "cycle_count_delay.h"
#include "util/macros.h"
#include "util/dds_config.h"

#include "BUILD/gravity.pb.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

namespace {
using util::DDSConfig;

pb_byte_t read_buffer[minig_RunExperimentShot_size];
pb_byte_t send_buffer[minig_Result_size];

minig_RunExperimentShot exp_params;
minig_Result send_back;

DDSConfig::params_t curr_dds_params;

void fill_dds_params() {
  curr_dds_params.mult = exp_params.mult;
  curr_dds_params.clk_transition_hex = exp_params.clk_transition_hex;
  curr_dds_params.chirp_start_hex = exp_params.chirp_start_hex;
  curr_dds_params.chirp_stop_hex = exp_params.chirp_stop_hex;
  curr_dds_params.mw_time_hex = exp_params.mw_time_hex;
  curr_dds_params.mw_freq_hex = exp_params.mw_freq_hex;
  curr_dds_params.chirp_time_hex = exp_params.chirp_time_hex;
  curr_dds_params.chirp_freq_hex = exp_params.chirp_freq_hex;
  curr_dds_params.actual_chirp = exp_params.actual_chirp;
  curr_dds_params.detuning = exp_params.detuning;
}

DDSConfig dds_config;
} // namespace

AutoG::AutoG() : MiniG(), pc_{USBTX, USBRX} {}

void AutoG::run() {
  while (1) {
    receive_shot_params();
    fill_dds_params();
    // Now exp_params should be ready
    // TODO(replace fringes[0] with received fringe)
    if (k_up_) {
      dds_config.configure_up(dds_, curr_dds_params);
    } else {
      dds_config.configure_down(dds_, curr_dds_params);
    }
    reset(0.0);
    mot();
    pgc();
    mw(300);
    interferometry(exp_params.T_ms);
    image();
    integrate();
    send_result();
  }
}

void AutoG::receive_shot_params() {
  size_t bytes_read = 0;
  while (1) {
    if (pc_.readable()) {

      // Create a stream that reads from the buffer.
      char c = pc_.getc();
      read_buffer[bytes_read] = c;
      bytes_read++;

      // Try decoding every time
      pb_istream_t stream = pb_istream_from_buffer(
          static_cast<pb_byte_t *>(read_buffer), bytes_read);

      // Now we are ready to decode the message.
      bool status =
          pb_decode(&stream, minig_RunExperimentShot_fields, &exp_params);

      // Check for errors...
      if (status) {
        // Decode succeeded. Message is now ready for use.
        if (exp_params.detuning < 0) {
          k_up_ = false;
        } else {
          k_up_ = true;
        }
        return;
      }
      // Take another character and try again.
    }
  }
}

void AutoG::send_result() {
  send_back.pd_fraction = pd_fraction_;
  send_back.atom_number = atom_number_;

  /* Create a stream that will write to our buffer. */
  pb_ostream_t stream =
      pb_ostream_from_buffer(send_buffer, sizeof(send_buffer));

  /* Now we are ready to encode the message! */
  bool status = pb_encode(&stream, minig_Result_fields, &send_back);

  /* Then just check for any errors.. */
  if (!status) {
    pc_.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    return;
  }

  for (size_t i = 0; i < sizeof(send_buffer); i++) {
    pc_.putc(send_buffer[i]);
  }
}

void AutoG::interferometry(uint32_t T) {
  WRITE_IO(GPIOE, BITS_NONE, ao_3_ | cooling_shutter_ | raman_eo_);
  pixi_.run_ramps(&raman_on_ramp_);
  // last 5 ms
  cycle_delay_ms(2);
  WRITE_IO(GPIOE, BITS_NONE, dds_switch_);

  // Freefall
  if (k_up_) {
    WRITE_IO(GPIOE, inter_dds_profile_pin_, BITS_NONE);
  } else {
    WRITE_IO(GPIOE, BITS_NONE, inter_dds_profile_pin_);
  }
  cycle_delay_ms(75);

  WRITE_IO(GPIOG, scope_, BITS_NONE);
  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(5);
  WRITE_IO(GPIOE, BITS_NONE, ao_2_ | ao_3_);

  cycle_delay_ms(T);

  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(10);
  WRITE_IO(GPIOE, BITS_NONE, ao_2_ | ao_3_);

  cycle_delay_ms(T);

  WRITE_IO(GPIOE, ao_2_ | ao_3_, BITS_NONE);
  cycle_delay_us(5);

  WRITE_IO(GPIOE, BITS_NONE, ao_2_ | ao_3_);

  WRITE_IO(GPIOG, BITS_NONE, scope_);
  cycle_delay_ms(45 + 2*(70 -T));
  cycle_delay_us(500);

  // Stop sweeping
}
