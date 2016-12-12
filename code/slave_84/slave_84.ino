/* #include<avr/wdt.h> */
#include <SoftwareSerial.h>

#define ADDRESS 2
#define HEADER 0
#define ARG1 2
#define ARG2 3
#define CRC8_1 4
#define CRC8_2 5
#define FOOTER 6
#define PACKET_LENGTH 6
#define POLY 0x8408
#define SWITCHING_DELAY 40

//----- Pin Assignments -----
#define RELAY 0 
#define RX 4
#define TX 3
#define RX_EN 2 


uint16_t crc16(uint8_t *data_p, unsigned short length);
void extractPackets();
void actOnPacket(char,char,char);
/* void addActOnBuff(char); */
void flushActOnBuff(void);
void rx_enable(void);
void tx_enable(void);
void blink_relay(void);

using namespace std;
char actOnBuff[300];
int nextActBuffIndex = 0;

SoftwareSerial softSerial(RX,TX);

//int eEPromAddy = 0;
void setup() {
    pinMode(RELAY,OUTPUT);
    // test RELAY
    /* while(1){ */

    /*   digitalWrite(RELAY,HIGH); */
    /*   delay(100); */
    /*   digitalWrite(RELAY,LOW); */
    /*   delay(100); */
    /* } */

    /* Serial.begin(9600); */
    softSerial.begin(600);
    /* wdt_enable(WDTO_500MS); */
    /*if (file == NULL)
    {
        return NULL; //could not open file
    }*/
    pinMode(RX_EN,OUTPUT);

    rx_enable();

}
void loop(){
  
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
    
    while(address!='e')
    {
      /* wdt_reset(); */
        if(softSerial.available()>0)
        {
            c = softSerial.read();
            /* Serial.print(c); */
            if(i == sizeof(data))//If the last byte of a possible packet is reached
            {
                crc = crc16(data, 4);
            
                if(data[4]==(uint8_t) (crc & 0xFF)&& data[5] == (uint8_t) ((crc >> 8) & 0xFF))//if checksum of the entire possible packet is passed
                {
                    done = 1;
                }
                else//checksum of packet does not pass
                {
                    /* Serial.print("\nCRC fail: ["); */
                    /* Serial.print(data[4]); */
                    /* Serial.print("]["); */
                    /* Serial.print(data[5]); */
                    /* Serial.println("]"); */
                    /* Serial.print("Should have been: ["); */
                    /* Serial.print((uint8_t) (crc & 0xFF)); */
                    /* Serial.print("]["); */
                    /* Serial.print((uint8_t) ((crc >> 8) & 0xFF)); */
                    /* Serial.println("]"); */
                    done = 0;
                    if(nextStart!=0)//If there was a startbyte in middle of what was a possible packet
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
                    /*for(int i=0;i<6;i++)
                    {
                      writeToStorage(data[i]);
                    }*/
                    /* Serial.print("\nCalling actOnPacket: "); */
                    /* Serial.print((char)data[1]); */
                    /* Serial.print(" "); */
                    /* Serial.print((char)data[2]); */
                    /* Serial.print(" "); */
                    /* Serial.println((char)data[3]); */
                    actOnPacket(data[1],data[2],data[3]);
                    done = 0;
                    address = data[1];
                }
            }   
            
            if(!done)
            {
                if(i<sizeof(data))data[i]=c;
                i++;
            }        
        }
    }
    return;
}

/*void writeToStorage(char c)
{
  if(eEPromAddy<200)
    EEPROM.write(eEPromAddy++, c);
}
*/

