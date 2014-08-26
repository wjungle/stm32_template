PROJECT=temp
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
ARCH=CM3

LIB_STM = ./libstm
FRTOS = ./FreeRTOS

TOOLCHAIN_PATH ?= /usr/local/csl/arm-2012.03/arm-none-eabi
C_LIB= $(TOOLCHAIN_PATH)/lib/thumb2

CFLAGS=-g -O2 -mlittle-endian -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib
CFLAGS+=-DUSE_STDPERIPH_DRIVER #-D__TARGET_FPU_VFP
#CFLAGS+=-D __FPU_PRESENT=1 -D ARM_MATH_CM4
CFLAGS+=-DUSE_USB_OTG_FS=1

# to run from FLASH
CFLAGS+=-Wl,-T,$(LIB_STM)/stm32_flash.ld

CFLAGS+=-I./

# FreeRTOS lib
#CFLAGS+=-I$(FRTOS)/Source/include
#CFLAGS+=-I$(FRTOS)/Demo/CORTEX_M4F_STM32F407ZG-SK
#CFLAGS+=-I$(FRTOS)/Source/portable/GCC/ARM_CM4F

# stm32f4_discovery lib
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc/device_support
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc/core_support

# stm32f4 USB lib
CFLAGS+=-I$(LIB_STM)/STM32_USB_Device_Library/Class/cdc/inc
CFLAGS+=-I$(LIB_STM)/STM32_USB_Device_Library/Core/inc
CFLAGS+=-I$(LIB_STM)/usb_lib
CFLAGS+=-I$(LIB_STM)/STM32_USB_OTG_Driver/inc

# STM32F4xx_StdPeriph_Driver\inc
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc

# Utilities
CFLAGS+=-I./libstm/Utilities/STM32F4-Discovery

####################### INC ########################

# Source Files
SRC += main.c system_stm32f4xx.c stm32f4xx_it.c usart3.c delay.c

# STARTUP
SRC+=$(LIB_STM)/startup_stm32f4xx.s

# Utilities
SRC+=$(LIB_STM)/Utilities/STM32F4-Discovery/stm32f4_discovery.c  \
#	$(LIB_STM)/Utilities/STM32F4-Discovery/stm32f4_discovery_lis302dl.c

# stm32f4 USB src
SRC+=$(LIB_STM)/usb_lib/usb_bsp.c
SRC+=$(LIB_STM)/usb_lib/usbd_desc.c
SRC+=$(LIB_STM)/usb_lib/usbd_usr.c
SRC+=$(LIB_STM)/usb_lib/usb_core.c
SRC+=$(LIB_STM)/usb_lib/usbd_cdc_vcp.c
SRC+=$(LIB_STM)/STM32_USB_OTG_Driver/src/usb_dcd_int.c
SRC+=$(LIB_STM)/STM32_USB_OTG_Driver/src/usb_dcd.c
SRC+=$(LIB_STM)/STM32_USB_Device_Library/Core/src/usbd_core.c
SRC+=$(LIB_STM)/STM32_USB_Device_Library/Core/src/usbd_req.c
SRC+=$(LIB_STM)/STM32_USB_Device_Library/Core/src/usbd_ioreq.c
SRC+=$(LIB_STM)/STM32_USB_Device_Library/Class/cdc/src/usbd_cdc_core.c

# FreeROTS source
#SRC+=./FreeRTOS/Source/croutine.c
#SRC+=./FreeRTOS/Source/timers.c
#SRC+=./FreeRTOS/Source/event_groups.c
#SRC+=./FreeRTOS/Source/list.c
#SRC+=./FreeRTOS/Source/queue.c
#SRC+=./FreeRTOS/Source/tasks.c
#SRC+=./FreeRTOS/Source/portable/MemMang/heap_1.c
#SRC+=./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c

all: $(BIN_IMAGE)

libstm_build:
	$(MAKE) -C libstm/STM32F4xx_StdPeriph_Driver/build

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

$(EXECUTABLE): main.c $(SRC)
	$(CC) $(CFLAGS) $^ -o $@  -L./libstm/STM32F4xx_StdPeriph_Driver/build \
		-lSTM32F4xx_StdPeriph_Driver -L$(C_LIB) -lc -lm -lg -lgcc

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

flash:
	st-flash write $(BIN_IMAGE) 0x8000000

.PHONY: all clean
