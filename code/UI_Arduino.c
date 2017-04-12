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
  currentTxBuffIndex = 0;

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
	char address = 'n';
	int tLastRead;

  while(i<nextRxBuffIndex && i<sizeof(rxBuff))
  { 
    c = rxBuff[i];
    if(i == sizeof(data))//If the last byte of a possible packet is reached
    {
      crc = crc16(&data[1], 3);
  
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
        display(data[1],data[2],data[3]);
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
  nextRxBuffIndex = 0;
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

