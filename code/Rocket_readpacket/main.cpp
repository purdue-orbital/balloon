/* 
 * File:   main.cpp
 * Author: Rahul
 *
 * Created on February 5, 2016, 7:01 PM
 */

#include <cstdlib>
#include <stdio.h>
#include<stdlib.h>



#define ADDRESS 0
#define HEADER 1
#define ARG1 2
#define ARG2 3
#define CRC8_1 4
#define CRC8_2 5
#define FOOTER 6
#define PACKET_LENGTH 6
#define POLY 0x8408



uint16_t crc16(uint8_t *data_p, unsigned short length);
void extractPackets();
void actOnPacket(char,char,char);

using namespace std;

FILE *file = fopen("C:\\Users\\Maureen Cohen Laurus\\Desktop\\Rocket Code 1\\test.txt", "r+");

int main(int argc, char** argv) {
    if (file == NULL)
    {
        return NULL; //could not open file
    }
    extractPackets();
    /*uint8_t data[6];
    
    char *code;
    size_t n = 0;
    int c;   
    int crc;
    int i = 0;
    bool isCorrupt;
    bool isPacket;
    bool done=0;
    int nextStart = 0;
    char address = 'n';
    
    code = (char*) malloc(1000);
    */
    /*
    while(address!='e')
    {
    while ((c = fgetc(file)) != EOF)
    {
        //printf("A\n");
        if(i == sizeof(data))//If the last byte of a possible packet is reached
        {
            //printf("F\n");
            //printf("i: %d sizof(data): %d\n",i,sizeof(data));
            crc = crc16(data, 4);
            
            if(data[4]==(uint8_t) (crc & 0xFF)&& data[5] == (uint8_t) ((crc >> 8) & 0xFF))//if checksum of the entire possible packet is passed
            {
                //printf("G\n");
                done = 1;
            }
            else//checksum of packet does not pass
            {
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
                //printf("SHIFTED ARRAY____________________\n");
                //for(int k = 0; k < sizeof(data);k++) 
                //printf("%c",data[k]);
                //printf("i: %d",i);
                //printf("END\n");
                }
            }
        }
        
        if(c=='@')
        {
            //printf("B\n");
            if(!done)
            {
                if(i>=sizeof(data))
                {
                    i = 0;
                    nextStart = 0;
                }
                else if(nextStart==0)
                    nextStart = i;
                //printf("C\n");   
            }
            else
            {
                //printf("D\n");
                i = 0;
                nextStart = 0;
                //isPacket = 1;
                for(int j = 1; j < sizeof(data)-2;j++) 
                    printf("%c",data[j]);
                //printf("%c",data[1]);
                printf("END\n");
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
    }*/
    return(0);
}

void extractPackets()
{
    uint8_t data[6];
    //char *code;
    size_t n = 0;
    int c;   
    int crc;
    int i = 0;
    bool isCorrupt;
    bool isPacket;
    bool done=0;
    int nextStart = 0;
    char address = 'n';
    while(address!='e')
    {
        while ((c = fgetc(file)) != EOF)
        {
            //printf("A\n");
            if(i == sizeof(data))//If the last byte of a possible packet is reached
            {
                //printf("F\n");
                //printf("i: %d sizof(data): %d\n",i,sizeof(data));
                crc = crc16(data, 4);
            
                if(data[4]==(uint8_t) (crc & 0xFF)&& data[5] == (uint8_t) ((crc >> 8) & 0xFF))//if checksum of the entire possible packet is passed
                {
                    //printf("G\n");
                    done = 1;
                }
                else//checksum of packet does not pass
                {
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
                    //printf("SHIFTED ARRAY____________________\n");
                    /*for(int k = 0; k < sizeof(data);k++) 
                    printf("%c",data[k]);
                    printf("i: %d",i);
                    printf("END\n");*/
                    }
                }
            }
        
            if(c=='@')
            {
                //printf("B\n");
                if(!done)
                {
                    if(i>=sizeof(data))
                    {
                        i = 0;
                        nextStart = 0;
                    }
                    else if(nextStart==0)
                        nextStart = i;
                    //printf("C\n");   
                }
                else
                {
                    //printf("D\n");
                    i = 0;
                    nextStart = 0;
                    //isPacket = 1;
                    actOnPacket(data[1],data[2],data[3]);
                    //for(int j = 1; j < sizeof(data)-2;j++) 
                    //    printf("%c",data[j]);
                    //printf("%c",data[1]);
                    //printf("END\n");
                    done = 0;
                    address = data[1];
                }
            }   
            
            /*if(c == '@'&& i > sizeof(data)-1){
                int crc = crc16(data, 4);
                printf("\nSTART\n");
                if(data[4]==(uint8_t) (crc & 0xFF)&& data[5] == (uint8_t) ((crc >> 8) & 0xFF))
                {
                    for(int j = 1; j < sizeof(data)-2;j++) 
                    printf("%c",data[j]);
                    printf("END\n");
                }
                printf("CRC: %d %d",(uint8_t) (crc16(data,4) & 0xFF)-'0', (uint8_t) ((crc16(data,4) >> 8) & 0xFF)-'0');
                isPacket = 1;
                i = 0;
                }*/
            if(!done)
            {
                //printf("E\n");
                //if(i<sizeof(data))data[i] = c; IS the check redundant if it isn't done?
                //printf("%d: %d\n",i,(c-'0'));
                if(i<sizeof(data))data[i]=c;
                i++;
            
            }        
        }
    }
    return;
}

void actOnPacket(char address, char dataOne, char dataTwo)
{
    switch(address)
    {
        case '1':
            printf("Sensor 1 data: %d mph, %d degrees\n",(int)dataOne,(int)dataTwo);
            break;
        case '2':
            printf("Sensor 2 data: %d K\n",(int)dataOne);
            break;
        case '3':
            printf("Sensor 3 data: %d N\n",(int)dataOne);
            break;
        case '4':
            printf("Sensor 4 data: %d m\n",(int) dataOne);
            break;
    }
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