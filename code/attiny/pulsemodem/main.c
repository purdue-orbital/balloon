#include <avr/io.h>
#include <util/delay.h>
#include "softuart.h"
#include "modem.h"

int main(){
  DDRB |= 1<<PB4;

  // ------------------ Modem Send Test -----------------------
  mod_init();
  char c;
  softuart_init();
  softuart_rx();

  DDRB |= 1<<PB1;
  while(1){
    /* if (softuart_kbhit()){ */
    /*   c = softuart_getchar(); */
    /*   PORTB ^= 1<<PB4; */

    /*   modem_put(c); */
    /* } */

    _delay_ms(100);
    PORTB ^= 1<<PB1;
    /* modem_put(0b11110000); */
    /* modem_put(0b11110000); */
    /* modem_put(0b11110000); */
    /* modem_put(0b11110000); */
  }

  // ------------------ Modem Receive and UART Send Test -----------------------
  /* demod_init(); */
  /* softuart_init(); */
  /* softuart_tx(); */
  /* char c; */
  /* while(1){ */
  /*   if (modem_available()){ */
  /*     PORTB ^= 1<<PB4; */
  /*     c = modem_get(); */
  /*     softuart_putchar(c); */

  /*   } */
  /* } */
}
