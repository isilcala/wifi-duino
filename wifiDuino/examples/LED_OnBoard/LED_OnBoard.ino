/*
  LED_OnBoard
  Control the LED on WiFi-Duino board
  Author: Winterland
  Doc: http://www.winterland.com/wifiduino
 */
#include <wifiDuino.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#define led 13

char wifiSSID[18] = "WiFi-Duino";
char wifiEncrypt[12] = "wpawpa2_aes";
char wifiPassword[11] = "helloled";
char listenPort[5] = "8081";

char webPage[] PROGMEM =
"<html>"
  "<head><title>"
    "multipackets Test"
  "</title></head>"
  "<body>"
    "<a href='/ledon'>LED ON</a><br>"
    "<a href='/ledoff'>LED OFF</a><br>"
  "</body>"   
"</html>" 
;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.  
  pinMode(led, OUTPUT);  
 
  Serial.begin(9600);
  // while the serial stream is not open, do nothing:
   while (!Serial) ;
   
  //Check if WiFi-Duino ready
  while( !wifiDuino.checkState() ) delay(2000);
      Serial.println("WiFi Duino Found");
  delay(1000);
  
  wifiDuino.MODE = WIFI_MODE_AP_SERVER;
  strcpy(wifiDuino.wifiSSID, wifiSSID);
    strcpy(wifiDuino.wifiEncrypt, wifiEncrypt);
      strcpy(wifiDuino.wifiPassword, wifiPassword);
        strcpy(wifiDuino.listenPort, listenPort);
  

  strcpy_P(wifiDuino.respondbuffer, webPage);
  wifiDuino.begin();
  
  for(int i = 0; i<100 ; i++)
    wifiDuino.waitACK(255);
}

// the loop routine runs over and over again forever:
void loop() {
 if(wifiDuino.request(255)){
     wifiDuino.phaseGetRequest();
     Serial.println(wifiDuino.requestbuffer);
     if( strcmp(wifiDuino.requestbuffer,"/ledon") == 0 ){
       digitalWrite(led,HIGH);
       wifiDuino.respondMessage("OK"); 
     }
     else if( strcmp(wifiDuino.requestbuffer,"/ledoff") == 0 ){
       digitalWrite(led,LOW);
       wifiDuino.respondMessage("OK"); 
     }
     else if( strcmp(wifiDuino.requestbuffer,"/") == 0 ){
       Serial.println("Web page served.");
       wifiDuino.respond(); 
     }
 }
}


