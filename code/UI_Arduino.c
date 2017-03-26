/********************************
*TO DO LIST
* - Finish functions by adding their data to buffer instead of using writeData command
*	- write high on digital output pin to alert master comm arduino that UI data is available
* - Rename flushBuff to msgRadio. Add additional functionality:
*	- attached to interrupt on slave select pin 
*	- write data from buffer and read data from radio
* - Create display buffer
* - Create display function with switch/case structure
* - Do we interrupt prompts to display new data?
*	- Maybe interrupt prompt with new data then print last prompt again.
* 
*
*********************************/

#define IGNITION_IN 2
#define IGNITION_OUT 8

void setup() {
pinMode(IGNITION_IN,INPUT);
pinMode(IGNITION_OUT,OUTPUT);
attachInterrupt(digitalPinToInterrupt(IGNITION_IN),ignite,RISING);

Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
	parse();
}

void ignite() {
  digitalWrite(IGNITION_OUT,HIGH);
  detachInterrupt(digitalPinToInterrupt(IGNITION_IN));
}

/*--------------------------------BUFFER----------------------------------*/
uint8_t buff[300];
int nextBuffIndex = 0;

void addBuff(char address,char arg1, char arg2)
{
  if((nextBuffIndex+2)<sizeof(buff))
  {
    buff[nextBuffIndex++] = address;
    buff[nextBuffIndex++] = arg1;
    buff[nextBuffIndex++] = arg2;
  }
  return;
}

void flushBuff()
{
  for(int i=0; (i+2)<nextBuffIndex&&i<sizeof(buff);i+=3)
  {
    SPI.transfer('@');
    SPI.transfer(buff[i]);
    SPI.transfer(buff[i+1]);
    SPI.transfer(buff[i+2]);
    SPI.transfer({buff[]})
  }
  nextBuffIndex = 0;
  return;
}
/*------------------------------------------------------------------------*/
/*----------------------------------Write-By-Command--------------------------------*/
void writeByCommand(char address)
{
    char dataOne;
    char dataTwo;
    switch(address)
    {
        case 'l':
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
            break;

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
void parse()
{
  String inputBuff = "";
  uint8_t tempAd;
  uint8_t tempArg1;
  uint8_t tempArg2;
  int oldIndx = 0;
  int indx = 0;
  while(Serial.available()>0)
  {
    inputBuff = inputBuff + Serial.read();
  }
  oldIndx = inputBuff.indexOf('$');
  indx = inputBuff.indexOf('%');
  tempAd = (uint8_t)inputBuff.substring(oldIndx+1,indx);
  
  inputBuff = inputBuff.substring(indx+1);
  indx = inputBuff.indexOf('%');
  tempArg1 = (uint8_t)inputBuff.substring(0,indx);
  
  inputBuff = inputBuff.substring(indx+1);
  oldIndx = inputBuff.indexOf('$');
  tempArg2 = (uint8_t)inputBuff.substring(0,oldIndx);
}
