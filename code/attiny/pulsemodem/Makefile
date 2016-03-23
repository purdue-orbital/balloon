DEVICE     = attiny85
CLOCK      = 8000000
PROGRAMMER = usbasp
PORT	     = /dev/arduino
BAUD       = 19200
FILE       = main
COMPILE    = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: build upload

8mhz-internal:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

1mhz-internal:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

softuart:
	$(COMPILE) -c softuart.c -o softuart.o

modem:
	$(COMPILE) -c modem.c -o modem.o

build: modem softuart
	$(COMPILE) -c $(FILE).c -o $(FILE).o
	$(COMPILE) -o $(FILE).elf $(FILE).o modem.o softuart.o
	avr-objcopy -j .text -j .data -O ihex $(FILE).elf $(FILE).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILE).elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILE).hex:i 

clean:
	rm main.o
	rm main.elf
	rm main.hex
