/********************************
*TO DO LIST
* - Finish functions by adding their data to buffer instead of using writeData command
*	- write high on digital output pin to alert master comm arduino that UI data is available
* - Create talk(). Add additional functionality:
*	- attached to interrupt on slave select pin 
*	- write data from buffer and read data from radio
* - Method for flushing rxbuffer
* - Do we interrupt prompts to display new data?
*	- Maybe interrupt prompt with new data then print last prompt again.
* - Add proper delays for SPI transfer
* - Modify talk() so it works as a slave
* - Finish setup (for example add SPI setup to setup() )
* - Confirm: Change in ATTENTION_IN or ATTENTION_OUT, if even possible in middle of talk(), will not cause a logical error
*********************************/

#define ATTENTION_IN 9
#define ATTENTION_OUT 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
typedef unsigned char  uint8_t;
volatile boolean process_it;
int display(int function, uint8_t data1, uint8_t data2);


void setup() 
{
  Serial.begin (115200);   // debugging

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  pinMode(ATTENTION_IN, INPUT);
  pinMode(ATTENTION_OUT, OUTPUT);

  digitalWrite(ATTENTION_OUT, HIGH);
  
  // get ready for an interrupt 
  nextRxBuffIndex = 0;   // buffer empty
  nextTxBuffIndex = 0;

  // now turn on interrupts
  SPI.attachInterrupt();

}

// the loop routine runs over and over again forever:
void loop() 
{
  while(Serial.available()<=0);
  char input = Serial.read();
  if(input == '%')
  {
    while(Serial.available()<=0);
    input = Serial.read();
    actOnUser(input);
  }
  else
  {
    Serial.println("Input received outside of command. To start a command enter '%' followed by the character corresponding to the command you wish to execute.");
  }

  if (digitalRead(ATTENTION_IN) == HIGH && nextRxBuffIndex > 0) {
    parseRxData  
  }
  
}


