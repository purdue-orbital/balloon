DEVICE     = attiny85
CLOCK      = 8000000
PROGRAMMER = usbasp
PORT	   = /dev/arduino
BAUD       = 19200
FILE   = mod
COMPILE    = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: build upload

8mhz-internal:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

1mhz-internal:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

build:
	$(COMPILE) -c $(FILE).c -o $(FILE).o
	$(COMPILE) -o $(FILE).elf $(FILE).o
	avr-objcopy -j .text -j .data -O ihex $(FILE).elf $(FILE).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILE).elf

# BasicSerial3:
# 	avr-gcc -Wall -Os -DF_CPU=8000000 -mmcu=attiny85 -c BasicSerial3.S -o BasicSerial3.o

# uart_test: BasicSerial3
# 	$(COMPILE) -c uart_test.c -o uart_test.o
# 	$(COMPILE) -o uart_test.elf uart_test.o BasicSerial3.o
# 	avr-objcopy -j .text -j .data -O ihex uart_test.elf uart_test.hex
# 	avr-size --format=avr --mcu=$(DEVICE) uart_test.elf

softuart:
	$(COMPILE) -c softuart.c -o softuart.o

uart_test: softuart
	$(COMPILE) -c uart_test.c -o uart_test.o
	$(COMPILE) -o uart_test.elf uart_test.o softuart.o
	avr-objcopy -j .text -j .data -O ihex uart_test.elf uart_test.hex
	avr-size --format=avr --mcu=$(DEVICE) uart_test.elf

modem:
	$(COMPILE) -c modem.c -o modem.o


demod_test: modem
	$(COMPILE) -c demod_test.c -o demod_test.o
	$(COMPILE) -o demod_test.elf demod_test.o modem.o
	avr-objcopy -j .text -j .data -O ihex demod_test.elf demod_test.hex
	avr-size --format=avr --mcu=$(DEVICE) demod_test.elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILE).hex:i 

clean:
	rm main.o
	rm main.elf
	rm main.hex
