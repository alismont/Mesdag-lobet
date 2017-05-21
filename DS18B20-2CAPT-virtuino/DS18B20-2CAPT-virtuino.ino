/* Virtuino example No1 (Bluetooth)
 * Example name = "Hello Virtuino World"
 * Created by Ilias Lamprou
 * Updated Jul 01 2016
 * This is the code you need to run on your arduino board to cummunicate whith VirtUino app via bluetooth
 * Before  running this code config the settings below as the instructions on the right
 * 
 * 
 * Download latest Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.virtuino_automations.virtuino
 * Getting starting link:
 * Video tutorial link: https://www.youtube.com/watch?v=CYR_jigRkgk
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

/*========= VirtuinoBluetooth Class methods  
*  vPinMode(int pin, int state)                                  set pin as digital OUTPUT or digital INPUT.  (Insteed default pinMode method
*
*========= Virtuino General methods  
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)   write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)               read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int analogMemoryIndex, float value)         write a value to Virtuino float memory       (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int analogMemoryIndex)                      read the value of  Virtuino analog memory    (analogMemoryIndex=0..31, returned value range = 0..1023)
*  run()                                                         neccesary command to communicate with Virtuino android app  (on start of void loop)
*  void vDelay(long milliseconds);                               Pauses the program (without block communication) for the amount of time (in miliseconds) specified as parameter
*  int getPinValue(int pin)                                      read the value of a Pin. Usefull for PWM pins
*/

/* 
 * 
 * Auteur: Alain Lismont
 * 
 * 
 */

 // chargement librairie Akeru carte de communication SigFox & réseau un fil.......................
#include <OneWire.h>
#include "VirtuinoBluetooth.h" 

 
// communication vers tablette
// Include VirtuinoBluetooth library to your code
//#include <SoftwareSerial.h>                              //  Disable this line if you want to use hardware serial 
//SoftwareSerial bluetoothSerial =  SoftwareSerial(2,3);   // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to bluetooth module TX pin   -  connect the arduino TX pin to bluetooth module RX pin.  Disable this line if you want to use hardware serial 
//VirtuinoBluetooth virtuino(Serial,9600);       // Set SoftwareSerial baud rate.  -  Disable this line if you want to use hardware serial 

// config réseau one wire
/* Broche du bus 1-Wire */
const byte BROCHE_ONEWIRE = 8;

/* Code de retour de la fonction getTemperature() */
enum DS18B20_RCODES {
  READ_OK,
  NO_SENSOR_FOUND,
  INVALID_ADDRESS,
  INVALID_SENSOR
};


/* Création de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);                                                 

#include <Akeru.h>

// TD1208 Sigfox module IO definition
/*   Snootlab device | TX | RX
               Akeru | D4 | D5
               Akene | D5 | D4
            Breakout | your pick */
#define TX 4 //4
#define RX 5 //5

#define Led1 10
#define Pous1 7

#define BP1On digitalRead(Pous1)



#define Led1On digitalWrite(Led1,HIGH)
#define Led1Off digitalWrite(Led1,LOW)

// Sigfox instance management
Akeru akeru(RX, TX);

 //char charVal[15];                                            
//int cpt=0;
//String IDString;

String data="69";
      char chrval1[2]; 
            char chrval2[2]; 
//================================================================== setup
//==================================================================
//==================================================================
void setup() 
{

pinMode (Pous1,INPUT_PULLUP);
pinMode(Led1, OUTPUT);
  //virtuino.DEBUG=true;               // set this value TRUE to enable the serial monitor status
  Serial.begin(9600);                // Set serial monitor baud rate

  akeru.begin();
    Serial.println("Starting...");

  // Check TD1208 communication
  if (!akeru.begin())
  {
    Serial.println("TD1208 KO");
    while(1);
  }

}
 

//******************************************************************************************************
void loop(){

      

      
 float temperature[3];
   //virtuino.run();           //  neccesary command to communicate with Virtuino android app

   //  virtuino.vMemoryWrite(0, temperature[0]);
  //  virtuino.vMemoryWrite(1, temperature[1]);
 
  

  
if(BP1On==0)
{
Serial.println("BOUTON ON ??????????????????????????????????????");

  // Convert to hexadecimal before sending
  String c = akeru.toHex(temperature[0]);
      String esp = akeru.toHex(47);
  String l = akeru.toHex(temperature[1]);

 String msg = c +esp+ l ; // Put everything together

    Serial.println(msg);
    Led1On;
    Serial.print("akeru.isReady():");
    Serial.println(akeru.isReady());

    delay(500);
    Led1Off;
if(akeru.isReady())
{
          Led1On;
              delay(500);
    Led1Off;
    // modification remplacer data par msg
      if (akeru.sendPayload(msg))

  {
  

      Led1On;
    Serial.println("Message sent !");
    delay(3000);
    Led1Off;
  } else {
        Serial.println("no sent !");
  }
}
 }   

   
  /* Lit les températures des trois capteurs */
  if (getTemperature(&temperature[0], true) != READ_OK) {
    Serial.println(F("Erreur de lecture du capteur 1"));
    return;
  }
  if (getTemperature(&temperature[1], false) != READ_OK) {
    Serial.println(F("Erreur de lecture du capteur 2"));
    return;
  }
/*  if (getTemperature(&temperature[2], false) != READ_OK) {
    Serial.println(F("Erreur de lecture du capteur 3"));
    return;
  }*/
   
  /* Affiche les températures */
  Serial.print(F("Temperatures : "));
  Serial.print(temperature[0], 2);
  Serial.write("°"); // Caractère degré
  Serial.print(F("C, "));
  Serial.print(temperature[1], 2);
  Serial.write("°"); // Caractère degré
  Serial.println(F("C, "));

  
dtostrf(temperature[0], 2, 0, chrval1);
  Serial.println(chrval1);

  dtostrf(temperature[1], 2, 0, chrval2);
  Serial.println(chrval2);
  
}


//*******************************************************************************************

 
 //****************************************************************************************** 
/**
 * Fonction de lecture de la température via un capteur DS18B20.
 */
byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : Données lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire détecté
  
  /* Reset le bus 1-Wire ci nécessaire (requis pour la lecture du premier capteur) */
  if (reset_search) {
    ds.reset_search();
  }
 
  /* Recherche le prochain capteur 1-Wire disponible */
  if (!ds.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  }
  
  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }
 
  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (addr[0] != 0x28) {
    // Mauvais type de capteur
    return INVALID_SENSOR;
  }
 
  /* Reset le bus 1-Wire et sélectionne le capteur */
  ds.reset();
  ds.select(addr);
  
  /* Lance une prise de mesure de température et attend la fin de la mesure */
  ds.write(0x44, 1);
  delay(800);
  
  /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
 
 /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
   
  /* Calcul de la température en degré Celsius */
  *temperature = ((data[1] << 8) | data[0]) * 0.0625; 
  
  // Pas d'erreur
  return READ_OK;
}

