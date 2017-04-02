#include <SPI.h>
#define ATTENTION_IN 34
#define ATTENTION_OUT 35

/*void setup (void)
{
  
  digitalWrite(SS, HIGH);  // ensure SS stays high for now
  digitalWrite(ATTENTION_OUT, HIGH);

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  
  
}  // end of setup


void loop (void)
{

  char c;

  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  // send test string
  for (const char * p = "Not me!\n" ; c = *p; p++)
  {
    delay(10);
    SPI.transfer (c);
  }

  digitalWrite(ATTENTION_OUT, LOW);

 // send test string
  for (const char * p = "Definitely me!\n" ; c = *p; p++)
  {
    delay(10);
    SPI.transfer (c);
  }

  digitalWrite(ATTENTION_OUT, HIGH);

  // send test string
  for (const char * p = "Definitely me!\n" ; c = *p; p++)
  {
    delay(10);
    SPI.transfer (c);
  }
  
  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (1000);  // 1 seconds delay 
}  // end of loop

#include <SPI.h>*/

void setup (void)
{

  digitalWrite(SS, HIGH);  // ensure SS stays high for now
  digitalWrite(ATTENTION_OUT, HIGH);

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  
  // digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  // SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  
}  // end of setup


void loop (void)
{

  char c;

  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10
  
  // send test string
  for (const char * p = "Not this one!\n" ; c = *p; p++)
    SPI.transfer (c);

  digitalWrite(ATTENTION_OUT, LOW);

  for (const char * p = "Yup, me!\n" ; c = *p; p++)
    SPI.transfer (c);

  digitalWrite(ATTENTION_OUT, HIGH);

  for (const char * p = "Not this either!\n" ; c = *p; p++)
    SPI.transfer (c);

  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (1000);  // 1 seconds delay 
}  // end of loop
