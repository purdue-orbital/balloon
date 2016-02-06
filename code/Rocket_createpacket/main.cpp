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
#define FOOTER 6
#define POLY 0x8408



uint16_t crc16(uint8_t *data_p, unsigned short length);

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    //Local Declarations
    
    uint8_t data[7];
    data[HEADER] = '@';
    data[ADDRESS] = '1';
    data[ARG1] = '2';
    data[ARG2] = '3';
    data[FOOTER] = '?';
    
    
    int crc = crc16(data, 4);
    data[4] = (uint8_t) (crc & 0xFF);
    data[5] = (uint8_t) ((crc >> 8) & 0xFF);
  
    FILE *yo;
    yo = fopen("test.txt", "w");
    
    
    int i;
    
    for (i=0;i < (sizeof (data) /sizeof (data[0]));i++) {
        fprintf(yo, "%c",(char)data[i]);
        printf("%d\n",data[i]);
    }
           
    fclose(yo);
    return(0);
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