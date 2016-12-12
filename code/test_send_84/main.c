/* Evan Widloski - 2016-09-21

   Pinout
   ------
         ATTiny84
          ------
   VCC  --|o   |--  GND
   PB0  --|    |--  PA0 - RELAY
   PB1  --|    |--  PA1
   PB3  --|    |--  PA2 - TX/RX (TX = 0)
   PB2  --|    |--  PA3 - TX
   PA7  --|    |--  PA3
   PA6  --|    |--  PA5
          ------
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000

int main (void)
{
  // set PB1 to be output - connect your LED to pin 3
  DDRA = (1 << PA0) | (1 << PA2) | (1 << PA3);

  PORTA &= ~(1 << PA2);

  while (1) {


    PORTA |= (1 << PA0) | (1<< PA3);
    _delay_ms(200);

    PORTA &= ~(1 << PA0) & ~(1 << PA3);
    _delay_ms(200);
  }
  return 1;
}
