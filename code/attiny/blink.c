// blink.c
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000

int main (void)
{
    // see http://matt16060936.blogspot.com/2012/04/attiny-pwm.html

    // direction register - set PB0 output
    DDRB = 1<<DDB0;

    // clear 0C0A and set to BOTTOM on compare-match
    TCCR0A = 1<<COM0A1 | 0<<COM0A0;

    // enable fast pwm
    TCCR0A |= 0<<WGM01 | 0<<WGM00;
    TCCR0B = 0<<WGM02;

    // set counter 0 prescaler to 1
    TCCR0B |= 1<<CS00;

    int i = 0;
    for (;;) {
      if (++i>255) i=0;
      OCR0A = i;
      _delay_ms(10);
    }
    return 0;
}
