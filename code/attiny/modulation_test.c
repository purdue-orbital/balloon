//tone generation - 1KHz and 1.2KHz
//Evan Widloski - 2016-02-12
#include <avr/io.h>
#include <util/delay.h>

#define timer1_count TCINT1

void main()
{

  //1KHz = 8.192MHz/(prescaler * steps)
  //Let prescaler = 32
  //then steps = 256 ⇒ OCR1C = 255
  //% error = 0%

  //1.2KHz = 8.192MHz/(prescaler * steps)
  //Let prescaler = 32
  //then steps = 213.33 ≈ 213 ⇒ OCR1C = 212
  //% error = 1.6%


  //clear TCINT1 at OCR1C, Toggle 0C1A at OCR1A, Set prescaler to 32
  TCCR1 = 1<<CTC1 | 0<<COM1A1 | 1<<COM1A0 | 1<<CS12 | 1<<CS10;

  OCR1A = 0; //when to toggle OC1A
  OCR1C = 212; //when to reset TCINT1

  DDRB = (1<<PB1);  // set OC1A to output

  //oscillate between 1 and 1.2KHz
  while(1){
    _delay_ms(100);
    OCR1C = 212;
    _delay_ms(100);
    OCR1C = 255;


  }
}
