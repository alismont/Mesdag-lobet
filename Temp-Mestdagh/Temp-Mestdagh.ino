/******************************************************************
 Created with PROGRAMINO IDE for Arduino Sixfox - 31.01.2017 10:41:17
 Project     : Mestdagh
 Libraries   :
 Author      : Lobet
 Description : Transmission température frigo
******************************************************************/


#include <SoftwareSerial.h>
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

long tpsActuel, tpsPrecedent;
int deltaT = 0;
double tps1;
const int buttonPin = 7;
const int pinReset = 8;
int led = 13;

int buttonState = 0;         // variable for reading the pushbutton status

//******************************************************************************************
// the setup routine runs once when you press reset:
void setup() {

  // initialize the digital pin of L led as an output
  pinMode(led, OUTPUT);
  //pinMode(pinReset, OUTPUT);
 // pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT);           // set pin to input
digitalWrite(buttonPin, HIGH);

  //digitalWrite(pinReset, LOW);

  tps1 = 60000; //1 min

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Starting...");

  // Wait 1 second for the modem to warm
  delay(1000);

  // Init modem
  akeru.begin();
  tpsPrecedent = millis();
}

//************************************************************************
void loop() {

// mesure température
int valeur_brute = analogRead(A0);

  float temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0);

  Serial.println(temperature_celcius);
  delay(250);


  int sensorValue = ((-0.0974 * analogRead(A3)) + 97.54); // / 10.23
if (sensorValue>100){
  sensorValue=100;
}
  buttonState = digitalRead(buttonPin);
      Serial.println(buttonState);

  tpsActuel = millis();
  deltaT = tpsActuel - tpsPrecedent;

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:


  // LED is on


  // Trace on serial console

  //Serial.println(tps1);
  delay(1000);
  if (buttonState == LOW)
    //if (tps1 <= 0) // timer fini
  {
    Serial.println(analogRead(A3));
    Serial.println(sensorValue);
    digitalWrite(led, HIGH);
    // Wait for availability of the modem
    delay(1000);
    akeru.begin();
    delay(1000);
    //      while (!akeru.isReady()) {
    //        Serial.println("Modem not ready");
    //        //digitalWrite(pinReset, HIGH);
    //        //delay(1000);
    //      }
    // Send in the mighty internet!
    akeru.send(&sensorValue, 1);//sizeof(sensorValue));
    delay(6000);
    Serial.println("envoi sur Sigfox");
    // LED is off
    digitalWrite(led, LOW);

    delay(1000);

    tps1 = 60000;
  }
  else
  {
    tps1 = tps1 - deltaT;
  }
  tpsPrecedent = tpsActuel; // copie du temps au cycle prÃƒÂ©cÃƒÂ©dent


}
