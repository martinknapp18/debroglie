#include "bsm_delay.h"
#include "drivers/max11300/max11300.h"
#include "mbed.h"
#include "minig.h"

#define RUN_MINIG 1

int main() {
  printf("\nhello debugging!\n");
  enableCycleCounter();

  MiniG minig;
  minig.init();
/// Init ends with reset so I can do whatever I want out here.

#if RUN_MINIG
  while (1) {
    minig.run();
  }
#endif
  while(1);
}
