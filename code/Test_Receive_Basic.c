#include<SPI.h>
#define LED_PIN 8
#define RAD_ATTN 9
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

#define RECEIVE_SIZE 22

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


uint8_t ardTxBuff[300];
uint8_t radRxBuff[300];
int nextArdTxBuffIndex = 0;
int nextRadRxBuffIndex = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  digitalWrite(SS,HIGH);
  pinMode(RAD_ATTN,INPUT);
  pinMode(LED_PIN,OUTPUT);
}
void loop()
{
  digitalWrite(SS,LOW);
  delay(2);
  for(int i = 0; i<100 && digitalRead(RAD_ATTN)==LOW;i++)
    addRadRxBuff(SPI.transfer(0x00));
  Serial.print("about to parse");
  if(nextRadRxBuffIndex>0)
    parseRadData();
  delay(3);
  digitalWrite(SS,HIGH);
  if(nextArdTxBuffIndex>0)
    parseTxData();
  delay(3);
  
}
/*--------------------------------BUFFER----------------------------------*/
/*----------------------------------ADDTXBUFF--------------------------------*/
void addArdTxBuff(uint8_t data[],int size)
{
  for(int i = 0; i<size && nextArdTxBuffIndex<sizeof(ardTxBuff);i++)
  {
    ardTxBuff[nextArdTxBuffIndex++] = data[i];
    //Serial.print(data[i]);
    //Serial.println(" was added to ardTxBuff");
  }
  return;
}
/*----------------------------------ADDRXBUFF--------------------------------*/
void addRadRxBuff(char data)
{
  if(nextRadRxBuffIndex < sizeof(radRxBuff))
  {
    radRxBuff[nextRadRxBuffIndex++] = data;
  }
  else
  {
    nextRadRxBuffIndex = 0;
    radRxBuff[nextRadRxBuffIndex++] = data;
  }
  Serial.print(data,HEX);
  Serial.print(" ");
  Serial.print((char) data);
  Serial.println(" was added to radRxBuff");
  return;  
}
/*-------------------------PARSE-RADIO-DATA----------------------------*/
void parseRadData()
{
  uint8_t data[RECEIVE_SIZE];
  size_t n = 0;
  char c;
  int chk;
  int i = 0;
  bool isCorrupt;
  bool isPacket;
  bool done=0;
  int nextStart = 0;
  int nextStart2 = 0;
  char address = 'n';
  int k = 0;
  int lastUsed = 0;
  int lastMeaningful = 0;
  int oldNextRadRx = nextRadRxBuffIndex;

  //Serial.println("Entering parse... ");
  
  while(((k<oldNextRadRx && k<sizeof(radRxBuff))||(radRxBuff[oldNextRadRx-sizeof(data)]==START_BYTE && !(k<oldNextRadRx && k<sizeof(radRxBuff)) && done!=1))&& ((k-lastUsed)<=sizeof(data)))
  {
    if(k<oldNextRadRx && k<sizeof(radRxBuff))
    {
      c = radRxBuff[k];
      lastUsed = k;
    }
    k++;
    if(i == sizeof(data))//If the last byte of a possible packet is reached
    {
      //Serial.print(1);
      //NOTE this does not adapt to different sized data packets because sizeof(data) is defined and constant
      chk = chkSum(&data[0], sizeof(data));
      /*Serial.print("Generated chk: ");
      Serial.print(chk,HEX);
      Serial.print(" chk found in packet: ");
      Serial.println(data[sizeof(data)-1],HEX);
      printArray(&data[0],sizeof(data));*/
      if(data[sizeof(data)-1]==chk)//compare last byte of packet to calculated hash sum
      {
        //Serial.print(2);
        i = 0;
        nextStart = 0;
        addArdTxBuff(&data[14],6);//Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
        printArray(&data[14],6);
        address = data[14]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2.
        done = 1;
        lastMeaningful = lastUsed-1;//c hasn't actually been added to data[] yet, so the last byte that we know has meaning is the byte before lastUsed. c is added near the end of this loop...
      }  
      else//checksum of packet does not pass
      {
        //Serial.print(3);
        //for(int index = 0; index<sizeof(data);index++) 
          //Serial.print((char) data[index]);
        if(nextStart!=0)//If there was a startbyte in middle of what was a possible packet
        {
          //Serial.print(4);
          int j;
          for(j = nextStart;j < sizeof(data);j++)
          {
            //Serial.print(5);
            if(data[j]==START_BYTE) nextStart2 = j-nextStart; //if there is another start even after shifting everything over....
            data[j-nextStart] = data[j];
          }
          i = j - nextStart;
          nextStart = nextStart2;
        }
        done = 0;
      }
    }
    
    if(c == START_BYTE && i != 0)
    {
      //Serial.print(6);
      if(i>sizeof(data))
      {
        //Serial.print(7);
        i = 0;
        nextStart = 0;
      }
      else if(nextStart==0)
      {
        nextStart = i;
        //Serial.print(8);
      }
    }

    if(i<sizeof(data))
    {
      data[i]=c;
      //Serial.print(9);
      i++;
    }
  }
  for(k = lastMeaningful+1; k<nextRadRxBuffIndex && k<sizeof(radRxBuff);k++)
  {
    radRxBuff[k-lastMeaningful-1] = radRxBuff[k];
  }
  nextRadRxBuffIndex = k-lastMeaningful-1;
  return;
}
/*------------Print-Array-----------------*/
void printArray(uint8_t data[],int size)
{
  for(int i=0;i<size;i++)
  {
    Serial.print((char) data[i]);
    Serial.println(",");
  }
}
uint8_t chkSum(uint8_t data[], int size) //Shouldn't use for packets larger than 260 bytes total in size, unsigned int sum may rollover. Technically should still work though...?
{
  unsigned int sum = 0;
  for(int i=3;i<(size-1);i++)sum += data[i];//add all bytes except start delimeter, length bytes, and checksum byte
  return 0xFF-(uint8_t)sum;                 //return checksum: 0xFF minus this sum ^
}
/*--------------------------------PARSE----------------------------------*/
void parseTxData()
{
  uint8_t data[6];
  size_t n = 0;
  char c;
  int crc;
  int i = 0;
  bool isCorrupt;
  bool isPacket;
  bool done=0;
  int nextStart = 0;
  int nextStart2 = 0;
  char address = 'n';
  int k = 0;
  int lastUsed = 0;
  int lastMeaningful = 0;
  int oldNextTx = nextArdTxBuffIndex;
  
  while((k<oldNextTx && k<sizeof(ardTxBuff))||(ardTxBuff[oldNextTx-6]=='@' && !(k<oldNextTx && k<sizeof(ardTxBuff)) && done!=1))
  {
    if(k<oldNextTx && k<sizeof(ardTxBuff))
    {
      c = ardTxBuff[k];
      lastUsed = k;
    }
    k++;
    if(i == sizeof(data))//If the last byte of a possible packet is reached
    {
      //Serial.print(1);
      //NOTE this does not adapt to different sized data packets because sizeof(data) is defined and constant
      crc = crc16(&data[1], 3);
      if(data[4]==(uint8_t) (crc & 0xFF)&& data[5] == (uint8_t) ((crc >> 8) & 0xFF))//compare last byte of packet to calculated hash sum
      {
        //Serial.print(2);
        i = 0;
        nextStart = 0;
        actOnPacket(data[1],data[2],data[3]);//Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
        address = data[1]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2.
        done = 1;
        lastMeaningful = lastUsed-1;//c hasn't actually been added to data[] yet, so the last byte that we know has meaning is the byte before lastUsed. c is added near the end of this loop...
      }  
      else//checksum of packet does not pass
      {
        //Serial.print(3);
        //for(int index = 0; index<sizeof(data);index++) 
          //Serial.print((char) data[index]);
        if(nextStart!=0)//If there was a startbyte in middle of what was a possible packet
        {
          //Serial.print(4);
          int j;
          for(j = nextStart;j < sizeof(data);j++)
          {
            //Serial.print(5);
            if(data[j]=='@') nextStart2 = j-nextStart; //if there is another start even after shifting everything over....
            data[j-nextStart] = data[j];
          }
          i = j - nextStart;
          nextStart = nextStart2;
        }
        done = 0;
      }
    }
    
    if(c == '@' && i != 0)
    {
      //Serial.print(6);
      if(i>sizeof(data))
      {
        //Serial.print(7);
        i = 0;
        nextStart = 0;
      }
      else if(nextStart==0)
      {
        nextStart = i;
        //Serial.print(8);
      }
    }

    if(i<sizeof(data))
    {
      data[i]=c;
      //Serial.print(9);
      i++;
    }
  }
  noInterrupts();
  for(k = lastMeaningful+1; k<nextArdTxBuffIndex && k<sizeof(ardTxBuff);k++)
  {
    ardTxBuff[k-lastMeaningful-1] = ardTxBuff[k];
  }
  nextArdTxBuffIndex = k-lastMeaningful-1;
  interrupts();
  return;
}

void actOnPacket(uint8_t address, uint8_t arg1, uint8_t arg2)
{
  switch(address)
  {
    case 'c':
      if(arg1=='o' && arg2 == 'n')
        digitalWrite(LED_PIN,HIGH);
        delay(1000);
        digitalWrite(LED_PIN,LOW);
      break;
  }
  return;
}
uint16_t crc16(uint8_t *data_p, unsigned short length)
{
      unsigned char i;
      unsigned int data;
      unsigned int crc = 0xffff;
 
      if (length == 0)
            return (~crc);
 
      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8; 
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);
 
      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);
 
      return (crc);
}
