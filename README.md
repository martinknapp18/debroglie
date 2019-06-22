# Embedded Control System for Mobile Atom Interferometers

Please refer to [this paper](https://arxiv.org/abs/1812.01028) for details about the project.

## Setup
To successfully build and use the code, install the correct build tools for your system from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm). The current gcc version used for this project is gcc-7-2018-q2-update

To modify the MAX11300 pins, use this [EV Kit Configuratione Software](https://www.maximintegrated.com/en/design/software-description.html/swpart=SFW0003260A).

Please refer to the hardware schematic for setting up the system using [Nucleo-F746ZG](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f746zg.html#design-scroll) development board, [MAX11300 evaluation kit](https://www.maximintegrated.com/en/products/analog/data-converters/analog-to-digital-converters/MAX11300PMB1.html), and [AD9959 Evaluatoin Board](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-AD9959.html)

## Build
Make a virtual environement and install the requirements.txt file
Source the environment before building as it is required to build the protobuf library.

## Code Structure
Comments next to directory name indicate at a high level the code inside each.
```
├── bsp # Board Support Package.
├── dds_params # Header files to configure DDS
├── desktop # Code to run from desktop client 
│   ├── auto_g # Scripts to automatically zoom and find gravity
│   ├── notebooks # Post processing including fitting fringes and finding gravity
│   └── scripts # Scripts to store the data and plot while running
├── drivers # For peripherals
│   ├── ad9959 # DDS driver
│   └── max11300 # ADC DAC driver
├── experiment # Header file with experiment settings and variables
├── hardware # Hardware schematics for replicating setup
├── mbed-os # MBED library.
├── nanopb # Library for sending data back and forth to computer.
└── util # Macros and other useful shorthands.
```


## Copyright
###UC Berkeley's Standard Copyright and Disclaimer Notice
Copyright ©2019. The Regents of the University of California (Regents). All Rights Reserved. Permission to use, copy, modify, and distribute this software and its documentation for educational, research, and not-for-profit purposes, without fee and without a signed licensing agreement, is hereby granted, provided that the above copyright notice, this paragraph and the following two paragraphs appear in all copies, modifications, and distributions. Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, otl@berkeley.edu, http://ipira.berkeley.edu/industry-info for commercial licensing opportunities.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
