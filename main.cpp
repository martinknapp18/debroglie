#include "TimerOc.h"
#include "mbed.h"
#include "minig.h"

DigitalOut blue{LED2};
Ticker led_ticker;

void toggle_led() {
  static int i = 0;
  blue = i & 1;
  i++;
}

uint32_t ticks[4] = {4, 8, 4, 4};

// MiniG minig;
int main() {
  led_ticker.attach(toggle_led, .2);
  printf("\nhello debugging!\n");

  // minig.init();

  TimerOc timer_oc{PB_0_ALT0};
  timer_oc.start(100000 /* us*/, 3, ticks);
  wait_ms(210);
  timer_oc.stop();

  while (1) {
    // minig.run();
  }
}
