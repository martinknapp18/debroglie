# This file was automagically generated by mbed.org. For more information, 
# see http://mbed.org/handbook/Exporting-to-GCC-ARM-Embedded

###############################################################################
# Boiler-plate

# cross-platform directory manipulation
ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

OBJDIR := BUILD
# Move to the build directory
ifeq (,$(filter $(OBJDIR),$(notdir $(CURDIR))))
.SUFFIXES:
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKETARGET = '$(MAKE)' --no-print-directory -C $(OBJDIR) -f '$(mkfile_path)' \
		'SRCDIR=$(CURDIR)' $(MAKECMDGOALS)
.PHONY: $(OBJDIR) clean
all:
	+@$(call MAKEDIR,$(OBJDIR))
	+@$(MAKETARGET)
$(OBJDIR): all
Makefile : ;
% :: $(OBJDIR) ; :
clean :
	$(call RM,$(OBJDIR))

else

# trick rules into thinking we are in the root, when we are in the bulid dir
VPATH = ..

# Boiler-plate
###############################################################################
# Project settings

PROJECT := DeBroglie
MBED := mbed-os
MBED_STM := $(MBED)/targets/TARGET_STM
MBED_STM32F7 := $(MBED_STM)/TARGET_STM32F7
MBED_STM32F746xG := $(MBED_STM32F7)/TARGET_STM32F746xG
MBED_NUCLEO := $(MBED_STM32F746xG)/TARGET_NUCLEO_F746ZG

# Project settings
###############################################################################
# Objects and Paths

OBJECTS += main.o

SYS_OBJECTS += $(MBED)/drivers/AnalogIn.o
SYS_OBJECTS += $(MBED)/drivers/BusIn.o
SYS_OBJECTS += $(MBED)/drivers/BusInOut.o
SYS_OBJECTS += $(MBED)/drivers/BusOut.o
SYS_OBJECTS += $(MBED)/drivers/CAN.o
SYS_OBJECTS += $(MBED)/drivers/Ethernet.o
SYS_OBJECTS += $(MBED)/drivers/FlashIAP.o
SYS_OBJECTS += $(MBED)/drivers/I2C.o
SYS_OBJECTS += $(MBED)/drivers/I2CSlave.o 
SYS_OBJECTS += $(MBED)/drivers/InterruptIn.o
SYS_OBJECTS += $(MBED)/drivers/InterruptManager.o 
SYS_OBJECTS += $(MBED)/drivers/RawSerial.o
SYS_OBJECTS += $(MBED)/drivers/SPI.o
SYS_OBJECTS += $(MBED)/drivers/SPISlave.o
SYS_OBJECTS += $(MBED)/drivers/Serial.o
SYS_OBJECTS += $(MBED)/drivers/SerialBase.o
SYS_OBJECTS += $(MBED)/drivers/Ticker.o 
SYS_OBJECTS += $(MBED)/drivers/Timeout.o
SYS_OBJECTS += $(MBED)/drivers/Timer.o
SYS_OBJECTS += $(MBED)/drivers/TimerEvent.o
SYS_OBJECTS += $(MBED)/drivers/UARTSerial.o 

SYS_OBJECTS += $(MBED)/hal/mbed_flash_api.o
SYS_OBJECTS += $(MBED)/hal/mbed_gpio.o
SYS_OBJECTS += $(MBED)/hal/mbed_lp_ticker_api.o
SYS_OBJECTS += $(MBED)/hal/mbed_pinmap_common.o
SYS_OBJECTS += $(MBED)/hal/mbed_sleep_manager.o
SYS_OBJECTS += $(MBED)/hal/mbed_ticker_api.o
SYS_OBJECTS += $(MBED)/hal/mbed_us_ticker_api.o 
SYS_OBJECTS += $(MBED)/hal/mbed_critical_section_api.o

