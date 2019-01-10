constexpr MiniG::dds_params_t inter_dds_params_0 = {
    .mult = 10,
    .clk_transition_hex = 0x20565b56,
    .chirp_start_hex = 0x222e374a,
    .chirp_stop_hex = 0x317c296c,
    .mw_time_hex = 0xc5,
    .mw_freq_hex = 0x5f2f,
    .chirp_time_hex = 0xc5,
    .chirp_freq_hex = 0x1e66,
    .actual_chirp = 22.993503,
    .detuning = 720.000000};

constexpr MiniG::dds_params_t inter_dds_params_1 = {
    .mult = 10,
    .clk_transition_hex = 0x20565b56,
    .chirp_start_hex = 0x222e374a,
    .chirp_stop_hex = 0x317c2a15,
    .mw_time_hex = 0xc3,
    .mw_freq_hex = 0x5e38,
    .chirp_time_hex = 0xc3,
    .chirp_freq_hex = 0x1e17,
    .actual_chirp = 22.993519,
    .detuning = 720.000000};

constexpr MiniG::dds_params_t inter_dds_params_2 = {
    .mult = 11,
    .clk_transition_hex = 0x1d65c765,
    .chirp_start_hex = 0x1f12bde7,
    .chirp_stop_hex = 0x2cfc83e1,
    .mw_time_hex = 0xc9,
    .mw_freq_hex = 0x5043,
    .chirp_time_hex = 0xc9,
    .chirp_freq_hex = 0x19a2,
    .actual_chirp = 22.993532,
    .detuning = 720.000000};

constexpr MiniG::dds_params_t inter_dds_params_3 = {
    .mult = 11,
    .clk_transition_hex = 0x1d65c765,
    .chirp_start_hex = 0x1f12bde7,
    .chirp_stop_hex = 0x2cfc8484,
    .mw_time_hex = 0xda,
    .mw_freq_hex = 0x570d,
    .chirp_time_hex = 0xda,
    .chirp_freq_hex = 0x1bcd,
    .actual_chirp = 22.993548,
    .detuning = 720.000000};

constexpr MiniG::dds_params_t inter_dds_params_4 = {
    .mult = 11,
    .clk_transition_hex = 0x1d65c765,
    .chirp_start_hex = 0x1f12bde7,
    .chirp_stop_hex = 0x2cfc850f,
    .mw_time_hex = 0xeb,
    .mw_freq_hex = 0x5dd7,
    .chirp_time_hex = 0xeb,
    .chirp_freq_hex = 0x1df8,
    .actual_chirp = 22.993562,
    .detuning = 720.000000};

constexpr MiniG::dds_params_t inter_dds_params_5 = {
    .mult = 11,
    .clk_transition_hex = 0x1d65c765,
    .chirp_start_hex = 0x1f12bde7,
    .chirp_stop_hex = 0x2cfc8588,
    .mw_time_hex = 0xfc,
    .mw_freq_hex = 0x64a1,
    .chirp_time_hex = 0xfc,
    .chirp_freq_hex = 0x2023,
    .actual_chirp = 22.993574,
    .detuning = 720.000000};

constexpr MiniG::dds_params_t inter_dds_params_6 = {
    .mult = 17,
    .clk_transition_hex = 0x13059f23,
    .chirp_start_hex = 0x141b2f95,
    .chirp_stop_hex = 0x1d1bde5c,
    .mw_time_hex = 0xa0,
    .mw_freq_hex = 0x1ac0,
    .chirp_time_hex = 0xa0,
    .chirp_freq_hex = 0x88b,
    .actual_chirp = 22.993590,
    .detuning = 720.000000};

constexpr size_t NUM_POINTS_INTER = 7;
constexpr MiniG::dds_params_t fringes[NUM_POINTS_INTER] = {
    inter_dds_params_0, inter_dds_params_1, inter_dds_params_2,
    inter_dds_params_3, inter_dds_params_4, inter_dds_params_5,
    inter_dds_params_6,
};
