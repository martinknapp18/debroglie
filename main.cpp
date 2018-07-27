#include "mbed.h"
#include "minig.h"
#include "bsm_delay.h"

#define RUN_MINIG 0

int main() {
  printf("\nhello debugging!\n");

#if RUN_MINIG
  MiniG minig;
  minig.init();
  while (1) {
    minig.run();
  }
#endif

  DigitalOut pg_5{PG_5, 0};
  DigitalOut pg_6{PG_6, 1};

  while (1) {
    bsm_delay_us(4);
    GPIOG->ODR ^= GPIO_PIN_5 | GPIO_PIN_6;
  }
}
