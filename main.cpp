#include "mbed.h"

DigitalOut red { LED1 };
DigitalOut blue { LED2 };
DigitalOut green { LED3 };

Ticker led_ticker;

void toggle_led() {
	static int i = 0;

	red = i & 1;
	blue = i & 2;
	green = i & 4;
	i++;
}

int main() {
	led_ticker.attach(toggle_led, 0.2);
	printf("\nhello debugging! \n");

  while(1) {
    wait(1);
  }

}
