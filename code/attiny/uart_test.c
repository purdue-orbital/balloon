#include "BasicSerial3.h"
#include <avr/io.h>


void serOut(const char* str)
{
  while (*str) TxByte (*str++);
}

int main(){
  uint8_t c;
  while (1){
    serOut("Serial echo test\n\r");
  }
}
