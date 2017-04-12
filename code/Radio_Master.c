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
#define DEST_NET_1 13
#define DEST_NET_2 14
#define RADIUS 15
#define OPTIONS 16
#define ADDRESS 17
#define ARG1 18
#define ARG2 19
#define CHK_SUM 20

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
char rxBuff [100];
char txBuff [100];
volatile byte nextRxBuffIndex;
volatile byte nextTxBuffIndex;

int lastcalled = 0;

void setup() {
  if(analogRead(3)>512){board = 2; tOutDelay = 1000;} else {board = 1; tOutDelay = 337;}
  Serial.begin (115200);   // debugging
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  pinMode(ATTENTION_IN, INPUT);
  pinMode(ATTENTION_OUT, OUTPUT);

  digitalWrite(ATTENTION_OUT, HIGH);

  nextRxBuffIndex = 0;   // buffer empty
  nextTxBuffIndex = 0;

}

void loop(){
  if(board == 2)
  {
     if(millis()-lastcalled > 100 ){ writeByCommand('6'); lastcalled = millis();}
       /*
   * This is where user/automated requests (from this node) to send a packet should be processed. 
   */
  }
  else if(board == 1)
  {
      /*
   * This is where user/automated requests (from this node) to send a packet should be processed. 
   */
  }
  writeByCommand('e');
  extractPackets();
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

void extractRadioPackets()
{
    uint8_t data[19]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
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

void writeData(char address, char dataOne, char dataTwo)
{
    uint8_t data[21];
    
    data[START_DELIMITER] = START_BYTE;
	data[LENGTH_1] = 0x00; //MSB. See next line
	data[LENGTH_2] = 0x11; //LSB. Length 17 Bytes in packet EXCLUDING start delimiter,length bytes, and check sum.
	data[FRAME_TYPE] = 0x10; //Transmit Request Frame
	data[FRAME_ID] = 0x00; //Each packet does not have an individualized ID, confirmation packet will not be sent back
	data[DEST_1] = DEST_1_BYTE;
	data[DEST_2] = DEST_2_BYTE;
	data[DEST_3] = DEST_3_BYTE;
	data[DEST_4] = DEST_4_BYTE;
	data[DEST_5] = DEST_5_BYTE;
	data[DEST_6] = DEST_6_BYTE;
	data[DEST_7] = DEST_7_BYTE;
	data[DEST_8] = DEST_8_BYTE;
	data[DEST_NET_1] = 0xFF; //Not sure why, user guide said to do this. Maybe only when not broadcasting?
	data[DEST_NET_2] = 0xFE; //Same^
	data[RADIUS] = 0x00; //Set to max hops value. Not sure if this matters when not broadcasting...
	data[OPTIONS] = 0x00; //Not sure if there are any other options for this frame.
    data[ADDRESS] = address;
    data[ARG1] = dataOne;
    data[ARG2] = dataTwo;
    data[CHK_SUM] = chkSum(data,21);
	
    /* Serial1.print("\nYo this is what I'm sending back to master: "); */
    int i;
    for (i=0;i < sizeof(data);i++) 
    {
          SPI.transfer(data[i]);
          /* Serial1.print((char)data[i]);    */
      //fprintf(yo, "%c",(char)data[i]);
        //printf("%d\n",data[i]);
    }
    /* Serial1.println(""); */
    /*if(address=='e') //Previously implemented in wired protocol, however after adapting code to match the radio protocol this should be artificially added, locally to the data stream on the receiving arduino when the radio appears to no longer have data available. 
    {
      Serial1.print('@');
    }*/

}

void actOnPacket(char address, char dataOne, char dataTwo)
{
  /* wdt_reset(); */
    switch(address)
    {
        case '1':
            //printf("Sensor 1 data: %d mph, %d degrees\n",(int)dataOne,(int)dataTwo);
            /* Serial1.print("Sensor 1 data: "); */
            /* Serial1.print((int)dataOne); */
            /* Serial1.print(" mph, "); */
            /* Serial1.print((int)dataTwo); */
            /* Serial1.print(" degrees\n"); */
            break;
        case '2':
            Serial.print("Identity of remote node: board_");
            Serial.print((int)dataOne);
            Serial.print(" I'm board_");
            Serial.println(board);
            /* Serial1.print("Sensor 2 data: "); */
            /* Serial1.print((int)dataOne); */
            /* Serial1.print(" K\n"); */
            break;
        case '3':
            //printf("Sensor 3 data: %d N\n",(int)dataOne);
            /* Serial1.print("Sensor 3 data: "); */
            /* Serial1.print((int)dataOne); */
            /* Serial1.print("N\n"); */
            break;
        case '4':
            //printf("Sensor 4 data: %d m\n",(int) dataOne);
            /* Serial1.print("Sensor 4 data: "); */
            /* Serial1.print((int)dataOne); */
            /* Serial1.print(" m\n"); */
            break;
        case '5':
        case '6':
        case '7':
        case '8':
            addActOnBuff(address-4);
            break;
		case 'c':
			Serial1.print("Request to move motor %c",dataOne);
			Serial1.print(" to ");
			Serial1.println(((int) dataTwo)*360/255);
			break;
        case 'e':
            delay(SWITCHING_DELAY);
            flushActOnBuff();
            break;
    }
}

void addActOnBuff(char address)
{
  /* Serial1.println("\nADDED SOMETHING TO BUFFER"); */
  if(nextActBuffIndex<sizeof(actOnBuff))
    actOnBuff[nextActBuffIndex++] = address;
  return;
}

void flushActOnBuff()
{
  for(int i=0; i<nextActBuffIndex&&i<sizeof(actOnBuff);i++)
  {
    writeByCommand(actOnBuff[i]);
  }
  nextActBuffIndex = 0;
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