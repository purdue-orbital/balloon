/* Evan Widloski - 2016-02-28
   Pulse Modem for ATTiny85

   Adapted to TX/RX from http://labs.rakettitiede.com/12kbps-simple-audio-data-transfer-for-avr/
*/

#pragma once

#include <avr/interrupt.h>
#include <stdlib.h>

/* Modem ring buffer size must be power of 2 */
#define MODEM_BUFFER_SIZE	4

/* Modem defines */
#define MODEM_TX_SYNC_LEN	199
#define MODEM_TX_ONE_LEN 124
#define MODEM_TX_ZERO_LEN 24

#define MODEM_RX_SYNC_LEN 180

#define MODEM_TIMER		TCNT1
#define MODEM_RX_PIN 		PB0
#define MODEM_TX_PIN 		PB1
#define MODEM_DDR		DDRB
#define MODEM_PORT		PORTB

#define MODEM_PRESCALE 0<<CS13 | 1<<CS12 | 0<<CS11 | 1<<CS10

/* Public funtions */
uint8_t modem_available();
uint8_t modem_get();
void demod_init();

void modem_put(uint8_t);
void mod_init();
