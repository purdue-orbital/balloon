#include <SPI.h>
#define ATTENTION_IN 2
#define ATTENTION_OUT 3

char rxBuff [100];
char txBuff [100];
volatile byte nextRxBuffIndex;
volatile byte nextTxBuffIndex;
volatile byte currentTxBuffIndex;
volatile boolean process_it;

void setup (void) {
  
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
  nextRxBuffIndex = 0;   // buffer empty
  nextTxBuffIndex = 0;
  currentTxBuffIndex = 0;

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect) {

  // add to buffer if room
  if (nextRxBuffIndex < sizeof(rxBuff) && digitalRead(ATTENTION_IN) == LOW) {

    rxBuff[nextRxBuffIndex++] = SPDR;
    
  }
  
    // grab byte from SPI Data Register
  if (nextTxBuffIndex > 0) {
    
    digitalWrite(ATTENTION_OUT, LOW);
    SPDR = txBuff[currentTxBuffIndex]; 
    currentTxBuffIndex++;
    digitalWrite(ATTENTION_OUT, HIGH);
   
  }

}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void) {
  
  if (nextRxBuffIndex > 0 && digitalRead(ATTENTION_IN) == HIGH) {
    
    rxBuff[nextRxBuffIndex] = 0;  
    Serial.println (rxBuff);
    nextRxBuffIndex = 0;
    
  }  // end of flag set
    
}  // end of loop
