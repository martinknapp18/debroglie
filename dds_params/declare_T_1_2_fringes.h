constexpr DDSConfig::params_t inter_dds_params_0 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x1862e4eb,
    .mw_time_hex = 0x31,
    .mw_freq_hex = 0x5c1,
    .chirp_time_hex = 0x31,
    .chirp_freq_hex = 0x1cf,
    .actual_chirp = 22.000120,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_1 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x1873ef3d,
    .mw_time_hex = 0x2b,
    .mw_freq_hex = 0x50c,
    .chirp_time_hex = 0x2b,
    .chirp_freq_hex = 0x19a,
    .actual_chirp = 22.200131,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_2 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x1884fbde,
    .mw_time_hex = 0xf0,
    .mw_freq_hex = 0x1c2f,
    .chirp_time_hex = 0xf0,
    .chirp_freq_hex = 0x905,
    .actual_chirp = 22.400248,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_3 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x189607ae,
    .mw_time_hex = 0x85,
    .mw_freq_hex = 0xf9e,
    .chirp_time_hex = 0x85,
    .chirp_freq_hex = 0x50b,
    .actual_chirp = 22.600328,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_4 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x18a7129a,
    .mw_time_hex = 0xf6,
    .mw_freq_hex = 0x1ce3,
    .chirp_time_hex = 0xf6,
    .chirp_freq_hex = 0x969,
    .actual_chirp = 22.800367,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_5 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x18b81e3a,
    .mw_time_hex = 0xad,
    .mw_freq_hex = 0x1451,
    .chirp_time_hex = 0xad,
    .chirp_freq_hex = 0x6ad,
    .actual_chirp = 23.000438,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_6 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x18c929ab,
    .mw_time_hex = 0x8d,
    .mw_freq_hex = 0x108f,
    .chirp_time_hex = 0x8d,
    .chirp_freq_hex = 0x57d,
    .actual_chirp = 23.200500,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_7 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x18da34c8,
    .mw_time_hex = 0xda,
    .mw_freq_hex = 0x199a,
    .chirp_time_hex = 0xda,
    .chirp_freq_hex = 0x88f,
    .actual_chirp = 23.400548,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_8 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x18eb3f24,
    .mw_time_hex = 0x16,
    .mw_freq_hex = 0x295,
    .chirp_time_hex = 0x16,
    .chirp_freq_hex = 0xdf,
    .actual_chirp = 23.600561,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_9 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x18fc5dfd,
    .mw_time_hex = 0xf7,
    .mw_freq_hex = 0x1d01,
    .chirp_time_hex = 0xf7,
    .chirp_freq_hex = 0x9dd,
    .actual_chirp = 23.801513,
    .detuning = 700.000000};

constexpr DDSConfig::params_t inter_dds_params_10 = {
    .mult = 20,
    .clk_transition_hex = 0x102b2dab,
    .chirp_start_hex = 0x11108dec,
    .chirp_stop_hex = 0x190d6811,
    .mw_time_hex = 0xaf,
    .mw_freq_hex = 0x148d,
    .chirp_time_hex = 0xaf,
    .chirp_freq_hex = 0x70c,
    .actual_chirp = 24.001513,
    .detuning = 700.000000};

constexpr size_t NUM_POINTS_INTER = 11;
constexpr DDSConfig::params_t fringes[NUM_POINTS_INTER] = {
    inter_dds_params_0, inter_dds_params_1,  inter_dds_params_2,
    inter_dds_params_3, inter_dds_params_4,  inter_dds_params_5,
    inter_dds_params_6, inter_dds_params_7,  inter_dds_params_8,
    inter_dds_params_9, inter_dds_params_10,
};
