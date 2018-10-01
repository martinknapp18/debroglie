constexpr MiniG::dds_params_t inter_dds_params_0 = {
    .mult = 13,
    .clk_transition_hex = 0xc6ffbbe,
    .chirp_start_hex = 0xd206d2c,
    .chirp_stop_hex = 0x1303577d,
    .mw_time_hex = 0xf3,
    .mw_freq_hex = 0x10e2,
    .chirp_time_hex = 0xf3,
    .chirp_freq_hex = 0x58c,
    .actual_chirp = 22.993703,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_1 = {
    .mult = 16,
    .clk_transition_hex = 0xa1afc8b,
    .chirp_start_hex = 0xaaa58b3,
    .chirp_stop_hex = 0xf72b770,
    .mw_time_hex = 0xfd,
    .mw_freq_hex = 0xb9b,
    .chirp_time_hex = 0xfd,
    .chirp_freq_hex = 0x3d0,
    .actual_chirp = 22.993729,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_2 = {
    .mult = 15,
    .clk_transition_hex = 0xac773c7,
    .chirp_start_hex = 0xb605e9d,
    .chirp_stop_hex = 0x107a5dda,
    .mw_time_hex = 0xa7,
    .mw_freq_hex = 0x8b7,
    .chirp_time_hex = 0xa7,
    .chirp_freq_hex = 0x2dd,
    .actual_chirp = 22.993769,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_3 = {
    .mult = 17,
    .clk_transition_hex = 0x982cf91,
    .chirp_start_hex = 0xa09bce5,
    .chirp_stop_hex = 0xe8a16f7,
    .mw_time_hex = 0x97,
    .mw_freq_hex = 0x622,
    .chirp_time_hex = 0x97,
    .chirp_freq_hex = 0x204,
    .actual_chirp = 22.993799,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_4 = {
    .mult = 18,
    .clk_transition_hex = 0x8fb8b26,
    .chirp_start_hex = 0x97af983,
    .chirp_stop_hex = 0xdbb4ecb,
    .mw_time_hex = 0x68,
    .mw_freq_hex = 0x3c5,
    .chirp_time_hex = 0x68,
    .chirp_freq_hex = 0x13d,
    .actual_chirp = 22.993817,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_5 = {
    .mult = 14,
    .clk_transition_hex = 0xb8c8e55,
    .chirp_start_hex = 0xc30655f,
    .chirp_stop_hex = 0x11a7af9b,
    .mw_time_hex = 0xb5,
    .mw_freq_hex = 0xad8,
    .chirp_time_hex = 0xb5,
    .chirp_freq_hex = 0x390,
    .actual_chirp = 22.993891,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_6 = {
    .mult = 19,
    .clk_transition_hex = 0x88283d3,
    .chirp_start_hex = 0x8fb3d39,
    .chirp_stop_hex = 0xd024bde,
    .mw_time_hex = 0xe3,
    .mw_freq_hex = 0x762,
    .chirp_time_hex = 0xe3,
    .chirp_freq_hex = 0x26d,
    .actual_chirp = 22.993924,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_7 = {
    .mult = 15,
    .clk_transition_hex = 0xac773c7,
    .chirp_start_hex = 0xb605e9d,
    .chirp_stop_hex = 0x107a60e9,
    .mw_time_hex = 0x95,
    .mw_freq_hex = 0x7c6,
    .chirp_time_hex = 0x95,
    .chirp_freq_hex = 0x28e,
    .actual_chirp = 22.993979,
    .detuning = 700.000000};

constexpr size_t NUM_POINTS_INTER = 8;
constexpr MiniG::dds_params_t fringes[NUM_POINTS_INTER] = {
    inter_dds_params_0, inter_dds_params_1, inter_dds_params_2,
    inter_dds_params_3, inter_dds_params_4, inter_dds_params_5,
    inter_dds_params_6, inter_dds_params_7,
};
