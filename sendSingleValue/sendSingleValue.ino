/* Akeru.h - sendSingleValues.ino
 * 
 * Copyleft Snootlab 2016
 * 
 * How to send a single analog value on the Sigfox network
 */

#include <Akeru.h>

// TD1208 Sigfox module IO definition
/*   Snootlab device | TX | RX
               Akeru | D4 | D5
               Akene | D5 | D4
            Breakout | your pick */
#define TX 4
#define RX 5

// Sigfox instance management 
Akeru akeru(RX, TX);
int led = 13;

//*********************SETUP**********************************
void setup() 
{    
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Starting...");
    pinMode(led, OUTPUT);
  // Check TD1208 communication
  if (!akeru.begin())
  {
    Serial.println("TD1208 KO");
    while(1);
  }
  
  //akeru.echoOn(); // uncomment this line to see AT commands
}

//*********************LOOP**********************************
void loop() 
{
  // Read module temperature
  int temperature = 0;
  if (akeru.getTemperature(&temperature))
  {
    Serial.print("Temperature MODULE = ");
    Serial.print(temperature);
    Serial.println(" C");
    
  }
  else
  {
    Serial.println("Temperature KO");
  }
  
  
    int valeur_brute = analogRead(A0);

  //float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0);
    //int temperature_celciusint = int(temperature_celcius);
      //Serial.print("temperature ambiante: ");
      //Serial.println(temperature_celciusint);
  delay(250);
  
  

  String data = temperature;//String(70) akeru.toHex(temperature_celcius); 
  Serial.println(data);

  // Send in the mighty internet!
  // akeru.sendPayload() returns 0 if message failed.
  if (akeru.sendPayload(data))
  {
    Serial.println("Message sent !");
    digitalWrite(led, HIGH);
     delay(1000);
    digitalWrite(led, LOW);
  }

  // Wait for 10 minutes.
  // Note that delay(600) will block the Arduino (bug in delay()?)
  for (int second = 0; second < 10; second++) 
  {
    delay(1000);
  }
}
