#include <SPI.h>
#define ATTENTION_IN 2
#define ATTENTION_OUT 3

void setup (void)
{
  Serial.begin (115200);   // debugging
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  pinMode(ATTENTION_IN, INPUT);
  pinMode(ATTENTION_OUT, OUTPUT);

  digitalWrite(ATTENTION_OUT, HIGH);
  
}  // end of setup


void loop (void)
{

  char c;

  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  // send test string
  for (const char * p = "NOPE!\n" ; c = *p; p++)
  {
    SPI.transfer (c);
    delay(1);
  }

  int i = 0;
  char d[30];
  // send test string
  digitalWrite(ATTENTION_OUT, LOW);
  for (const char * p = "Hello, world!\n" ; c = *p; p++)
  {
    if (digitalRead(ATTENTION_IN) == LOW)
    {
      d[i] = SPI.transfer (c);
      i++;
    } 
    else 
    {
      SPI.transfer (c);
    }

    delay(1);
  }

  Serial.println(d);

  // send test string
  digitalWrite(ATTENTION_OUT, HIGH);
  for (const char * p = "NOPE 2!\n" ; c = *p; p++)
  {
    SPI.transfer (c);
    delay(1);
  }

  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (1000);  // 1 seconds delay 
}  // end of loop
