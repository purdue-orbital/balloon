// Written by Nick Gammon
// February 2011
#define START_DELIMITER 0
#define LENGTH_1 1
#define LENGTH_2 2
#define FRAME_TYPE 3
#define FRAME_ID 4
#define DEST_1 5
#define DEST_2 6
#define DEST_3 7
#define DEST_4 8
#define DEST_5 9
#define DEST_6 10
#define DEST_7 11
#define DEST_8 12
#define OPTIONS 13
#define DATA_1 14
#define DATA_2 15
#define DATA_3 16
#define DATA_4 17
#define DATA_5 18
#define DATA_6 19
#define DATA_7 20

//Destination Addy (Other node's 64-bit MAC) 
#define DEST_1_BYTE 0x00
#define DEST_2_BYTE 0x13
#define DEST_3_BYTE 0xA2
#define DEST_4_BYTE 0x00
#define DEST_5_BYTE 0x40
#define DEST_6_BYTE 0xF6
#define DEST_7_BYTE 0xE1
#define DEST_8_BYTE 0x0C

//The SPI_ATTN pin, pulled low by radio when data is available
#define ATTN 52
#define SS_pin 10

#define POLY 0x8408
#define SWITCHING_DELAY 40 

#define RX_EN 3

#define START_BYTE 0x7E

//#define LED_SETUP DDRB = DDRB | B10000000
//#define LED_ON PORTB = PORTB | B10000000
//#define LED_OFF PORTB = PORTB & B01111111
#define RADIO_ID_1 {0x00,0x13,0xA2,0x00,0x40,0xF6,0xE1,0x0C}
#define RADIO_ID_2 {0x00,0x13,0xA2,0x00,0x41,0x48,0x88,0x07C}
#include <SPI.h>


void setup (void)
{

  digitalWrite(SS, HIGH);  // ensure SS stays high for now
  Serial.begin(9600);
  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  
}  // end of setup

void loop (void)
{

  char c;
  uint8_t pay[5] = {0x48,0x65,0x6C,0x6F,0x21};
  
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10
  delay(5);
  writeData(pay,sizeof(pay));
  delay(5);
  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (15);  // 1 seconds delay 
}  // end of loop

void writeData(uint8_t payload[],int len)
{
  uint8_t data[len+15];   
  data[START_DELIMITER] = START_BYTE;
  data[LENGTH_1] = (uint8_t) (len+11)>>8; //MSB. See next line
  data[LENGTH_2] = (uint8_t) (len+11); //LSB. Length 17 Bytes in packet EXCLUDING start delimiter,length bytes, and check sum.
  data[FRAME_TYPE] = 0x00;//0x10; //Transmit Request Frame
  data[FRAME_ID] = 0x00;//0x00; //Each packet does not have an individualized ID, confirmation packet will not be sent back
  data[DEST_1] = DEST_1_BYTE;
  data[DEST_2] = DEST_2_BYTE;
  data[DEST_3] = DEST_3_BYTE;
  data[DEST_4] = DEST_4_BYTE;
  data[DEST_5] = DEST_5_BYTE;
  data[DEST_6] = DEST_6_BYTE;
  data[DEST_7] = DEST_7_BYTE;
  data[DEST_8] = DEST_8_BYTE;
  data[OPTIONS] = 0x00;
  for(int i = 0; i<len;i++)
  {
    data[i+OPTIONS+1] = payload[i];
  }
  data[len+14] = chkSum(data,len+15);

  for (int i=0;i < sizeof(data);i++) 
  {
        SPI.transfer(data[i]);
        //Serial.println(data[i],HEX);    
  }
  //Serial.println("");
}

uint8_t chkSum(uint8_t data[], int size) //Shouldn't use for packets larger than 260 bytes total in size, unsigned int sum may rollover. Technically should still work though...?
{
  unsigned int sum = 0;
  for(int i=3;i<(size-1);i++)sum += data[i];//add all bytes except start delimeter, length bytes, and checksum byte
  return 0xFF-(uint8_t)sum;                 //return checksum: 0xFF minus this sum ^
}


/* PACKET OUTPUT FROM OTHER RADIO
126 7E
0 0
19 13
144 90
0 0
19 13
162 A2
0 0
65 41
72 48
136 88
124 7C
255 FF
254 FE
193 C1
72 48
101 65
108 6C
108 6C
111 6F
33 21
32 20
58 3A
 */
