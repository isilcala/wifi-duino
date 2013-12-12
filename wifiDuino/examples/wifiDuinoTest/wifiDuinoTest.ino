/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#include <wifiDuino.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
char wifiSSID[32] = "204 cafe (slow but steady)";
char wifiEncrypt[5] = "auto";
char wifiPassword[32] = "showmethemoney";
char clientPort[5] = "8081";

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.  
  pinMode(led, OUTPUT);  
 
  Serial.begin(9600);
  // while the serial stream is not open, do nothing:
   while (!Serial) ;
   
  Serial.println("Test begin!"); 
  //Check if WiFi-Duino ready
  while( !wifiDuino.checkState() ) delay(2000);
    Serial.println("Test begin!"); 
  wifiDuino.MODE =  WIFI_MODE_ADAPTER_SERVER;
    Serial.println("Test begin!"); 
  strcpy(wifiDuino.wifiSSID, wifiSSID);
    strcpy(wifiDuino.wifiEncrypt, wifiEncrypt);
      strcpy(wifiDuino.wifiPassword, wifiPassword);
        strcpy(wifiDuino.clientPort, clientPort);
  
  
  wifiDuino.DHCP = 1;
  
  Serial.println("Test begin!"); 
  wifiDuino.begin();


}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  wifiDuino.waitACK(1000);
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  wifiDuino.waitACK(1000);
}