SYS_OBJECTS += $(MBED)/platform/ATCmdParser.o
SYS_OBJECTS += $(MBED)/platform/CallChain.o
SYS_OBJECTS += $(MBED)/platform/FileBase.o
SYS_OBJECTS += $(MBED)/platform/FileHandle.o
SYS_OBJECTS += $(MBED)/platform/FilePath.o
SYS_OBJECTS += $(MBED)/platform/FileSystemHandle.o
SYS_OBJECTS += $(MBED)/platform/LocalFileSystem.o
SYS_OBJECTS += $(MBED)/platform/Stream.o     
SYS_OBJECTS += $(MBED)/platform/mbed_alloc_wrappers.o
SYS_OBJECTS += $(MBED)/platform/mbed_application.o
SYS_OBJECTS += $(MBED)/platform/mbed_assert.o
SYS_OBJECTS += $(MBED)/platform/mbed_board.o
SYS_OBJECTS += $(MBED)/platform/mbed_board.o
SYS_OBJECTS += $(MBED)/platform/mbed_critical.o
SYS_OBJECTS += $(MBED)/platform/mbed_error.o
SYS_OBJECTS += $(MBED)/platform/mbed_interface.o
SYS_OBJECTS += $(MBED)/platform/mbed_mem_trace.o
SYS_OBJECTS += $(MBED)/platform/mbed_mktime.o
SYS_OBJECTS += $(MBED)/platform/mbed_poll.o
SYS_OBJECTS += $(MBED)/platform/mbed_retarget.o
SYS_OBJECTS += $(MBED)/platform/mbed_retarget.o
SYS_OBJECTS += $(MBED)/platform/mbed_rtc_time.o
SYS_OBJECTS += $(MBED)/platform/mbed_sdk_boot.o
SYS_OBJECTS += $(MBED)/platform/mbed_sdk_boot.o
SYS_OBJECTS += $(MBED)/platform/mbed_semihost_api.o
SYS_OBJECTS += $(MBED)/platform/mbed_stats.o
SYS_OBJECTS += $(MBED)/platform/mbed_wait_api_no_rtos.o
SYS_OBJECTS += $(MBED)/platform/mbed_wait_api_rtos.o

SYS_OBJECTS += $(MBED_STM)/analogin_api.o
SYS_OBJECTS += $(MBED_STM)/analogout_api.o
SYS_OBJECTS += $(MBED_STM)/can_api.o
SYS_OBJECTS += $(MBED_STM)/gpio_api.o
SYS_OBJECTS += $(MBED_STM)/gpio_irq_api.o
SYS_OBJECTS += $(MBED_STM)/hal_tick_16b.o
SYS_OBJECTS += $(MBED_STM)/hal_tick_32b.o
SYS_OBJECTS += $(MBED_STM)/i2c_api.o
SYS_OBJECTS += $(MBED_STM)/lp_ticker.o
SYS_OBJECTS += $(MBED_STM)/mbed_overrides.o
SYS_OBJECTS += $(MBED_STM)/pinmap.o
SYS_OBJECTS += $(MBED_STM)/port_api.o
SYS_OBJECTS += $(MBED_STM)/pwmout_api.o
SYS_OBJECTS += $(MBED_STM)/rtc_api.o
SYS_OBJECTS += $(MBED_STM)/serial_api.o
SYS_OBJECTS += $(MBED_STM)/sleep.o
SYS_OBJECTS += $(MBED_STM)/stm_spi_api.o
SYS_OBJECTS += $(MBED_STM)/trng_api.o
SYS_OBJECTS += $(MBED_STM)/us_ticker.o

SYS_OBJECTS += $(MBED_STM32F7)/analogin_device.o
SYS_OBJECTS += $(MBED_STM32F7)/analogout_device.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_adc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_adc_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_can.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_cec.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_cortex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_crc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_crc_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_cryp.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_cryp_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dac.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dac_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dcmi.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dcmi_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dfsdm.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dma.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dma2d.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dma_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_dsi.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_eth.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_flash.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_flash_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_gpio.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_hash.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_hash_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_hcd.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_i2c.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_i2c_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_i2s.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_irda.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_iwdg.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_jpeg.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_lptim.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_ltdc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_ltdc_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_mdios.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_mmc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_nand.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_nor.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_pcd.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_pcd_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_pwr.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_pwr_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_qspi.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_rcc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_rcc_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_rng.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_rtc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_rtc_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_sai.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_sai_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_sd.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_sdram.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_smartcard.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_smartcard_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_smbus.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_spdifrx.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_spi.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_spi_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_sram.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_tim.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_tim_ex.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_uart.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_usart.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_hal_wwdg.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_adc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_crc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_dac.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_dma.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_dma2d.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_exti.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_fmc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_gpio.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_i2c.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_pwr.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_rcc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_rng.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_rtc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_sdmmc.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_spi.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_tim.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_usart.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_usb.o
SYS_OBJECTS += $(MBED_STM32F7)/device/stm32f7xx_ll_utils.o
SYS_OBJECTS += $(MBED_STM32F7)/device/system_stm32f7xx.o
SYS_OBJECTS += $(MBED_STM32F7)/flash_api.o
SYS_OBJECTS += $(MBED_STM32F7)/gpio_irq_device.o
SYS_OBJECTS += $(MBED_STM32F7)/pwmout_device.o
SYS_OBJECTS += $(MBED_STM32F7)/serial_device.o
SYS_OBJECTS += $(MBED_STM32F7)/spi_api.o

