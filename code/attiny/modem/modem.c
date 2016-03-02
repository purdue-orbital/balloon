#include "modem.h"

//Ring buffer global variables
static volatile uint8_t modem_buffer_head = 0, modem_buffer_tail = 0;
static volatile uint8_t modem_buffer[MODEM_BUFFER_SIZE];

//Current bit position for transmission of byte
static volatile uint8_t pos = 0;
//sync bits left to send
static volatile uint8_t sync_bits = 2;

//Returns number of available bytes in ringbuffer or 0 if empty
uint8_t modem_available() {
	return modem_buffer_head - modem_buffer_tail;
}

//Store 1 byte in ringbuffer
static inline void modem_buffer_put(const uint8_t c) {
	if (modem_available() != MODEM_BUFFER_SIZE) { 
		modem_buffer[modem_buffer_head++ % MODEM_BUFFER_SIZE] = c;
	} 
}

/* ------------------ Demodulation ---------------------- */

//Fetch 1 byte from ringbuffer
uint8_t modem_get() {
	uint8_t b = 0;
	if (modem_available() != 0) {
		b = modem_buffer[modem_buffer_tail++ % MODEM_BUFFER_SIZE];
	}
	return b;
}

//Pin Change Interrupt Vector. This is The Modem.
ISR(PCINT0_vect) {
	//Static variables instead of globals to keep scope inside ISR
	static uint8_t modem_bit = 0;
	static uint8_t modem_bitlen = 0;
	static uint8_t modem_byte = 0;

	//Read & Zero Timer/Counter 1 value
	uint8_t modem_pulselen = MODEM_TIMER;
	MODEM_TIMER = 0;

	 /* Check if we received Start/Sync -pulse. */
	 /* Calculate bit signal length middle point from pulse. */
	 /* Return from ISR immediately. */
	if (modem_pulselen > MODEM_RX_SYNC_LEN) {
		modem_bitlen = (modem_pulselen >> 2);
		modem_bit = 0;
		return;
	}

	/*
	 * Shift byte and set high bit according to the pulse length.
	 * Long pulse = 1, Short pulse = 0
	 */
	modem_byte = (modem_byte >> 1) | (modem_pulselen < modem_bitlen ? 0x00 : 0x80);

	// Check if we received complete byte and store it in ring buffer
	if (!(++modem_bit % 0x08)) {
		modem_buffer_put(modem_byte);
	}
}

// initialize modem for receiving
void demod_init()  {

	// Modem pin as input
	MODEM_DDR &= ~(1 << MODEM_RX_PIN);

	// Enable Pin Change Interrupts and PCINT for MODEM_RX_PIN
	GIMSK |= (1 << PCIE);
	PCMSK |= (1 << MODEM_RX_PIN);

	// Timer: TCCR1: CS10, CS11 and CS12 bits: 8MHz clock with Prescaler 64 = 125kHz timer clock
	TCCR1 = (0 << CS10) | (0 << CS11) | (1 << CS12);

	/* Enable interrupts */
	sei();
} 


/* ------------------ Modulation ---------------------- */

void modem_put(uint8_t c) {
  modem_buffer_put(c);

  TIMSK |= 1<<OCIE1A ; //interrupt when counter hits TOP
}

//this function is called for each bit at the tail byte in the modem buffer
ISR(TIMER1_COMPA_vect){
  MODEM_PORT ^= 1<<MODEM_TX_PIN;

  if (sync_bits){
    OCR1A = OCR1C = MODEM_TX_SYNC_LEN;
    sync_bits--;
    return;
  }

  //once the end of the byte is reached, reset bit counter.
  if (pos > 7){
    modem_buffer_tail++;
    pos = 0;
    //if there are no more bytes to send, disable timer1_compa
    //  interrupt and reset the sync bits for next time there
    //  are bits available
    if(!modem_available()){
      TIMSK &= ~(1<<OCIE1A);
      sync_bits=2;
      return;
    }
  }

  //set output toggle delay
  if((modem_buffer[modem_buffer_tail % MODEM_BUFFER_SIZE] >> pos)%2)
    OCR1A = OCR1C = MODEM_TX_ONE_LEN;
  else
    OCR1A = OCR1C = MODEM_TX_ZERO_LEN;

  pos++;

}

// initialize modem for sending
void mod_init() {
  //clear TCNT1 at OCR1C,, set prescaler 
  TCCR1 =  1<<CTC1 | 0<<CS13 | 1<<CS12 | 0<<CS11 | 0<<CS10;

  MODEM_DDR = 1<<MODEM_TX_PIN;
  sei(); //enable interrupts
}
