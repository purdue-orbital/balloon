/* Name: main.c
 *
 * Modem demo main code. All this does is run in loop and forward
 * bytes coming to ring buffer to SPI.
 *
 * Author: Jari Tulilahti
 * Copyright: 2014 Rakettitiede Oy
 * License: LGPLv3, see COPYING, and COPYING.LESSER -files for more info
 */

#include "modem.h"

/*
 * Set up SPI and Modem
 * Entry point of the program 
 */
int main(void)
{
	/* Call setup */
	modem_init();
  DDRB = 1<<PCINT0;

	/* Loop forever */
	for (;;) {
		if (modem_buffer_available()) {
      modem_buffer_get();
		}
	}

	/* We never reach this */
	return 0;
}
