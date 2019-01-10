constexpr MiniG::dds_params_t inter_dds_params_0 = {
    .mult = 10,
    .clk_transition_hex = 0x20565b56,
    .chirp_start_hex = 0x22211bd9,
    .chirp_stop_hex = 0x316f10b5,
    .mw_time_hex = 0xbd,
    .mw_freq_hex = 0x58c8,
    .chirp_time_hex = 0xbd,
    .chirp_freq_hex = 0x1d2a,
    .actual_chirp = 22.993566,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_1 = {
    .mult = 13,
    .clk_transition_hex = 0x18dff77d,
    .chirp_start_hex = 0x1a40da58,
    .chirp_stop_hex = 0x2606aacd,
    .mw_time_hex = 0xc3,
    .mw_freq_hex = 0x3634,
    .chirp_time_hex = 0xc3,
    .chirp_freq_hex = 0x11ce,
    .actual_chirp = 22.993578,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_2 = {
    .mult = 17,
    .clk_transition_hex = 0x13059f23,
    .chirp_start_hex = 0x141379cb,
    .chirp_stop_hex = 0x1d142892,
    .mw_time_hex = 0xa0,
    .mw_freq_hex = 0x1a01,
    .chirp_time_hex = 0xa0,
    .chirp_freq_hex = 0x88b,
    .actual_chirp = 22.993590,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_3 = {
    .mult = 13,
    .clk_transition_hex = 0x18dff77d,
    .chirp_start_hex = 0x1a40da58,
    .chirp_stop_hex = 0x2606aba3,
    .mw_time_hex = 0xcb,
    .mw_freq_hex = 0x386d,
    .chirp_time_hex = 0xcb,
    .chirp_freq_hex = 0x1289,
    .actual_chirp = 22.993603,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_4 = {
    .mult = 19,
    .clk_transition_hex = 0x110507a6,
    .chirp_start_hex = 0x11f67a72,
    .chirp_stop_hex = 0x1a04909f,
    .mw_time_hex = 0xd1,
    .mw_freq_hex = 0x1b32,
    .chirp_time_hex = 0xd1,
    .chirp_freq_hex = 0x8ef,
    .actual_chirp = 22.993614,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_5 = {
    .mult = 13,
    .clk_transition_hex = 0x18dff77d,
    .chirp_start_hex = 0x1a40da58,
    .chirp_stop_hex = 0x2606ac68,
    .mw_time_hex = 0xd3,
    .mw_freq_hex = 0x3aa6,
    .chirp_time_hex = 0xd3,
    .chirp_freq_hex = 0x1344,
    .actual_chirp = 22.993626,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_6 = {
    .mult = 16,
    .clk_transition_hex = 0x1435f916,
    .chirp_start_hex = 0x1554b167,
    .chirp_stop_hex = 0x1ee56c67,
    .mw_time_hex = 0xc3,
    .mw_freq_hex = 0x23c8,
    .chirp_time_hex = 0xc3,
    .chirp_freq_hex = 0xbc1,
    .actual_chirp = 22.993638,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_7 = {
    .mult = 13,
    .clk_transition_hex = 0x18dff77d,
    .chirp_start_hex = 0x1a40da58,
    .chirp_stop_hex = 0x2606ad1f,
    .mw_time_hex = 0xdb,
    .mw_freq_hex = 0x3cdf,
    .chirp_time_hex = 0xdb,
    .chirp_freq_hex = 0x13ff,
    .actual_chirp = 22.993647,
    .detuning = 700.000000};

constexpr MiniG::dds_params_t inter_dds_params_8 = {
    .mult = 17,
    .clk_transition_hex = 0x13059f23,
    .chirp_start_hex = 0x141379cb,
    .chirp_stop_hex = 0x1d142a4a,
    .mw_time_hex = 0x9d,
    .mw_freq_hex = 0x1985,
    .chirp_time_hex = 0x9d,
    .chirp_freq_hex = 0x862,
    .actual_chirp = 22.993657,
    .detuning = 700.000000};

constexpr size_t NUM_POINTS_INTER = 9;
constexpr MiniG::dds_params_t fringes[NUM_POINTS_INTER] = {
    inter_dds_params_0, inter_dds_params_1, inter_dds_params_2,
    inter_dds_params_3, inter_dds_params_4, inter_dds_params_5,
    inter_dds_params_6, inter_dds_params_7, inter_dds_params_8,
};
