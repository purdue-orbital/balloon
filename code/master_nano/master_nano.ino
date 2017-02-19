//NOTE: After 50 days millis rolls over. Shouldn't be an issue for normal operations
#define ADDRESS 1
#define HEADER 0
#define ARG1 2
#define ARG2 3
#define CRC8_1 4
#define CRC8_2 5
#define FOOTER 6
#define PACKET_LENGTH 6
#define POLY 0x8408
#define SWITCHING_DELAY 40 

#define RX_EN 3

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

int lastcalled = 0;

void setup() {
  if(analogRead(3)>512){board = 2; tOutDelay = 1000;} else {board = 1; tOutDelay = 337;}
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(13,OUTPUT);
  extractPackets();  
  digitalWrite(13,HIGH);
  synched = 1;
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

void extractPackets()
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
    char address = 'n';
    int tLastRead;

    while(address!='e')
    {
        if(tLastRead - millis() > tOutDelay){ writeByCommand('e'); digitalWrite(13,LOW); synched = 0;} //Timeout.. haven't received a packet in a while
        
        if(Serial1.available()>0)
        {
            if(synched == 0){digitalWrite(13,HIGH); synched = 1;}
            c = Serial1.read();
            if(i == sizeof(data))//If the last byte of a possible packet is reached
            {
                crc = crc16(data, 4);
            
                //if checksum of the entire possible packet is passed
                if(data[4]==(uint8_t) (crc & 0xFF)&& data[5] == (uint8_t) ((crc >> 8) & 0xFF))
                {
                    done = 1;
                }
                //checksum of packet does not pass
                else
                {
                    done = 0;
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
        
            if(c=='@')
            {
                if(!done)
                {
                    if(i>=sizeof(data))
                    {
                        i = 0;
                        nextStart = 0;
                    }
                    else if(nextStart==0)
                        nextStart = i;   
                }
                else
                {
                    i = 0;
                    nextStart = 0;
                    actOnPacket(data[1],data[2],data[3]);
                    done = 0;
                    address = data[1];
                }
            }   
            
            if(!done)
            {
                if(i<sizeof(data))
                  data[i]=c;
                i++;
            } 
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
      uint8_t data[6];
    
    data[HEADER] = '@';
    data[ADDRESS] = address;
    data[ARG1] = dataOne;
    data[ARG2] = dataTwo;
    
    int crc = crc16(data, 4);
    data[4] = (uint8_t) (crc & 0xFF);
    data[5] = (uint8_t) ((crc >> 8) & 0xFF);
    
    /* Serial1.print("\nYo this is what I'm sending back to master: "); */
    int i;
    for (i=0;i < (sizeof (data) /sizeof (data[0]));i++) 
    {
          Serial1.print((char)data[i]);
          /* Serial1.print((char)data[i]);    */
      //fprintf(yo, "%c",(char)data[i]);
        //printf("%d\n",data[i]);
    }
    /* Serial1.println(""); */
    if(address=='e')
    {
      Serial1.print('@');
    }

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
