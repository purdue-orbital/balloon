/* Evan Widloski - 2016-09-21

   Pinout
   ------
         ATTiny84
          ------
   VCC  --|o   |--  GND
   PB0  --|    |--  PA0 - RELAY
   PB1  --|    |--  PA1
   PB3  --|    |--  PA2 - Radio TX/RX (TX = 0)
   PB2  --|    |--  PA3 - TX
   PA7  --|    |--  PA4 - RX
   PA6  --|    |--  PA5
          ------
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//
// A simple program for the ATtiny84 that blinks an LED.
//
// electronut.in
//

#include <avr/io.h>
#include <util/delay.h>
 
#define F_CPU 8000000

int main (void)
{
  DDRA = (1 << PA0) | (1 << PA2) | (1 << PA3);

  // turn off radio
  PORTA |= (1 << PA2);

  while (1) {

    if (PINA & (1 << PA4)){
      PORTA |= (1 << PA0);
    }
    else {
      PORTA &= ~(1 << PA0);
    }
  }
  return 1;
}
