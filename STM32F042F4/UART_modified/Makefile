TARGET = main

LD_SCRIPT = STM32F042F4.ld
MCU_SPEC = cortex-m0
COREFLAGS = -mthumb -mcpu=$(MCU_SPEC)

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

INCLUDE_PATHS = -I/home/andre/STM32/STM32F0/STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Include -I/home/andre/STM32/STM32F0/STM32CubeF0/Drivers/CMSIS/Include

CFLAGS = $(COREFLAGS) -Wall -g -fmessage-length=0 --specs=nosys.specs $(INCLUDE_PATHS)
ASFLAGS = -c -O0 -mcpu=$(MCU_SPEC) -mthumb -Wall -fmessage-length=0

VECT_TBL = ./vector_table.S
AS_SRC = ./startup_stm32f042x6.s
C_SRC = ./main.c

# Linker directives.
LSCRIPT = ./$(LD_SCRIPT)
LFLAGS = $(COREFLAGS) -Wall --specs=nosys.specs -L/usr/lib/gcc/arm-none-eabi/9.2.1/ -lgcc -T$(LSCRIPT)

OBJS += $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.s
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET).elf $(TARGET).bin
