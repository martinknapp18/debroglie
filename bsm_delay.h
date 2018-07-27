#ifndef _BSM_DELAY_H_
#define _BSM_DELAY_H_
// This is calibrated and optimized. Tested at 1us, 4us, 8us, 100us, and 1ms
#define bsm_delay_us(us)                                                        \
  do {                                                                         \
    asm volatile("MOV R0,%[loops]\n\t"                                         \
                 "1: \n\t"                                                     \
                 "SUB R0, #1\n\t"                                              \
                 "CMP R0, #0\n\t"                                              \
                 "BNE 1b \n\t"                                                 \
                 :                                                             \
                 : [loops] "r"(108 * us - 5)                                   \
                 : "memory");                                                  \
  } while (0)

#define bsm_delay_ms(ms) bsm_delay_us(ms * 1000)

#endif  // _BSM_DELAY_H_
