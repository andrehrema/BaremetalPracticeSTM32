TARGET = main

LD_SCRIPT = STM32F030F4P6.ld
MCU_SPEC = cortex-m0

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

INCLUDE_PATHS=-I/home/andre/STM32/STM32F0/STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/andre/STM32/STM32F0/STM32CubeF0/Drivers/CMSIS/Include

# GCC compile options
CFLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall -g -fmessage-length=0 --specs=nosys.specs $(INCLUDE_PATHS)
#CFLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall -g -fmessage-length=0 --specs=nosys.specs -print-sysroot

# assembler options.
ASFLAGS = -c -O0 -mcpu=$(MCU_SPEC) -mthumb -Wall -fmessage-length=0

VECT_TBL = ./vector_table.s
AS_SRC   = ./core.s
C_SRC    = ./main.c

# Linker directives.
LSCRIPT = ./$(LD_SCRIPT)
LFLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall --specs=nosys.specs -nostdlib -lgcc -T$(LSCRIPT)

OBJS =  $(VECT_TBL:.s=.o)
OBJS += $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)


.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
