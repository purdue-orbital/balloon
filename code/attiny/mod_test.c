//Evan Widloski - 2016-02-20
//create 4 short pulses and 4 long pulses on OC1A
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/* #include "uart.h" */
#include <stdio.h>

volatile uint8_t pos = 0;
volatile uint8_t buffer[9] = {119,39,39,39,39,19,19,19,19};

void main()
{

  //clear OC1A at OCR1A, Set prescaler to 512
  TCCR1 =  1<<CTC1 | 1<<COM1A0 | 0<<CS13 | 1<<CS12 | 1<<CS11 | 1<<CS10;
  //set OC1A at OCR1B
  /* GTCCR = 1<<COM1B1 | 1<<COM1B0; */

  OCR1A = OCR1C = 10; //when to set OC1A

  DDRB = 1<<PB1 | 1<<PB0;  // enable output on PB1 (OC1A), and PB2 for debugging
  sei(); //enable interrupts
  TIMSK = 1<<OCIE1A; //interrupt when counter hits TOP

  /* uart_init(); */
  /* stdout = &uart_output; */
  /* stdin  = &uart_input; */


  while(1){
    /* printf("hello\n"); */
  }

}


ISR(TIMER1_COMPA_vect){
  //create 4 short pulses and 4 long pulses
  OCR1A = OCR1C = buffer[pos];
  pos++;
  pos %= 9;
}
