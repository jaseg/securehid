# put your *.o targets here, make should handle the rest!
CUBE_PATH 		?= $(wildcard ~)/resource/STM32CubeF4
CMSIS_PATH 		?= $(CUBE_PATH)/Drivers/CMSIS
CMSIS_DEV_PATH  ?= $(CMSIS_PATH)/Device/ST/STM32F4xx
HAL_PATH   		?= $(CUBE_PATH)/Drivers/STM32F4xx_HAL_Driver
USB_PATH		?= $(CUBE_PATH)/Middlewares/ST/STM32_USB_Host_Library
OUT 			?= out

VPATH 			= .:$(HAL_PATH)/Src:$(USB_PATH)/Core/Src:$(USB_PATH)/Class/HID/Src

CC      := arm-none-eabi-gcc
LD      := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size

CFLAGS  = -g -Wall -std=gnu11 -Os -fdump-rtl-expand
CFLAGS += -mlittle-endian -mcpu=cortex-m4 -mthumb
#CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS = -nostartfiles
LDFLAGS += -g -Wl,-Map=main.map -nostdlib
#LDFLAGS += -Wl,--gc-sections 
LIBS = -lgcc

CFLAGS += -DSTM32F407xx

LDFLAGS += -Tstm32_flash.ld
CFLAGS += -I$(CMSIS_DEV_PATH)/Include -I$(CMSIS_PATH)/Include -I$(HAL_PATH)/Inc -I. -Iconfig
LDFLAGS += -L$(CMSIS_PATH)/Lib/GCC

SOURCES = main.c keyboard.c menu.c usbh_conf.c startup_stm32f407xx.s system_stm32f4xx.c syscalls.c stm32f4xx_it.c
SOURCES_C = $(filter %.c,$(SOURCES))
SOURCES_ASM = $(filter %.s,$(SOURCES))

CFLAGS += -I$(USB_PATH)/Core/Inc -I$(USB_PATH)/Class/HID/Inc
USB_SOURCES := $(notdir $(wildcard $(USB_PATH)/Core/Src/*.c)) $(notdir $(wildcard $(USB_PATH)/Class/HID/Src/*.c))
USB_SOURCES := $(filter-out usbh_conf_template.c,$(USB_SOURCES))

HAL_SOURCES = stm32f4xx_hal.c
HAL_SOURCES += stm32f4xx_hal_rcc.c stm32f4xx_hal_flash.c stm32f4xx_hal_flash_ex.c stm32f4xx_hal_dma.c
HAL_SOURCES += stm32f4xx_hal_usart.c stm32f4xx_hal_pwr.c
HAL_SOURCES += stm32f4xx_hal_hcd.c stm32f4xx_hal_gpio.c stm32f4xx_hal_cortex.c
HAL_SOURCES += stm32f4xx_ll_usb.c

################################################################################

.PHONY: program clean

all: main.elf main.pdf

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

%.dot: %.elf
	r2 -a arm -qc 'aa;agC' $< 2>/dev/null >$@

#$(HAL_PATH)/Src/stm32f4xx_ll_utils.o
main.elf: $(SOURCES_ASM:.s=.o) $(SOURCES_C:.c=.o) $(USB_SOURCES:.c=.o) $(HAL_SOURCES:.c=.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(OBJCOPY) -O ihex $@ $(@:.elf=.hex)
	$(OBJCOPY) -O binary $@ $(@:.elf=.bin)
	$(OBJDUMP) -St $@ >$(@:.elf=.lst)
	$(SIZE) $@
	
program: main.elf openocd.cfg
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	rm -f **.o
	rm -f main.elf main.hex main.bin main.map main.lst
	rm -f **.expand

