// DRA818V_3 

// set parameters for DRA818V 
int bw = 1; // bandwith in KHz ( 0= 12.5KHz or 1= 25KHz ) 
float ftx = 433.2250; // tx frequency in MHz (134.0000 - 174.0000) 
float frx = 433.2250; // rx frequency in MHz (134.0000 - 174.0000) 
String tx_ctcss = "0000"; // ctcss frequency ( 0000 - 0038 ); 0000 = "no CTCSS" 
String rx_ctcss = "0000"; // ctcss frequency ( 0000 - 0038 ); 0000 = "no CTCSS" 
int squ = 0; // squelch level ( 0 - 8 ); 0 = "open" 

void setup() 
{ 
Serial1.begin(9600); // open serial at 9600 bps 
delay(10); 
Serial1.print("AT+DMOSETGROUP="); // begin message 
Serial1.print(bw,1); 
Serial1.print(","); 
Serial1.print(ftx,4); 
Serial1.print(","); 
Serial1.print(frx,4); 
Serial1.print(","); 
Serial1.print(tx_ctcss); 
Serial1.print(","); 
Serial1.print(squ); 
Serial1.print(","); 
Serial1.println(rx_ctcss); 
} 

void loop() 
{ 
}
