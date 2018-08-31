#ifndef _BSM_DELAY_H_
#define _BSM_DELAY_H_

#include <stdint.h>
#include <system_stm32f7xx.h>
#include "mbed.h"
#include <core_cm7.h>
#include <math.h>


inline void enableCycleCounter() {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  DWT->LAR = 0xC5ACCE55;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

inline void delayCycles(uint32_t const numCycles) {
  uint32_t const startCycles = DWT->CYCCNT;
  while ((DWT->CYCCNT - startCycles) < numCycles) {
  }
}

inline uint32_t usToCycles(uint32_t const us) {
  // return ceil(us * ((float)SystemCoreClock / 1e6f)) - 56;
  return ceil(us * ((float)SystemCoreClock / 1e6f)) - 16;
}

inline void bsm_delay_us(uint32_t const us) { delayCycles(usToCycles(us)); }
inline void bsm_delay_ms(uint32_t const ms) { delayCycles(usToCycles(ms * 1000)); }

#endif // _BSM_DELAY_H_
