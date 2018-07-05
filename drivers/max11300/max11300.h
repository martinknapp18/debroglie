/**********************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 **********************************************************************/

#ifndef _DRIVERS_MAX11300_MAX11300_H_
#define _DRIVERS_MAX11300_MAX11300_H_

#include "MAX11300Hex.h"
#include "mbed.h"

/**
 @brief MAX11300 - PIXI, 20-Port Programmable Mixed-Signal I/O with
 12-Bit ADC, 12-Bit DAC, Analog Switches, and GPIO

 The MAX11300 integrates a PIXI™, 12-bit, multichannel, analog-to-digital
 converter (ADC) and a 12-bit, multichannel, buffered digital-to-analog
 converter (DAC) in a single integrated circuit (IC). This device offers
 20 mixed-signal high-voltage, bipolar ports, which are configurable as an
 ADC analog input, a DAC analog output, a general-purpose input port (GPI),
 a general-purpose output port (GPO), or an analog switch terminal.
 One internal and two external temperature sensors track junction and
 environmental temperature, respectively. Adjacent pairs of ports are
 configurable as a logic-level translator for open-drain devices or an
 analog switch.

 Use configuration software found at
 https://www.maximintegrated.com/en/products/analog/data-converters/analog-to-digital-converters/MAX11300.html/tb_tab2
 to generate MAX11300hex.h file
 */

namespace drivers {
namespace max11300 {

class MAX11300 {
public:
  /// MAX11300 Ports
  enum MAX11300_Ports {
    PORT0,
    PORT1,
    PORT2,
    PORT3,
    PORT4,
    PORT5,
    PORT6,
    PORT7,
    PORT8,
    PORT9,
    PORT10,
    PORT11,
    PORT12,
    PORT13,
    PORT14,
    PORT15,
    PORT16,
    PORT17,
    PORT18,
    PORT19
  };

  /// MAX11300 Port Modes
  enum MAX11300_Port_Modes {
    /// HIGH_Z
    MODE_0,
    /// Digital input with programmable threshold, GPI
    MODE_1,
    /// Bidirectional level translator terminal
    MODE_2,
    /// Register-driven digital output with DAC-controlled level, GPO
    MODE_3,
    /// Unidirectional path output with DAC-controlled level, GPO
    MODE_4,
    /// Analog output for DAC
    MODE_5,
    /// Analog output for DAC with ADC monitoring
    MODE_6,
    /// Positive analog input to single-ended ADC
    MODE_7,
    /// Positive analog input to differential ADC
    MODE_8,
    /// Negative analog input to differential ADC
    MODE_9,
    /// Analog output for DAC and negative analog input to differential ADC
    MODE_10,
    /// Terminal to GPI-controlled analog switch
    MODE_11,
    /// Terminal to register-controlled analog switch
    MODE_12
  };

  enum CmdResult {
    /// Failed operation
    OpFailure,
    /// Successful operation
    Success
  };

  static const uint16_t MODE_BITMASK_PROCESS_1 = 0x047A;

  static const uint16_t MODE_BITMASK_PROCESS_2 = 0x0380;

  static const uint16_t MODE_BITMASK_PROCESS_3 = 0x1804;

  ///@brief MAX11300 Constructor
  ///@param[in] spi_bus - reference to SPI bus for this device
  ///@param[in] cs - pin to be used for chip select
  ///@param[in] interrupt - pin to be used as interrupt input, default = NC
  ///@param[in] cnvrt - pin to be used for convert, default = NC
  MAX11300(SPI &spi_bus, PinName cs, PinName interrupt = NC, PinName cnvt = NC);

  ///@brief MAX11300 Destructor
  ~MAX11300();

  void init(void);

  ///@brief Writes given register with data
  ///@param[in] reg - register to be written
  ///@param[in] data - data to write
  ///@return none
  void write_register(MAX11300RegAddress_t reg, uint16_t data);

  ///@brief Reads given register
  ///@param[in] reg - register to read
  ///@return contents of register
  uint16_t read_register(MAX11300RegAddress_t reg);

  ///@brief Writes a block of data starting at given register
  ///@param[in] reg - register to start writing at
  ///@param[in] data - pointer to data buffer
  ///@param[in] num_reg - number of registers to be written
  ///@return none
  void block_write(MAX11300RegAddress_t reg, uint16_t *data, uint8_t num_reg);

  ///@brief Reads a block of data starting at given register
  ///@param[in] reg - register to start reading at
  ///@param[in] data - pointer to data buffer
  ///@param[in] num_reg - number of registers to be read
  ///@return none
  void block_read(MAX11300RegAddress_t reg, uint16_t *data, uint8_t num_reg);

  ///@brief Writes gpo configured port with lsb of state
  ///@param[in] port - gpo congigured port to be written
  ///@param[in] state - lsb of state is written to port
  ///@return Result of operation
  CmdResult gpio_write(MAX11300_Ports port, uint8_t state);

  ///@brief Reads gpi configured port
  ///@param[in] port - gpi congigured port to be read
  ///@param[out] state - lsb of state matches port state
  ///@return Result of operation
  CmdResult gpio_read(MAX11300_Ports port, uint8_t &state);

  ///@brief Read single ended ADC configured port
  ///@param[in] port - single ended ADC configured port
  ///@param[out] data - contents of ADC data register
  ///@return Result of operation
  CmdResult single_ended_adc_read(MAX11300_Ports port, uint16_t &data);

  ///@brief Write single ended DAC configured port
  ///@param[in] port - single ended DAC configured port
  ///@param[in] data - value to be written to DAC data register
  ///@return Result of operation
  CmdResult single_ended_dac_write(MAX11300_Ports port, uint16_t data);

private:
  SPI &m_spi_bus;
  DigitalOut m_cs;
  InterruptIn m_int;
  DigitalOut m_cnvt;

  void config_process_1(uint16_t &device_control_local);
  void config_process_2(uint16_t &device_control_local);
  void config_process_3(void);
};

} // namespace drivers
} // namespace max11300

#endif /* _DRIVERS_MAX11300_MAX11300_H_ */