SYS_OBJECTS += $(MBED_STM32F746xG)/device/TOOLCHAIN_GCC_ARM/startup_stm32f746xx.o

SYS_OBJECTS += $(MBED_NUCLEO)/PeripheralPins.o
SYS_OBJECTS += $(MBED_NUCLEO)/system_clock.o

INCLUDE_PATHS += -I../
INCLUDE_PATHS += -I../.
INCLUDE_PATHS += -I../maxim/.
INCLUDE_PATHS += -I../debroglie/.
INCLUDE_PATHS += -I../analog_devices/.
INCLUDE_PATHS += -I../$(MBED)
INCLUDE_PATHS += -I../$(MBED)/cmsis/TARGET_CORTEX_M/.
INCLUDE_PATHS += -I../$(MBED_STM)
INCLUDE_PATHS += -I../$(MBED_STM32F7)
INCLUDE_PATHS += -I../$(MBED_STM32F7)/device/.
INCLUDE_PATHS += -I../$(MBED_STM32F746xG)/.
INCLUDE_PATHS += -I../$(MBED_STM32F746xG)/device/.
INCLUDE_PATHS += -I../$(MBED_STM32F746xG)/device/TOOLCHAIN_GCC_ARM/.
INCLUDE_PATHS += -I../$(MBED_STM32F746xG)/TARGET_NUCLEO_F746ZG/.

INCLUDE_PATHS += -I../$(MBED)/drivers
INCLUDE_PATHS += -I../$(MBED)/hal
INCLUDE_PATHS += -I../$(MBED)/platform

LIBRARY_PATHS := -L../$(MBED_STM32F746xG)/device/TOOLCHAIN_GCC_ARM
# LIBRARIES := -l$(MBED) 
LINKER_SCRIPT ?= ../$(MBED_STM32F746xG)/device/TOOLCHAIN_GCC_ARM/STM32F746xG.ld

# Objects and Paths
###############################################################################
# Tools and Flags

AS      = 'arm-none-eabi-gcc' '-x' 'assembler-with-cpp' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'
CC      = 'arm-none-eabi-gcc' '-std=gnu99' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'
CPP     = 'arm-none-eabi-g++' '-std=gnu++11' '-fno-rtti' '-Wvla' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'
LD      = 'arm-none-eabi-gcc'
ELF2BIN = 'arm-none-eabi-objcopy'
PREPROC = 'arm-none-eabi-cpp' '-E' '-P' '-Wl,--gc-sections' '-Wl,--wrap,main' '-Wl,--wrap,_malloc_r' '-Wl,--wrap,_free_r' '-Wl,--wrap,_realloc_r' '-Wl,--wrap,_memalign_r' '-Wl,--wrap,_calloc_r' '-Wl,--wrap,exit' '-Wl,--wrap,atexit' '-Wl,-n' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-sp-d16' '-mfloat-abi=softfp'


