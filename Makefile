# put your *.o targets here, make should handle the rest!
CUBE_PATH 		?= $(wildcard ~)/resource/STM32CubeF4
CMSIS_PATH 		?= $(CUBE_PATH)/Drivers/CMSIS
CMSIS_DEV_PATH  ?= $(CMSIS_PATH)/Device/ST/STM32F4xx
HAL_PATH   		?= $(CUBE_PATH)/Drivers/STM32F4xx_HAL_Driver

CC      := arm-none-eabi-gcc
LD      := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size

CFLAGS  = -Wall -std=gnu11 -Os -fdump-rtl-expand
CFLAGS += -mlittle-endian -mcpu=cortex-m4 -mthumb
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS = -nostartfiles
LDFLAGS += -Wl,-Map=main.map -nostdlib
LDFLAGS += -Wl,--gc-sections 
LIBS = -lgcc
#LIBS += -lrdimon

CFLAGS += -DSTM32F407xx

LDFLAGS += -Tstm32_flash.ld
CFLAGS += -I$(CMSIS_DEV_PATH)/Include -I$(CMSIS_PATH)/Include -I$(HAL_PATH)/Inc -Iconfig
LDFLAGS += -L$(CMSIS_PATH)/Lib/GCC

###################################################

.PHONY: program clean

all: main.elf main.pdf

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

%.dot: %.elf
	r2 -a arm -qc 'aa;agC' $< 2>/dev/null >$@

main.elf: main.o startup_stm32f407xx.o system_stm32f4xx.c $(HAL_PATH)/Src/stm32f4xx_ll_utils.o
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

