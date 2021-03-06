PRG              = front-light

SRC              = ./main.c
SRC              += ./serial.c
SRC              += ./i2c_master.c
SRC              += ./ds1307.c

OBJS             = $(SRC:.c=.o)

LIB_NAME         = #
OTHER_LIBS_NAMES = #
LIB_NAMES        = $(LIB_NAME) $(OTHER_LIBS_NAMES)

PROGRAMMER       = usbasp
#PROGRAMMER       = avrisp
PORT             = usb
MCU_TARGET       = atmega328p
AVRDUDE_TARGET   = atmega328p

HZ               = 20000000
SERIAL_BAUD      = 38400

DEFINE           = F_CPU=$(HZ)

DEFINE           += SERIAL_BAUD=$(SERIAL_BAUD)
DEFINE           += I2C_SCL_FREQ=10000
DEFINE           += I2C_PRESCALER=16

OPTIMIZE         = -Os
DEFS             = $(patsubst %,-D%,$(DEFINE))
LIBS             = $(patsubst lib%.a,-l%,$(LIB_NAMES))
LINK_LIBS        = -L./ $(LIBS)

LFUSE            = 0xFF
HFUSE            = 0xDF
EFUSE            = 0xFF

B_RATE           = $(shell echo $$(( 4000000 / $(HZ) + 1 )) )

# You should not have to change anything below here.

CC               = avr-gcc
AR               = avr-ar

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(LIB_NAMES) $(PRG).elf lst text #eeprom

$(PRG).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LINK_LIBS)

lib%.a : %.o
	$(AR) rcs $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES) $(LIB_NAME)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec


ehex:  $(PRG)_eeprom.hex
#ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

#%_eeprom.srec: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@


# command to program chip (invoked by running "make install")
install: $(LIB_NAMES) $(PRG).hex
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v  \
	-B $(B_RATE) -U flash:w:$(PRG).hex

fuse:
	avrdude -F -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
	-B $(B_RATE) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

read_fuse:
	avrdude -F -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
	-B $(B_RATE) -U lfuse:r:-:i -U hfuse:r:-:i -U efuse:r:-:i

#fuse:
#	sudo avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
#	-U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

serial:
	gtkterm -p /dev/ttyUSB0 -s $(SERIAL_BAUD)

ddd: gdbinit
	ddd --debugger "avr-gdb -x $(GDBINITFILE)"

gdbserver: gdbinit
	simulavr --device $(MCU_TARGET) --gdbserver

gdbinit: $(GDBINITFILE)

size: $(PRG).elf
	avr-size $<

$(GDBINITFILE): $(PRG).hex
	@echo "file $(PRG).elf" > $(GDBINITFILE)

	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load"                         >> $(GDBINITFILE)
	@echo "break main"                   >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"
