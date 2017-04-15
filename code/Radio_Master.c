#include<SPI.h>
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


uint16_t crc16(uint8_t *data_p, unsigned short length);
void extractPackets();
void actOnPacket(char,char,char);
void addActOnBuff(char);
void flushActOnBuff(void);
void rx_enable(void);
void tx_enable(void);

using namespace std;
char actOnBuff[300];
int nextActBuffIndex = 0;
int board;
int tOutDelay;
boolean synched;
uint8_t ardTxBuff[300];
uint8_t radRxBuff[300];
int nextArdTxBuffIndex = 0;
int nextRadRxBuffIndex = 0;

int lastcalled = 0;

void setup() {
  Serial.begin (115200);   // debugging
  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  digitalWrite(SS,HIGH);
  pinMode(RAD_ATTN,INPUT);

  pinMode(ATTENTION_IN, INPUT);
  pinMode(ATTENTION_OUT, OUTPUT);

  digitalWrite(ATTENTION_OUT, HIGH);
}

void loop()
{
/*----------------------Receive-From-Radio-Test-------------------*/
//  digitalWrite(SS,LOW);
//  delay(2);
//  for(int i = 0; i<100 && digitalRead(RAD_ATTN)==LOW;i++)
//    addRadRxBuff(SPI.transfer(0x00));
//  if(nextRadRxBuffIndex>0)
//    parseRadData();
//  delay(3);
//  digitalWrite(SS,HIGH);
//  delay(3); 
}

/*----------------------------------TALK--------------------------------*/
void talk()
{
  int crc;
  if(nextTxBuffIndex > 0) //if there is something to send then enter 
  {
    for(int i=0; (i+2)<nextTxBuffIndex&&(i+2)<sizeof(txBuff);i+=3)//send stuff until there is nothing left to send
    { 
      if(digitalRead(ATTENTION_IN) == HIGH)//if there is nothing to receive then just send and don't bother saving spi response
      {
        SPI.transfer('@');
        SPI.transfer(txBuff[i]);
        SPI.transfer(txBuff[i+1]);
        SPI.transfer(txBuff[i+2]);
        crc = crc16(&txBuff[i],3);
        SPI.transfer((uint8_t) (crc & 0xFF ));
        SPI.transfer((uint8_t) ((crc >> 8) & 0xFF));
      }
      else //there must be something to receive and send so send stuff from txBuff and add response to rxBuff
      {
        addRxBuff(SPI.transfer('@'));
        addRxBuff(SPI.transfer(txBuff[i]));
        addRxBuff(SPI.transfer(txBuff[i+1]));
        addRxBuff(SPI.transfer(txBuff[i+2]));
        crc = crc16(&txBuff[i],3);
        addRxBuff(SPI.transfer((uint8_t) (crc & 0xFF )));
        addRxBuff(SPI.transfer((uint8_t) ((crc >> 8) & 0xFF)));
      }
    }
    //flaw in parseRxData requires an '@' at the end of all streams of data
    if(digitalRead(ATTENTION_IN)==HIGH)
      SPI.transfer('@');
    else
      addRxBuff(SPI.transfer('@'));
    
    digitalWrite(ATTENTION_OUT,HIGH);//Let the other arduino know that we no longer have meaningful data to send
    nextTxBuffIndex = 0;
  }
 
  while(nextTxBuffIndex == 0 && digitalRead(ATTENTION_IN) == LOW) { //Have nothing of meaning to send so send garbage to get data back until there is no data to receive
    addRxBuff(SPI.transfer('e'));
  }
  
  if(nextRxBuffIndex > 0) //Do something with the data received
    parseRxData();
  
  return;
}

