# Makefile for armclang
# make		  # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

CC = @armcc
CLANG = @armclang
LD = @armlink
AS = @armasm
FROMELF = @fromelf

OUTPUT_NAME = flash_debug

SCATTER = ".\flash_debug.sct"

TARGET = arm-arm-none-eabi
CORE_NAME_LOW = cortex-m0
CORE_NAME = Cortex-M0
DERIVATE = XMC1100_T038x0064
XMCLIB = "XMClib/inc"
DEVICE_INC = "G:/Keil_v5/ARM/PACK/Infineon/XMC1000_DFP/2.11.0/Device/XMC1100_series/Include"
CMSIS_CORE_INC = "G:/Keil_v5/ARM/PACK/ARM/CMSIS/5.6.0/CMSIS/Core/Include"
RTE_INC = RTE/_flash_debug/
RTE_DEVICE_INC = RTE/Device/XMC1100-T038x0064/

RV31_INC = "G:\Keil_v5\ARM\RV31\INC"
CMSIS_INC = "G:\Keil_v5\ARM\CMSIS\Include"

AFLAGS = --cpu $(CORE_NAME)
AFLAGS += -I$(RV31_INC) -I$(CMSIS_INC) -I$(DEVICE_INC)
AFLAGS += -g

OPT_LEVEL = fast

CFLAGS = -D$(DERIVATE) -I$(XMCLIB) -I$(DEVICE_INC) -I$(CMSIS_CORE_INC) -I$(RTE_INC) -I$(RTE_DEVICE_INC) -I.
CFLAGS += -xc -std=c11
#CFLAGS += --cpu $(CORE_NAME)
CFLAGS += --target=$(TARGET)
CFLAGS += -mcpu=$(CORE_NAME_LOW)
CFLAGS += -gdwarf-3
CFLAGS += -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
CFLAGS += -O$(OPT_LEVEL)
#CFLAGS += --apcs=interwork --split_sections
CFLAGS += -D_RTE_
CFLAGS += -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality

LFLAGS = --cpu $(CORE_NAME)
LFLAGS += --strict --scatter $(SCATTER)
LFLAGS += --summary_stderr --info summarysizes --map --xref --callgraph --symbols 
LFLAGS += --info sizes --info totals --info unused --info veneers 
LFLAGS += --list "./Listings/elfout.map" 

EXECUABLE = $(OUTPUT_NAME).axf
HEX = $(OUTPUT_NAME).hex

# A windows native link
RETARGET_IO_SRC = retarget_io.c

ALL_OBJ = main.o retarget_io.o xmc1100_it.o led.o dhry_2.o xmc1_flash.o xmc1_gpio.o xmc1_rtc.o xmc_common.o xmc1_scu.o xmc_gpio.o xmc_ccu4.o xmc_rtc.o xmc_uart.o system_XMC1100.o xmc_usic.o startup_xmc1100.o

all: hex
hex: $(HEX)
	
$(HEX): $(EXECUABLE)
	$(FROMELF) $< --i32combined --output $@
	
$(EXECUABLE): $(ALL_OBJ)
	@echo "linking.."
	$(LD) $(LFLAGS) $^ -o $@

startup_xmc1100.o: RTE/Device/XMC1100-T038x0064/startup_xmc1100.asm
	@echo "assembling..$< === $@"
	$(AS) $(AFLAGS) $<
	
retarget_io.o: $(RETARGET_IO_SRC)
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

dhry_2.o: dhry_2.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc1100_it.o: xmc1100_it.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

led.o: led.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

main.o: main.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc1_flash.o: ./XMCLib/src/xmc1_flash.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc1_gpio.o: ./XMCLib/src/xmc1_gpio.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc1_rtc.o: ./XMCLib/src/xmc1_rtc.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc_common.o: ./XMCLib/src/xmc_common.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc1_scu.o: ./XMCLib/src/xmc1_scu.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc_gpio.o: ./XMCLib/src/xmc_gpio.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc_ccu4.o: ./XMCLib/src/xmc_ccu4.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc_rtc.o: ./XMCLib/src/xmc_rtc.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc_uart.o: ./XMCLib/src/xmc_uart.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

system_XMC1100.o: RTE/Device/XMC1100-T038x0064/system_XMC1100.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

xmc_usic.o: ./XMCLib/src/xmc_usic.c
	@echo "compiling..$< === $@"
	$(CLANG) -c $(CFLAGS) $<

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${EXECUABLE}
