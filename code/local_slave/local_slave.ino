#include <SPI.h>
#define ATTENTION_IN 2
#define ATTENTION_OUT 3

char buf [100];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (115200);   // debugging

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);


  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  pinMode(ATTENTION_IN, INPUT);
  pinMode(ATTENTION_OUT, OUTPUT);

  digitalWrite(ATTENTION_OUT, HIGH);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // grab byte from SPI Data Register
  //digitalWrite(ATTENTION_OUT, LOW);
  SPDR = 'f';
  //digitalWrite(ATTENTION_OUT, HIGH);
  
  // add to buffer if room
  if (pos < (sizeof (buf) - 1) && digitalRead(ATTENTION_IN) == LOW)
    buf [pos++] = c;
      
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (pos > 0 && digitalRead(ATTENTION_IN) == HIGH)
    {
    buf [pos] = 0;  
    Serial.println (buf);
    pos = 0;
    }  // end of flag set
    
}  // end of loop