C_FLAGS += -std=gnu99
C_FLAGS += -DDEVICE_SPI=1
C_FLAGS += -DTARGET_STM32F746xG
C_FLAGS += -DFEATURE_LWIP=1
C_FLAGS += -D__MBED__=1
C_FLAGS += -DDEVICE_I2CSLAVE=1
C_FLAGS += -D__FPU_PRESENT=1
C_FLAGS += -DDEVICE_PORTOUT=1
C_FLAGS += -DUSBHOST_OTHER
C_FLAGS += -DDEVICE_PORTINOUT=1
C_FLAGS += -D__MBED_CMSIS_RTOS_CM
C_FLAGS += -DDEVICE_LOWPOWERTIMER=1
C_FLAGS += -DTARGET_STM32F7
C_FLAGS += -DTOOLCHAIN_object
C_FLAGS += -DDEVICE_SERIAL_ASYNCH=1
C_FLAGS += -D__CMSIS_RTOS
C_FLAGS += -DDEVICE_ANALOGOUT=1
C_FLAGS += -DTARGET_STM32F746ZG
C_FLAGS += -DTOOLCHAIN_GCC
C_FLAGS += -DDEVICE_I2C_ASYNCH=1
C_FLAGS += -DARM_MATH_CM7
C_FLAGS += -DTARGET_CORTEX_M
C_FLAGS += -DTARGET_LIKE_CORTEX_M7
C_FLAGS += -DDEVICE_RTC=1
C_FLAGS += -DDEVICE_TRNG=1
C_FLAGS += -DTARGET_UVISOR_UNSUPPORTED
C_FLAGS += -DTARGET_M7
C_FLAGS += -DDEVICE_SPI_ASYNCH=1
C_FLAGS += -DDEVICE_PWMOUT=1
C_FLAGS += -DMBED_BUILD_TIMESTAMP=1515974215.22
C_FLAGS += -DTARGET_CORTEX
C_FLAGS += -DDEVICE_I2C=1
C_FLAGS += -DTRANSACTION_QUEUE_SIZE_SPI=2
C_FLAGS += -DDEVICE_STDIO_MESSAGES=1
C_FLAGS += -D__CORTEX_M7
C_FLAGS += -DTARGET_STM32F746
C_FLAGS += -DTARGET_FAMILY_STM32
C_FLAGS += -DTARGET_FF_ARDUINO
C_FLAGS += -DDEVICE_PORTIN=1
C_FLAGS += -DTARGET_RELEASE
C_FLAGS += -DTARGET_STM
C_FLAGS += -DTARGET_NUCLEO_F746ZG
C_FLAGS += -DTARGET_LIKE_MBED
C_FLAGS += -DTARGET_RTOS_M4_M7
C_FLAGS += -DDEVICE_SLEEP=1
C_FLAGS += -DTOOLCHAIN_GCC_ARM
C_FLAGS += -DDEVICE_CAN=1
C_FLAGS += -DDEVICE_INTERRUPTIN=1
C_FLAGS += -DDEVICE_SPISLAVE=1
C_FLAGS += -DDEVICE_ANALOGIN=1
C_FLAGS += -DDEVICE_SERIAL=1
C_FLAGS += -DDEVICE_FLASH=1
C_FLAGS += -include
C_FLAGS += mbed_config.h
C_FLAGS += -DDEBUG

