/* Evan Widloski - 2016-03-25
   Bell202 Modem for ATTiny85

   Adapted from https://sites.google.com/site/wayneholder/attiny-4-5-9-10-assembly-ide-and-programmer/bell-202-1200-baud-demodulator-in-an-attiny10

   Pinout
   ------
                      ATTiny85
                       ------
   mod select  PB5   --|o   |--
   data in     PB3   --|    |--   PB2  mod in
   data out    PB4   --|    |--   PB1  mod out
                     --|    |--
                       ------
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define AVG (out[oldest_sample] + out[oldest_sample + 1] + out[oldest_sample + 2] + out[oldest_sample + 3])>>2

uint8_t ADC_READY = 0;

int8_t samples[8] = {0,0,0,0,0,0,0,0};

int main()
{

  // --- Data pin Init ---
  // Set pullup on data in and mod select
  PORTB |= 1<<PB5 | 1<<PB3;

  // --- Demodulation Init ---
  //enable data out
  DDRB |= 1<<DDB4;

  // Set Timer 0 prescaler to 8
  TCCR0B = 1<<CS01;
  // Put Timer 0 in CTC mode (reset at OCR0A)
  TCCR0A = 1<<WGM01;
  // Sample at 9600Hz on 8MHz clock with 8x prescaler
  // (8000000/8)/9600 = 104.17
	OCR0A = 104;

	//Result Left Justified, channel ADC1
	ADMUX = (1<<ADLAR)|(1<<MUX0);
  // Enable ADC, enable ADC auto triggering
  ADCSRA = 1<<ADEN | 1<<ADATE;
  // Trigger ADC on Timer0 Compare Match A
  ADCSRB = 1<<ADTS1 | 1<<ADTS0;

  //start ADC
  ADCSRA |= 1<<ADSC;

  // --- Modulation Init ---

  // enable mod out
  DDRB |= 1<<DDB1;

  // enable toggling of OC1A
  TCCR1 |= 1<<COM1A0;
  // unmask data-in interrupt pin
  PCMSK |= 1<<PCINT3;

  // Put Timer 1 in CTC mode, Set Timer 1 Prescaler to 16
  TCCR1 |= 1<<CTC1 | 1<<CS12 | 1<<CS10;

  // interrupt on OCR1A match
  TIMSK |= 1<<OCIE1A;

  //enable global interrupts
	sei();

  int16_t out = 0;

  uint8_t oldest_sample = 0;
	while(1) {

    // check if ADC ready
    if (ADCSRA & 1<<ADIF){

      // reset the Timer 0 Compare Match A flag
      TIFR = 1<<OCF0A;

      // clear ADC flag
      ADCSRA |= 1<<ADIF;

      // get some vars ready for the computation
      int16_t cof1 = 0, cof2 = 0, cof3 = 0, cof4 = 0, sample_avg = 0;

      // add most recent ADC reading to samples[]
      samples[oldest_sample] = (int8_t)ADCH - 127;
      oldest_sample = (oldest_sample + 1) % 8;
      sample_avg = (samples[0] + samples[1] + samples[2] + samples[3] + samples[4] + samples[5] + samples[6] + samples[7])>>3;

      int16_t data = (samples[oldest_sample] - sample_avg) << 6;

      cof1 = data;
      cof2 = 0;
      cof3 = data;
      cof4 = 0;

      data = samples[(oldest_sample + 1)%8] - sample_avg;
      cof1 += data;
      cof2 += data;
      cof4 += data;
      data <<= 1;
      cof4 += data;
      data <<= 1;
      cof1 += data;
      cof2 += data;
      cof4 += data;
      data <<= 1;
      cof1 += data;
      cof2 += data;
      cof3 += data;
      cof4 += data;
      data <<= 1;
      cof4 += data;
      data <<= 1;
      cof1 += data;
      cof2 += data;
      cof4 += data;

      /* data = samples[(oldest_sample + 2)%8]; */
      data = samples[(oldest_sample + 2)%8] - sample_avg;
      cof4 += data;
      data <<= 1;
      cof3 -= data;
      data <<= 1;
      cof3 -= data;
      data <<= 1;
      cof3 -= data;
      data <<= 1;
      cof3 -= data;
      cof4 += data;
      data <<= 1;
      cof3 -= data;
      data <<= 1;
      cof2 += data;

      /* data = samples[(oldest_sample + 3)%8]; */
      data = samples[(oldest_sample + 3)%8] - sample_avg;
      cof1 -= data;
      cof2 += data;
      cof4 -= data;
      data <<= 1;
      cof4 -= data;
      data <<= 1;
      cof1 -= data;
      cof2 += data;
      data <<= 1;
      cof1 -= data;
      cof2 += data;
      cof3 -= data;
      cof4 -= data;
      data <<= 1;
      cof3 -= data;
      cof4 -= data;
      data <<= 1;
      cof1 -= data;
      cof2 += data;
      cof4 -= data;

      /* data = samples[(oldest_sample + 4)%8]; */
      data = samples[(oldest_sample + 4)%8] - sample_avg;
      cof3 += data;
      data <<= 1;
      cof3 += data;
      data <<= 1;
      cof3 += data;
      data <<= 1;
      data <<= 1;
      cof3 += data;
      data <<= 1;
      cof3 += data;
      cof4 -= data;
      data <<= 1;
      cof1 -= data;

      /* data = samples[(oldest_sample + 5)%8]; */
      data = samples[(oldest_sample + 5)%8] - sample_avg;
      cof1 -= data;
      cof2 -= data;
      cof3 += data;
      cof4 += data;
      data <<= 1;
      cof3 += data;
      cof4 += data;
      data <<= 1;
      cof1 -= data;
      cof2 -= data;
      cof3 += data;
      data <<= 1;
      cof1 -= data;
      cof2 -= data;
      data <<= 1;
      cof4 += data;
      data <<= 1;
      cof1 -= data;
      cof2 -= data;
      cof3 += data;
      cof4 += data;

      /* data = samples[(oldest_sample + 6)%8]; */
      data = samples[(oldest_sample + 6)%8] - sample_avg;
      cof3 -= data;
      cof4 += data;
      data <<= 1;
      data <<= 1;
      cof3 -= data;
      cof4 += data;
      data <<= 1;
      cof3 -= data;
      cof4 += data;
      data <<= 1;
      data <<= 1;
      cof3 -= data;
      cof4 += data;
      data <<= 1;
      cof2 -= data;

      /* data = samples[(oldest_sample + 7)%8]; */
      data = samples[(oldest_sample + 7)%8] - sample_avg;
      cof1 += data;
      cof2 -= data;
      cof3 -= data;
      cof4 -= data;
      data <<= 1;
      cof3 -= data;
      cof4 -= data;
      data <<= 1;
      cof1 += data;
      cof2 -= data;
      cof4 -= data;
      data <<= 1;
      cof1 += data;
      cof2 -= data;
      data <<= 1;
      cof3 -= data;
      data <<= 1;
      cof1 += data;
      cof2 -= data;
      cof3 -= data;
      cof4 -= data;

      cof1 >>= 8;
      cof2 >>= 8;
      cof3 >>= 8;
      cof4 >>= 8;


      if (cof1 < 0){
        cof1 *= -1;
      }
      if (cof2 < 0){
        cof2 *= -1;
      }
      if (cof3 < 0){
        cof3 *= -1;
      }
      if (cof4 < 0){
        cof4 *= -1;
      }

      out = cof3 + cof4 - cof1 - cof2;

      //2200Hz detected, set output high
      if (out > 4){
        PORTB |= 1<<PB4;
      }

      // 1200Hz detected, set output low
      else if (out < -4){
        PORTB &= ~(1<<PB4);
      }
    }
  }
}


ISR(TIMER1_COMPA_vect){
  if (PINB & 1<<PB3){
    // ((8000000/16)/2200)/2 = 113.64
    OCR1A = OCR1C = 113;
  }
  // if logic 0 input, modulate 1200Hz
  else {
    // ((8000000/16)/1200)/2 = 208.33
    OCR1A = OCR1C = 207;
  }
}
