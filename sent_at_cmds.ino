
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#include <SoftwareSerial.h>
SoftwareSerial BT1(3, 2); // RX | TX

void setup()
  {  
     Serial.begin(9600);
     BT1.begin(115200);
  }

void loop()
{

     if (BT1.available())
     { 
         char c = BT1.read() ;
         Serial.print(c);
     }
     if (Serial.available())
     { 
         char c = Serial.read();
         BT1.print(c);
     }
}