CXX_FLAGS += -std=gnu++11
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -Wvla
CXX_FLAGS += -DDEVICE_SPI=1
CXX_FLAGS += -DTARGET_STM32F746xG
CXX_FLAGS += -DFEATURE_LWIP=1
CXX_FLAGS += -D__MBED__=1
CXX_FLAGS += -DDEVICE_I2CSLAVE=1
CXX_FLAGS += -D__FPU_PRESENT=1
CXX_FLAGS += -DDEVICE_PORTOUT=1
CXX_FLAGS += -DUSBHOST_OTHER
CXX_FLAGS += -DDEVICE_PORTINOUT=1
CXX_FLAGS += -D__MBED_CMSIS_RTOS_CM
CXX_FLAGS += -DDEVICE_LOWPOWERTIMER=1
CXX_FLAGS += -DTARGET_STM32F7
CXX_FLAGS += -DTOOLCHAIN_object
CXX_FLAGS += -DDEVICE_SERIAL_ASYNCH=1
CXX_FLAGS += -D__CMSIS_RTOS
CXX_FLAGS += -DDEVICE_ANALOGOUT=1
CXX_FLAGS += -DTARGET_STM32F746ZG
CXX_FLAGS += -DTOOLCHAIN_GCC
CXX_FLAGS += -DDEVICE_I2C_ASYNCH=1
CXX_FLAGS += -DARM_MATH_CM7
CXX_FLAGS += -DTARGET_CORTEX_M
CXX_FLAGS += -DTARGET_LIKE_CORTEX_M7
CXX_FLAGS += -DDEVICE_RTC=1
CXX_FLAGS += -DDEVICE_TRNG=1
CXX_FLAGS += -DTARGET_UVISOR_UNSUPPORTED
CXX_FLAGS += -DTARGET_M7
CXX_FLAGS += -DDEVICE_SPI_ASYNCH=1
CXX_FLAGS += -DDEVICE_PWMOUT=1
CXX_FLAGS += -DMBED_BUILD_TIMESTAMP=1515974215.22
CXX_FLAGS += -DTARGET_CORTEX
CXX_FLAGS += -DDEVICE_I2C=1
CXX_FLAGS += -DTRANSACTION_QUEUE_SIZE_SPI=2
CXX_FLAGS += -DDEVICE_STDIO_MESSAGES=1
CXX_FLAGS += -D__CORTEX_M7
CXX_FLAGS += -DTARGET_STM32F746
CXX_FLAGS += -DTARGET_FAMILY_STM32
CXX_FLAGS += -DTARGET_FF_ARDUINO
CXX_FLAGS += -DDEVICE_PORTIN=1
CXX_FLAGS += -DTARGET_RELEASE
CXX_FLAGS += -DTARGET_STM
CXX_FLAGS += -DTARGET_NUCLEO_F746ZG
CXX_FLAGS += -DTARGET_LIKE_MBED
CXX_FLAGS += -DTARGET_RTOS_M4_M7
CXX_FLAGS += -DDEVICE_SLEEP=1
CXX_FLAGS += -DTOOLCHAIN_GCC_ARM
CXX_FLAGS += -DDEVICE_CAN=1
CXX_FLAGS += -DDEVICE_INTERRUPTIN=1
CXX_FLAGS += -DDEVICE_SPISLAVE=1
CXX_FLAGS += -DDEVICE_ANALOGIN=1
CXX_FLAGS += -DDEVICE_SERIAL=1
CXX_FLAGS += -DDEVICE_FLASH=1
CXX_FLAGS += -include
CXX_FLAGS += mbed_config.h
CXX_FLAGS += -DDEBUG

ASM_FLAGS += -x
ASM_FLAGS += assembler-with-cpp
ASM_FLAGS += -DTRANSACTION_QUEUE_SIZE_SPI=2
ASM_FLAGS += -D__CORTEX_M7
ASM_FLAGS += -DARM_MATH_CM7
ASM_FLAGS += -D__FPU_PRESENT=1
ASM_FLAGS += -DUSBHOST_OTHER
ASM_FLAGS += -D__MBED_CMSIS_RTOS_CM
ASM_FLAGS += -D__CMSIS_RTOS
ASM_FLAGS += -I.
ASM_FLAGS += -I$(MBED)/.
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TARGET_STM
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TARGET_STM/TARGET_STM32F7
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TARGET_STM/TARGET_STM32F7/TARGET_STM32F746xG
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TARGET_STM/TARGET_STM32F7/TARGET_STM32F746xG/TARGET_NUCLEO_F746ZG
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TARGET_STM/TARGET_STM32F7/TARGET_STM32F746xG/device
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TARGET_STM/TARGET_STM32F7/device
ASM_FLAGS += -I$(MBED)/TARGET_NUCLEO_F746ZG/TOOLCHAIN_GCC_ARM
ASM_FLAGS += -I$(MBED)/drivers
ASM_FLAGS += -I$(MBED)/hal
ASM_FLAGS += -I$(MBED)/platform


LD_FLAGS :=-Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp 
LD_SYS_LIBS :=-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -Wl,--end-group

# Tools and Flags
###############################################################################
# Rules

.PHONY: all lst size


all: $(PROJECT).bin $(PROJECT).hex size


.s.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
  
	@$(AS) -c $(ASM_FLAGS) -o $@ $<
  


.S.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
  
	@$(AS) -c $(ASM_FLAGS) -o $@ $<
  

.c.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).link_script.ld: $(LINKER_SCRIPT)
	@$(PREPROC) $< -o $@



$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS) $(PROJECT).link_script.ld 
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(filter-out %.o, $^) $(LIBRARY_PATHS) --output $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)



$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@
	+@echo "===== bin file ready to flash: $(OBJDIR)/$@ =====" 

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@


# Rules
###############################################################################
# Dependencies

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)
endif

# Dependencies
###############################################################################
