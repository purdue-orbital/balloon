#include <util/crc16.h>

void setup() {
  // put your setup code here, to run once:
struct DataPacket
{
 uint8_t Altitude;
 uint8_t Temp;
 uint8_t IMU;
 uint8_t GPS;
 uint8_t Pressure; 
 uint16_t CRC;
}


}
void loop() {


DataPacket test
  {
    test.Altitude = 1;
    test.Temp = 1;
    test.=IMU = 1;
    test.GPS = 1;
    test.Pressure = 1;
    test.CRC = MakeCRC(&test);
  }
  
  // put your main code here, to run repeatedly:
  tone(8, 1200, 1000);
  delay(1000);
  noTone(8);
  tone(8,1000, 1000);
  delay(1000);
  noTone(8);
  
}
