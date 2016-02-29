#include <avr/io.h>
#include <util/delay.h>
#include "softuart.h"
#include "modem.h"

int main(){
/*   mod_init(); */

/*   DDRB |= 1<<PB0; */
/*   char c; */
/*   modem_put(0b11110000); */
/*   while(1){ */
/*     _delay_ms(100); */
/*     modem_put(0b11110000); */
/*     modem_put(0b11110000); */
/*     modem_put(0b11110000); */
/*     modem_put(0b11110000); */
/*   } */

  demod_init();
  softuart_init();

  DDRB |= 1<<PB1;
  char c;
  while(1){
    if (modem_available()){
      c = modem_get();
      PORTB ^= 1<<PB1;
      softuart_putchar(0x00);

    }
  }
}
