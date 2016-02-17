// blink.c
#include <avr/io.h>
#include <util/delay.h>


void main()
{
  // setup timer1
  OCR1A = 1; // match count 1
  OCR1C = 1; // count to 2
  TCCR1 = (1<<CTC1)|(1<<COM1A0)|(1<<CS10);
  DDRB = (1<<PB1);  // set OC1A to output
  while(1){}
}