void extractUIPackets()
{
    uint8_t data[6]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
    size_t n = 0;
    char c;
    int crc;
    int i = 0;
    bool isCorrupt;
    bool isPacket;
    bool done=0;
    int nextStart = 0;
    char address = 'n';
    int tLastRead;

    while(address!='e')
    {
        if(tLastRead - millis() > tOutDelay){ writeByCommand('e'); digitalWrite(13,LOW); synched = 0;} //Timeout.. haven't received a packet in a while
        
        if(digitalRead(ATTN)==0)//not sure if this is necessary, used to be used to make sure program wasn't getting ahead of serial stream
        {
            if(synched == 0){digitalWrite(13,HIGH); synched = 1;}
            c = SPI.transfer(0); //Not sure if this is what we want to send, but we really just want to get bytes! Don't care about what is sent. Should look into the proper bytes to be sent when simply reading bytes
            if(i == sizeof(data))//If the last byte of a possible packet is reached
            {	
				//NOTE this does not adapt to different sized data packets because sizeof(data) is defined and constant
				if(data[sizeof(data)-1] == chkSum(data,sizeof(data)))//compare last byte of packet to calculated hash sum
                {
					i = 0;
                    nextStart = 0;
                    actOnPacket(data[15],data[16],data[17]); //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
                    address = data[15]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2.
                }
				
                //checksum of packet does not pass
                else
                {
                    //If there was a startbyte in middle of what was a possible packet
                    if(nextStart!=0)
                    {
                        int j;
                        for(j = nextStart;j < sizeof(data);j++)
                        {
                            data[j-nextStart] = data[j];
                        }
                        i = j - nextStart;
                        nextStart = 0;
                    }
                }
            }
        
            if(c == START_BYTE && i != 0)
            {
                if(i>sizeof(data))
                    {
                        i = 0;
                        nextStart = 0;
                    }
                else if(nextStart==0)
                        nextStart = i;   
            }
		
            if(i<sizeof(data))
                data[i]=c;
            i++;
            tLastRead = millis();       
        }
    }
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
  
  while(((k<oldNextRadRx && k<sizeof(radRxBuff))||(radRxBuff[oldNextRadRx-6]==START_BYTE && !(k<oldNextRadRx && k<sizeof(radRxBuff)) && done!=1))&& ((k-lastUsed)<=sizeof(data)))
  {
    if(k<oldNextRadRx && k<sizeof(radRxBuff))
    {
      c = radRxBuff[k];
      lastUsed = k;
    }
    k++;
    if(i == sizeof(data))//If the last byte of a possible packet is reached
    {
      Serial.print(1);
      //NOTE this does not adapt to different sized data packets because sizeof(data) is defined and constant
      chk = chkSum(&data[0], sizeof(data));
      Serial.print("Generated chk: ");
      Serial.print(chk,HEX);
      Serial.print(" chk found in packet: ");
      Serial.println(data[sizeof(data)-1],HEX);
      if(data[sizeof(data)-1]==chk)//compare last byte of packet to calculated hash sum
      {
        Serial.print(2);
        i = 0;
        nextStart = 0;
        addArdTxBuff(&data[14],7);//Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
        printArray(&data[14],7);
        address = data[14]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2.
        done = 1;
        lastMeaningful = lastUsed-1;//c hasn't actually been added to data[] yet, so the last byte that we know has meaning is the byte before lastUsed. c is added near the end of this loop...
      }  
      else//checksum of packet does not pass
      {
        Serial.print(3);
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

void writeByCommand(char address)
{
    char dataOne;
    char dataTwo;
    switch(address)
    {
        case '1':
            dataOne = 'a';
            dataTwo = 'b';
            writeData(address,dataOne,dataTwo);
            break;
        case '2':
            dataOne = (char)board;
            dataTwo = 'd';
            writeData(address,dataOne,dataTwo);
            break;
        case '3':
            dataOne = 'e';
            dataTwo = 'f';
            writeData(address,dataOne,dataTwo);
            break;
        case '4':
            dataOne = 'g';
            dataTwo = 'h';
            writeData(address,dataOne,dataTwo);
            break;
        case '5':
            writeData('5','a','b');
            break;
        case '6':
            writeData('6','c','d');
            break;
        case '7':
            writeData('7','e','f');
            break;
        case '8':
            writeData('8','g','h');
            break;
		case 'c':
			//parse Serial here!
			writeData(address,motor,(int)(degrees*255.0/360.0));
			break;
        case 'e':
            dataOne = 'n';
            dataTwo = 'd';            
            writeData(address,dataOne,dataTwo);
            break;
    }  
    return;
}

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
        //delay(5);    
  }
  //Serial.println("");
}

/*--------------------------------BUFFER----------------------------------*/
/*----------------------------------ADDARDTXBUFF--------------------------------*/
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
/*----------------------------------ADDRADRXBUFF--------------------------------*/
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

uint8_t chkSum(uint8_t data[], int size) //Shouldn't use for packets larger than 260 bytes total in size, unsigned int sum may rollover. Technically should still work though...?
{
  unsigned int sum = 0;
  for(int i=3;i<(size-1);i++)sum += data[i];//add all bytes except start delimeter, length bytes, and checksum byte
  return 0xFF-(uint8_t)sum;                 //return checksum: 0xFF minus this sum ^
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
