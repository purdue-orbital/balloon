#include <avr/io.h>
#include <util/delay.h>
#include "softuart.h"
#include "modem.h"

int main(){
  DDRB |= 1<<PB1;

  // ------------------ Modem Send Test -----------------------
  /* mod_init(); */
  /* char c; */
  /* modem_put(0b11110000); */
  /* while(1){ */
  /*   _delay_ms(100); */
  /*   modem_put(0b11110000); */
  /*   modem_put(0b11110000); */
  /*   modem_put(0b11110000); */
  /*   modem_put(0b11110000); */
  /* } */

  // ------------------ Modem Receive and UART Send Test -----------------------
  /* demod_init(); */
  /* softuart_init(); */
  /* softuart_tx(); */
  /* char c; */
  /* while(1){ */
  /*   if (modem_available()){ */
  /*     c = modem_get(); */
  /*     softuart_putchar(0x00); */

  /*   } */
  /* } */

  // ------------------- UART Receive Test ------------------------
  softuart_init();
  softuart_rx();
  char c;
  while(1){
    if (softuart_kbhit()){
      if(softuart_getchar() == 0x00){
        PORTB ^= 1<<PB1;
      }

    }
  }
}
