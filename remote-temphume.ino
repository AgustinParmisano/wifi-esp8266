#include <SoftwareSerial.h>
#define DEBUG true

#include <DHT11.h>

SoftwareSerial esp8266(3, 2); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
// This means that you need to connect the TX line from the esp to the Arduino's pin 2
// and the RX line from the esp to the Arduino's pin 3

int estado = 0  ;
int inputpin =  7;
DHT11 dht11(inputpin);



//int outputpin = 0;     

void setup()
{
  Serial.begin(9600);
  esp8266.begin(19200); // your esp's baud rate might be different

  sendData("AT+RST\r\n", 2000, DEBUG); // reset module
  sendData("AT+CWMODE=3\r\n", 1000, DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80
  sendData("AT+CWJAP=\"FBI\",\"teoputo021188\"\r\n", 3000, DEBUG); // turn on server on port 80

  //LEDS
  pinMode(13, OUTPUT);
  pinMode(inputpin, INPUT);

  //pinMode(outputpin, OUTPUT);
  
}


void loop()
{
  char c;


       int err;             

       float temp, hum;
       Serial.print("READ: " + String(dht11.read(hum, temp)));
       delay(1000);
       if((err = dht11.read(hum, temp)) == 0)    // Si devuelve 0 es que ha leido bien
          {
             Serial.print("Temperatura: ");
             Serial.print(temp);
             Serial.print(" Humedad: ");
             Serial.print(hum);
             Serial.println();
          }
       else
          {
             Serial.println();
             Serial.print("Error Num :");
             Serial.print(err);
             Serial.println();
          }
  
  if (esp8266.available()) // check if the esp is sending a message
  {

      delay(1000);

      int connectionId = esp8266.read() - 48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48

      String webpage;
      String parametro = readData(500, DEBUG);

      Serial.print("A");

      if (parametro.indexOf("apagar") > 0) {
        estado = 0;  
      }

      if (parametro.indexOf("prender") > 0) {
        estado = 1;
      }
      
      webpage = "<h1><a href='./apagar'>Apagar</a> Humedad: " + String(hum) + "%. Temperatura: " + String(temp) + " C </h1>";

      if (estado == 1)
      {
        webpage = "<h1><a href='./apagar'>Apagar</a> Humedad: " + String(hum) + "%. Temperatura: " + String(temp) + " C </h1>";
        digitalWrite(13, HIGH);
      }  
      if (estado == 0)
      {
        digitalWrite(13, LOW);
        webpage = "<h1><a href='./prender'>Encender</a> Humedad: " + String(hum) + "%. Temperatura: " + String(temp) + " C </h1>";
      }

      
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";


      sendData(cipSend, 500, DEBUG);
      sendData(webpage, 500, DEBUG);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";

      sendData(closeCommand, 100, DEBUG);
  }
}


String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  esp8266.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis())
  {

    while (esp8266.available())
    {

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  return response;
}

String readData(const int timeout, boolean debug)
{
  String response = "";

  long int time = millis();

  while ( (time + timeout) > millis())
  {

    while (esp8266.available())
    {

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  return response;
}
