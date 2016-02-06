#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000
int main(void)
{
  TCCR0A = 3<<COM0A0 | 3<<COM0B0 | 3<<WGM00;
}
