/* 
 * File:   main.cpp
 * Author: Rahul
 *
 * Created on February 5, 2016, 7:01 PM
 */

#include <cstdlib>
#include <stdio.h>

#define ADDRESS 1
#define HEADER 0
#define ARG1 2
#define ARG2 3
#define CRC8_1 4
#define CRC8_2 5
//#define FOOTER 6
#define POLY 0x8408



uint16_t crc16(uint8_t *data_p, unsigned short length);
void writeData(char);

using namespace std;

/*
 * 
 */
FILE *yo;
int main(int argc, char** argv) {
    yo = 
    yo = fopen("C:\\Users\\Maureen Cohen Laurus\\Desktop\\Rocket Code 1\\test.txt", "w");
    for(int j = 0; j<1000;j++)
    {
        for(int i = 1;i<5;i++)
            writeData((char)(i+48));
    }
    writeData('e');
    writeData('e');
    fclose(yo);
    /*//Local Declarations
    
    uint8_t data[6];
    //data[HEADER] = '@';
    //data[ADDRESS] = '1';
    //data[ARG1] = '2';
    //data[ARG2] = '3';
    //data[FOOTER] = '?';
    
    
    int crc = crc16(data, 4);
    data[4] = (uint8_t) (crc & 0xFF);
    data[5] = (uint8_t) ((crc >> 8) & 0xFF);
    printf("%d, %d", data[4], data[5]);
    FILE *yo;
    yo = fopen("test.txt", "w");
    
    
    int i;
    for(int j = 0; j<100;j++)//printing test packets
    {
        data[HEADER] = '@';
        data[ADDRESS] = (char)('1'+3*j);
        data[ARG1] = (char)('2'+3*j);
        data[ARG2] = (char)('3'+3*j);
        int crc = crc16(data, 4);
        data[4] = (uint8_t) (crc & 0xFF);
        data[5] = (uint8_t) ((crc >> 8) & 0xFF);
        for (i=0;i < (sizeof (data) /sizeof (data[0]));i++) {
            fprintf(yo, "%c",(char)data[i]);
            printf("%d\n",data[i]);
        }
    }
    
    fclose(yo);*/
    return(0);
}


//char* location = "C:\\Users\\Maureen Cohen Laurus\\Desktop\\Rocket Code 1\\test.txt";

void writeData(char address)
{
    char dataOne;
    char dataTwo;
    switch(address)
    {
        case '1':
            dataOne = 'a';
            dataTwo = 'b';
            break;
        case '2':
            dataOne = 'c';
            dataTwo = 'd';
            break;
        case '3':
            dataOne = 'e';
            dataTwo = 'f';
            break;
        case '4':
            dataOne = 'g';
            dataTwo = 'h';
            break;
        case 'e':
            dataOne = 'n';
            dataTwo = 'd';
            break;
    }
    uint8_t data[6];
    
    data[HEADER] = '@';
    data[ADDRESS] = address;
    data[ARG1] = dataOne;
    data[ARG2] = dataTwo;
    
    int crc = crc16(data, 4);
    data[4] = (uint8_t) (crc & 0xFF);
    data[5] = (uint8_t) ((crc >> 8) & 0xFF);
    
    int i;
    for (i=0;i < (sizeof (data) /sizeof (data[0]));i++) {
        fprintf(yo, "%c",(char)data[i]);
        //printf("%d\n",data[i]);
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