/* void writeByCommand(char address) */
/* { */
/*     char dataOne; */
/*     char dataTwo; */
/*     switch(address) */
/*     { */
/*         case '1': */
/*             dataOne = 'a'; */
/*             dataTwo = 'b'; */
/*             writeData(address,dataOne,dataTwo); */
/*             break; */
/*         case '2': */
/*             dataOne = 'c'; */
/*             dataTwo = 'd'; */
/*             writeData(address,dataOne,dataTwo); */
/*             break; */
/*         case '3': */
/*             dataOne = 'e'; */
/*             dataTwo = 'f'; */
/*             writeData(address,dataOne,dataTwo); */
/*             break; */
/*         case '4': */
/*             dataOne = 'g'; */
/*             dataTwo = 'h'; */
/*             writeData(address,dataOne,dataTwo); */
/*             break; */
/*         case '5': */
/*             writeData('1','a','b'); */
/*             break; */
/*         case '6': */
/*             writeData('2','c','d'); */
/*             break; */
/*         case '7': */
/*             writeData('3','e','f'); */
/*             break; */
/*         case '8': */
/*             writeData('4','g','h'); */
/*             break; */
/*         case 'e': */
/*             dataOne = 'n'; */
/*             dataTwo = 'd'; */
/*             writeData(address,dataOne,dataTwo); */
/*             delay(SWITCHING_DELAY); */
/*             extractPackets(); */
/*             break; */
/*     } */
/*     //writeData('e','n','d');     */
/*     return; */
/* } */

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
    
    int i;
    /* Serial.print("\nYo this is what I'm sending back to master: "); */
    for (i=0;i < (sizeof (data) /sizeof (data[0]));i++) 
    {
          softSerial.print((char)data[i]);
          /* Serial.print((char)data[i]);   */
      //fprintf(yo, "%c",(char)data[i]);
        //printf("%d\n",data[i]);
    }
    /* Serial.println(""); */
    if(address == 'e')
    {
      softSerial.print('@');
    }
}

void actOnPacket(char address, char dataOne, char dataTwo)
{
  /* wdt_reset(); */
    switch(address)
    {
        /* case '1': */
        /*     break; */
        case '2':
          blink_relay();
            //printf("Sensor 2 data: %d K\n",(int)dataOne);
            //printf("Sensor 1 data: %d mph, %d degrees\n",(int)dataOne,(int)dataTwo);
            break;
        /* case '3': */
        /*     //printf("Sensor 3 data: %d N\n",(int)dataOne); */
        /*     break; */
        /* case '4': */
        /*     //printf("Sensor 4 data: %d m\n",(int) dataOne); */
        /*     break; */
        /* case '5': */
        /*     addActOnBuff('1'); */
        /*     break; */
        /* case '6': */
        /*     addActOnBuff('2'); */
        /*     break; */
        /* case '7': */
        /*     addActOnBuff('3'); */
        /*     break; */
        /* case '8': */
        /*     addActOnBuff('4'); */
        /*     break; */
        /* case 'e': */
        /*     delay(SWITCHING_DELAY); */
        /*     flushActOnBuff(); */
        /*     break; */
    }
}

/* void addActOnBuff(char address) */
/* { */
/*   /\* Serial.println("\nADDED SOMETHING TO BUFFER"); *\/ */
/*   if(nextActBuffIndex<sizeof(actOnBuff)) */
/*   { */
/*     actOnBuff[nextActBuffIndex] = address; */
/*     nextActBuffIndex++; */
/*   } */
/*   /\* Serial.print("End of addActOnBuff nextActBuff = "); *\/ */
/*   /\* Serial.println(nextActBuffIndex); *\/ */
/*   return; */
/* } */

/* void flushActOnBuff() */
/* { */
/*   int i=0; */
/*   /\* Serial.print("Flush bool statement: "); *\/ */
/*   /\* Serial.println(i<nextActBuffIndex&&i<sizeof(actOnBuff)); *\/ */
/*   /\* Serial.print("Before flush nextActBuff = "); *\/ */
/*   /\* Serial.println(nextActBuffIndex); *\/ */
/*   for(i=0; i<nextActBuffIndex&&i<sizeof(actOnBuff);i++) */
/*   { */
/*     /\* writeByCommand(actOnBuff[i]); *\/ */
/*     /\* Serial.print("i: "); *\/ */
/*     /\* Serial.println(i); *\/ */
/*     /\* Serial.print("Flushed to writeByCommand: "); *\/ */
/*     /\* Serial.println((char) actOnBuff[i]); *\/ */
/*     /\* wdt_reset(); *\/ */
/*   } */
/*   nextActBuffIndex = 0; */
/*   /\* writeByCommand('e');//send packet signifying end  *\/ */
/*   /\* Serial.print("After flush nextActBuff = "); *\/ */
/*   /\* Serial.println(nextActBuffIndex); *\/ */
/*   return; */
/* } */

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

void tx_enable(){
  digitalWrite(RX_EN,LOW);
  delay(SWITCHING_DELAY);
}

void rx_enable(){
  digitalWrite(RX_EN,HIGH);
  delay(SWITCHING_DELAY);
}

void blink_relay(){
  digitalWrite(RELAY,HIGH);
  delay(100);
  digitalWrite(RELAY,LOW);
  delay(100);
}
