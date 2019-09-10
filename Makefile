# Usage:
# make		  # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

CC = @armcc
LD = @armlink
AS = @armasm
FROMELF = @fromelf

OUTPUT_NAME = flash_debug

SCATTER = flash_debug.sct

CORE_NAME = Cortex-M0
DERIVATE = XMC1100_T038x0064
XMCLIB = "G:/Keil_v5/ARM/PACK/Infineon/XMC1000_DFP/2.11.0/Device/XMClib/inc"
DEVICE_INC = "G:/Keil_v5/ARM/PACK/Infineon/XMC1000_DFP/2.11.0/Device/XMC1100_series/Include"
CMSIS_CORE_INC = "G:/Keil_v5/ARM/PACK/ARM/CMSIS/5.6.0/CMSIS/Core/Include"
RTE_INC = RTE/_flash_debug/
RTE_DEVICE_INC = RTE/Device/XMC1100-T038x0064/

OPT_LEVEL = 0

AFLAGS = --cpu $(CORE_NAME)

CFLAGS = -D$(DERIVATE) -I$(XMCLIB) -I$(DEVICE_INC) -I$(CMSIS_CORE_INC) -I$(RTE_INC) -I$(RTE_DEVICE_INC)
CFLAGS += --c99
CFLAGS += --cpu $(CORE_NAME)
CFLAGS += -g
CFLAGS += -O$(OPT_LEVEL)
CFLAGS += --apcs=interwork --split_sections
CFLAGS += -D_RTE_

LFLAGS = --cpu $(CORE_NAME)
LFLAGS += --strict --scatter $(SCATTER)
LFLAGS += --summary_stderr --info summarysizes --map --xref --callgraph --symbols 
LFLAGS += --info sizes --info totals --info unused --info veneers 
LFLAGS += --list "./Listings/elfout.map" 

EXECUABLE = $(OUTPUT_NAME).axf
HEX = $(OUTPUT_NAME).hex

# A windows native link
RETARGET_IO_SRC = retarget_io.c

ALL_OBJ = retarget_io.o ring_buffer.o xmc1100_it.o stdio_impl.o led.o main.o shell.o xmc1_flash.o xmc1_gpio.o xmc1_rtc.o xmc_common.o xmc1_scu.o xmc_gpio.o xmc_ccu4.o xmc_rtc.o xmc_uart.o system_XMC1100.o xmc_usic.o startup_xmc1100.o

all: hex
hex: $(HEX)
	
$(HEX): $(EXECUABLE)
	$(FROMELF) $< --i32combined --output $@
	
$(EXECUABLE): $(ALL_OBJ)
	@echo "Checking.."
	$(LD) $(LFLAGS) $^ -o $@

startup_xmc1100.o: RTE/Device/XMC1100-T038x0064/startup_xmc1100.s
	$(AS) $(AFLAGS)
	
retarget_io.o: $(RETARGET_IO_SRC)
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

ring_buffer.o: ring_buffer.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc1100_it.o: xmc1100_it.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

stdio_impl.o: stdio_impl.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

led.o: led.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

main.o: main.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

shell.o: shell.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc1_flash.o: ./XMCLib/src/xmc1_flash.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc1_gpio.o: ./XMCLib/src/xmc1_gpio.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc1_rtc.o: ./XMCLib/src/xmc1_rtc.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc_common.o: ./XMCLib/src/xmc_common.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc1_scu.o: ./XMCLib/src/xmc1_scu.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc_gpio.o: ./XMCLib/src/xmc_gpio.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc_ccu4.o: ./XMCLib/src/xmc_ccu4.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc_rtc.o: ./XMCLib/src/xmc_rtc.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc_uart.o: ./XMCLib/src/xmc_uart.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

system_XMC1100.o: RTE/Device/XMC1100-T038x0064/system_XMC1100.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

xmc_usic.o: ./XMCLib/src/xmc_usic.c
	@echo "Creating object..$< === $@"
	$(CC) -c $(CFLAGS) $<

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${EXECUABLE}j
