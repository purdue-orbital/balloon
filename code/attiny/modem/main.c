#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


uint8_t ADC_READY = 0;

int8_t samples[8] = {0,0,0,0,0,0,0,0};


int main()
{
  //enable output for debugging
  DDRB = 1<<DDB2;

  //TIMER Initializations
  // Set Timer 0 prescaler to 8
  TCCR0B = 1<<CS01;
  // Put Timer 0 in CTC mode (reset at OCR0A)
  TCCR0A = 1<<WGM01;
  // Sample at 9600Hz on 8MHz clock with 8x prescaler
  // (8000000/8)/9600 = 104.17
	/* OCR0A = 103; */
	OCR0A = 51;

	//Result Left Justified, channel ADC2
	ADMUX = (1<<ADLAR)|(1<<MUX1);
  // Enable ADC, enable ADC auto triggering
  ADCSRA = 1<<ADEN | 1<<ADATE;
  // Trigger ADC on Timer0 Compare Match A
  ADCSRB = 1<<ADTS1 | 1<<ADTS0;

  //enable global interrupts
	sei();
  //start ADC
  ADCSRA |= 1<<ADSC;

  uint8_t oldest_sample = 0;
	while(1)
	{

    // check if ADC ready
    if (ADCSRA & 1<<ADIF){
      // reset the Timer 0 Compare Match A flag
      TIFR = 1<<OCF0A;

      // clear ADC flag
      ADCSRA |= 1<<ADIF;

      // get some vars ready for the computation
      int16_t cof1 = 0, cof2 = 0, cof3 = 0, cof4 = 0;

      // add most recent ADC reading to samples[]
      samples[oldest_sample] = (int8_t)ADCH - 127;
      oldest_sample = (oldest_sample + 1) % 8;



      int16_t data = samples[oldest_sample] << 6;

      cof1 = data;
      cof2 = 0;
      cof3 = data;
      cof4 = 0;


      data = samples[(oldest_sample + 1)%8];
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


      data = samples[(oldest_sample + 2)%8];
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

      data = samples[(oldest_sample + 3)%8];
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

      data = samples[(oldest_sample + 4)%8];
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

      data = samples[(oldest_sample + 5)%8];
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

      data = samples[(oldest_sample + 6)%8];
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

      data = samples[(oldest_sample + 7)%8];
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


      /* if (cof3*cof3 + cof4*cof4 - cof1*cof1 - cof2*cof2 < 0){ */
      /*   PINB = 1<<PB2; */
      /* } */
      if (samples[oldest_sample] > 0){
        PORTB = 1<<PB2;
      }
      else {
        PORTB &= ~(1<<PB2);
      }


      //toggle debug pin after computation complete
    }
	}

  return(0);
}

