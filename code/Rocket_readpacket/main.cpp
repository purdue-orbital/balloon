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
#define packetlength 6
#define POLY 0x8408



uint16_t crc16(uint8_t *data_p, unsigned short length);

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    //Local Declarations
    
    uint8_t data[7];
    
    FILE *file = fopen("test.txt", "r");
    char *code;
    size_t n = 0;
    int c;
    
    if (file == NULL)
    {
        return NULL; //could not open file
    }
    code = (char*) malloc(1000);
    
    int i;
    bool iscorrupt;
    while ((c = fgetc(file)) != EOF)
    {
        if(i>packetlength){
            printf("DATA CORRUPT\n");
            iscorrupt;
            printf("CORRUPT: %d: %d\n",i, data[i]);
            i++;
        }
        else{
            if(c == '@'){
                printf("START\n");
                iscorrupt = false;
                i = 0;
            }
            
            else if(!iscorrupt){
            data[i] = (uint8_t)c;
            printf("%d: %d\n",i, data[i]);
            i++;
            //printf("%d\n", (int)c );
            code[n++] = (char) c;
            }
            
        }
        
        
    }
    
    // don't forget to terminate with the null character
    code[n] = '\0';  
    
    printf("%c", code);
    
        
    
    //printf("%d", test);   
            
    
//    int crc = crc16(data, 4);
//    data[4] = (uint8_t) (crc & 0xFF);
//    data[5] = (uint8_t) ((crc >> 8) & 0xFF);
  
    
    
    
//    int i;
//    
//    for (i=0;i < (sizeof (data) /sizeof (data[0]));i++) {
//        fprintf(yo, "%d",data[i]);
//        printf("%d\n",data[i]);
//    }
           
    
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