/*--------------------------------BUFFER----------------------------------*/
uint8_t txBuff[300];
uint8_t rxBuff[300];
int nextTxBuffIndex = 0;
int nextRxBuffIndex = 0;
/*--------------------------------INTERUPT----------------------------------*/
// SPI interrupt routine
ISR (SPI_STC_vect) {

  // add to buffer if room
  if (nextRxBuffIndex < sizeof(rxBuff) && digitalRead(ATTENTION_IN) == LOW) {

    rxBuff[nextRxBuffIndex++] = SPDR;
    
  }
  
    // grab byte from SPI Data Register
  if (nextTxBuffIndex > 0) {
    
    digitalWrite(ATTENTION_OUT, LOW);
    SPDR = txBuff[nextTxBuffIndex++]; 
    digitalWrite(ATTENTION_OUT, HIGH);
   
  }

}
/*----------------------------------ADDTXBUFF--------------------------------*/
void addTxBuff(char address,char arg1, char arg2)
{
  if((nextTxBuffIndex+2)<sizeof(txBuff))
  {
    txBuff[nextTxBuffIndex++] = address;
    txBuff[nextTxBuffIndex++] = arg1;
    txBuff[nextTxBuffIndex++] = arg2;
  }
  
  digitalWrite(ATTENTION_OUT,LOW);
  
  return;
}
/*----------------------------------ADDRXBUFF--------------------------------*/
void addRxBuff(char data)
{
  if(nextRxBuffIndex < sizeof(rxBuff))
    rxBuff[nextRxBuffIndex++] = data;
  return;  
}
/*----------------------------------ACT_ON_USER--------------------------------*/
void actOnUser(char address) //Previously called writeByCommand
{
    char dataOne;
    char dataTwo;
    switch(address)
    {
		case 'a':
			Serial.println("Type 'lt' to request a poll of the altitude sensor.");
			Serial.flush();
			Stream.flush();
			while(Serial.available()<=0);
			if(input == 'l')
			{
				Serial.flush();
				Stream.flush();
				while(Serial.available()<=0);
				if(input == 't')
				{
					addBuff(address,'t','l');
					Serial.println("Altitude poll request added to buffer...");
				}
        else
          Serial.println("Unexpected input, altitude poll request cancelled.");
			}
			else
				Serial.println("Unexpected input, altitude poll request cancelled.");
			break;
    case 'c':
			Serial.println("Type 'on' to request an advance of the 1st DOF position.");
			Serial.println("Type 'xx' to request the 1st DOF to stop.");
			Serial.flush();
			Stream.flush();
			while(Serial.available()<=0);
			if(input == 'o')
			{
				Serial.flush();
				Stream.flush();
				while(Serial.available()<=0);
				if(input == 'n')
				{
					addBuff(address,'o','n');
					Serial.println("Request to advance 1st DOF added to buffer...");
				}
        else
          Serial.println("Unexpected input, advance 1st DOF request cancelled.");
			}
      
      else if(input == 'x')
			{
				Serial.flush();
				Stream.flush();
				while(Serial.available()<=0);
				if(input == 'x')
				{
					addBuff(address,'x','x');
					Serial.println("Request to STOP 1st DOF added to buffer...");
				}
        else
          Serial.println("Unexpected input, STOP 1st DOF request cancelled.");
			}
      
			else
				Serial.println("Unexpected input, 1st DOF request cancelled.");
			break;
/*  case 'l':
      Serial.println("Confirm launch request by entering the capital letter Y or enter N for no launch.");
      Serial.flush();
      Stream.flush();
      while(Serial.available<=0);
      input = Serial.read();
      while(input != 'Y' || input != 'N')
        {
          Serial.println("Invalid input. Enter capital letter Y to launch or enter N to escape launch.");
          Serial.flush();
          Stream.flush();
          while(Serial.available<=0);
          input = Serial.read();
        }
      if(input == 'N') return;
      dataOne = 'Y';
      
      Serial.println("Launch? Confirm again by entering the capital letter Y, retreat request by entering N.");
      Serial.flush();
      Stream.flush();
      while(Serial.available<=0);
      input = Serial.read();
      while(input != 'Y' || input != 'N')
        {
          Serial.println("Invalid input. Confirm again by entering the capital letter Y, retreat request by entering N.");
          Serial.flush();
          Stream.flush();
          while(Serial.available<=0);
          input = Serial.read();
        }
      if(input == 'N') return;
      dataTwo = 'Y';
      writeData(address,dataOne,dataTwo);
      break;
    // --------------------------------------------MOTOR CONTROL-----------------------------------------------
    case 'c':
      unsigned int sum = 0;
      unsigned int angle = 0;
      unsigned int digit = 0;
  
      Serial.println("To escape this command type the capital letter N and hit enter. Otherwise input the angle and hit enter.");
      Serial.flush();
      Stream.flush();
      while(Serial.available<=0);
      while(Serial.available>0)
      {
        input = Serial.read();
        while((input < 48 || input > 57) && input!= 'N')
        {
          Serial.println("Invalid input. Enter digits representing requested angle in base 10 big endian or enter N to escape motor control.");
          Serial.flush();
          Stream.flush();
          while(Serial.available<=0);
          input = Serial.read();
        }
        if(input == 'N') return;
        sum += (input - 48)*pow(10,digit);//angle stored in little endian base 10
        digit++;
      }
      for(digit = 0;digit <= log10(sum),digit++)//convert angle to big endian base 10
      {
        angle += pow(10,log10(sum)-digit)*((sum/pow(10,digit))%10);
      }
      dataOne = (byte) (angle >> 8);
      dataTwo = (byte) angle;
      writeData(address,dataOne,dataTwo);
      break; */

    case 'e':
        dataOne = 'n';
        dataTwo = 'd';            
        writeData(address,dataOne,dataTwo);
        break;
    }  
    return;
}
/*--------------------------------CRC-16---------------------------------*/
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
/*--------------------------------PARSE----------------------------------*/
void parseRxData()
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
  int oldNextRx = nextRxBuffIndex;
  
  while((k<oldNextRx && k<sizeof(rxBuff))||(rxBuff[oldNextRx-6]=='@' && !(k<oldNextRx && k<sizeof(rxBuff)) && done!=1))
  {
    if(k<oldNextRx && k<sizeof(rxBuff))
    {
      c = rxBuff[k];
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
	display(data[1],data[2],data[3]);//Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2. 
        address = data[1]; //Assuming AO = 0, getting frame 0x90 and frame 0x10 was transmitted with a payload consisting of only ADDRESS, ARG1,and ARG2.
        done = 1;
        lastMeaningful = lastUsed-1;//c hasn't actually been added to data[] yet, so the last byte that we know has meaning is the byte before lastUsed. c is added near the end of this loop...
      }  
      else//checksum of packet does not pass
      {
        //Serial.print(3);
        for(int index = 0; index<sizeof(data);index++) 
          Serial.print((char) data[index]);
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
  for(k = lastMeaningful+1; k<nextRxBuffIndex && k<sizeof(rxBuff);k++)
  {
    rxBuff[k-lastMeaningful-1] = rxBuff[k];
  }
  nextRxBuffIndex = k-lastMeaningful-1;
  interrupts();
  return;
}

/*----------------------------------DISPLAY--------------------------------*/
int display(int function, uint8_t data1, uint8_t data2) {
	
	unsigned int value = 0;
	
	value = data1 << 8; 
	value += data2;
	//if ( value == 0)
	//	{ function = 6;}

	switch ( function ) {
  case 1:            // 
    printf("int function1 example temp\n");
    printf("%d C\n", value);
    break;
  case 2:            // 
    printf("int function2 example meter\n");
    printf("%d m\n", value);
    break;
  case 3:            // 
    printf("int function3\n");
    break;
  case 4:            // 
    printf("int function4\n");
    break;
  case 5:            // 
    printf("f5\n");
    break;
  case 6:            // 
    printf("value is NULL\n");
    break;
  default:            // catchall error case if wrong function number is received
    printf("Error\n");
    return 0;
    break;
  }
	
	
	






FILE *f1 = fopen("file.txt", "a");
if (f1 == NULL)
{
    printf("Error opening file!\n");
}

int i = value;
 int j = function;
fprintf(f1, "\n%d  ", j);
fprintf(f1, "%d  ", i);
//time
    struct timeval usec_time;
    time_t now = time(0);
    gettimeofday(&usec_time,NULL);
    // format time
    struct tm *current = localtime(&now);
    //printf("%d:%d:%d\n",current->tm_hour, current->tm_min, current->tm_sec);
    fprintf(f1,"%d:%d:%d\n",current->tm_hour, current->tm_min, current->tm_sec);

//char *ctime(const time_t *timer)
//fprintf(f1, ); //get time

fclose(f1);

	
	return 1;
}

