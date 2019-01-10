#include "bsm_delay.h"
#include "drivers/max11300/max11300.h"
#include "mbed.h"
#include "minig.h"

#define WRITE_IO(PORT, ON_BITS, OFF_BITS)                                      \
  PORT->ODR = ((PORT->ODR & ~(OFF_BITS)) | (ON_BITS))
#define BITS_NONE (0)

#define RUN_MINIG 1

// volatile uint8_t triggered = 0;
// void trigger() {
//   triggered++;
// }
//
// uint16_t samples[1024];

int main() {
  printf("\nhello debugging!\n");
  enableCycleCounter();

  MiniG minig;
  minig.init();
  while (1) {
    minig.run();
  }

  // For recording timing parameters
  //
  // while (1) {

  //   WRITE_IO(GPIOG, GPIO_PIN_2, BITS_NONE);
  //   bsm_delay_us(4);
  //   WRITE_IO(GPIOG, BITS_NONE, GPIO_PIN_2);
  //   bsm_delay_us(4);
  // }

  // InterruptIn pg0{PG_0};
  // pg0.rise(&trigger);

  // while (1) {
  //   if (triggered) {
  //     minig.pixi_.max_speed_adc_read(drivers::max11300::MAX11300::PORT0,
  //                                    samples, 3*127);
  //     printf("ListPlot[{");
  //     size_t i = 0;
  //     for (; i < 127; i++) {
  //       printf("%d,", samples[i]);
  //     }
  //     for (; i < 127 + 127; i++) {
  //       printf("%d,", samples[i]);
  //     }
  //     for (; i < 127 + 127 + 127; i++) {
  //       printf("%d,", samples[i]);
  //     }
  //     printf("}]\n\n");
  //     triggered = 0;
  //   }
  // }
}
