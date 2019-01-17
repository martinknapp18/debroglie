# Embedded Control System for Mobile Atom Interferometers

Please refer to [this paper](https://arxiv.org/abs/1812.01028) for details about the project.

## Setup
To successfully build and use the code, install the correct build tools for your system from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm). The current gcc version used for this project is gcc-7-2018-q2-update

To modify the MAX11300 pins, use this [EV Kit Configuratione Software](https://www.maximintegrated.com/en/design/software-description.html/swpart=SFW0003260A).

Please refer to the hardware schematic for setting up the system using [Nucleo-F746ZG](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f746zg.html#design-scroll) development board, [MAX11300 evaluation kit](https://www.maximintegrated.com/en/products/analog/data-converters/analog-to-digital-converters/MAX11300PMB1.html), and [AD9959 Evaluatoin Board](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-AD9959.html)