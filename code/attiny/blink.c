// blink.c
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// prototype
void wait_sec(int seconds);

int main (void)
{
  // direction register
  DDRB |= 0x03; // pin 0 and pin1 as output
  pinMode(0,OUTPUT);
  while(1)
    {
      PORTB |= 0x02; // LED ON
      wait_sec(1); // sleep
      PORTB &= 0x01; // LED OFF
      wait_sec(1); // sleep
    }
  return 0;

}

// sleeps the specified amount of seconds
void wait_sec(int seconds)
{
  int i;
  for(i=0; i<seconds; i++)
    _delay_ms(1000);

}
