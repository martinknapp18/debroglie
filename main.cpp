#include "bsm_delay.h"
#include "drivers/max11300/max11300.h"
#include "mbed.h"
#include "minig.h"

#define RUN_MINIG 1

int main() {
  printf("\nhello debugging!\n");

#if RUN_MINIG
  MiniG minig;
  minig.init();
  while (1) {
    minig.run();
  }
#endif

  using drivers::max11300::MAX11300;
  SPI pixi_spi_{SPI_MOSI, SPI_MISO, SPI_SCK};
  MAX11300 pixi_{pixi_spi_, SPI_CS};
  pixi_.init();

  MAX11300::Ramp ramps[3] = {
      {MAX11300::PORT10, 0, 0xF},
      {MAX11300::PORT11, 0, 0xFF},
      {MAX11300::PORT12, 0, 0xFFF},
  };

  MAX11300::RampAction ramp_action{
      .num_ramps = 3, .num_steps = 100, .step_time_us = 150};

  pixi_.prepare_ramps(&ramp_action, ramps);
  DigitalOut pe_15{PE_15, 1};
  while (1) {

    wait(1);
    pe_15 = 0;
    wait(1);
    pe_15 = 1;
    // pixi_.run_ramps(&ramp_action);
  }

  DigitalOut pg_5{PG_5, 0};
  DigitalOut pg_6{PG_6, 1};

  while (1) {
    bsm_delay_us(4);
    GPIOG->ODR ^= GPIO_PIN_5 | GPIO_PIN_6;
  }
}
