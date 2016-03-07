/* #include "BasicSerial3.h" */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "softuart.h"
#include <util/delay.h>



int main(){
  softuart_init();

  sei();
  while(1){
    _delay_ms(1000);
    softuart_putchar('a');
  }
}
