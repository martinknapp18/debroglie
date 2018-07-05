#include "mbed.h"
#include "drivers/ad9959/ad9959.h"
#include "drivers/max11300/max11300.h"

#include "utils/analog_operations.h"

using drivers::ad9959::AD9959;
using drivers::max11300::MAX11300;

using utils::AnalogOperations;

DigitalOut red { LED1 };
DigitalOut blue { LED2 };
DigitalOut green { LED3 };

Ticker led_ticker;

SPI dds_spi { PB_5, PB_4, PB_3 };
constexpr AD9959::Pins dds_pins = {
  PA_4, /* CS */
  D6, /* reset */
  D5, /* update */
  D4, /* p0 */
  D3 /* p1 */
};

AD9959 dds{dds_spi, dds_pins, 24000000 /* ref_freq */, 10 /* mult */};

SPI pixi_spi { SPI_MOSI, SPI_MISO, SPI_SCK};
MAX11300 pixi { pixi_spi, SPI_CS /* cs */, NC, D8 };

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

  // Note: Do not reset and initializing, or you'll waste time debugging for no reason.
  dds.init();
  dds.set_freq(AD9959::Channel1, 11000000.0);
  dds.set_freq_linear_sweep_params(AD9959::Channel0, 12000000.0, 13000000.0,
      2 << 19, 2 << 19, .0001, .0001);
  dds.io_update();

  pixi.init();
  AnalogOperations::set_voltage(pixi, MAX11300::PORT16, 5.0);
  AnalogOperations::ramp_voltage(pixi, MAX11300::PORT16, 5.0, 7.5, 1000, 9000000);
  wait ( 1 ) ;
  AnalogOperations::ramp_voltage(pixi, MAX11300::PORT16, 7.5, 5.0, 1000, 9000000);


  while(1) {
    dds.start_linear_sweep_up(AD9959::Channel0);
    wait(5);
    dds.start_linear_sweep_down(AD9959::Channel0);
    wait(5);
  }

}
