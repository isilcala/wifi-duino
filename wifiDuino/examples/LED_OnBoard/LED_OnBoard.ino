/*
  LED_OnBoard
  Control the LED on WiFi-Duino board
  Author: Winterland
  Doc: http://www.winterland.com/wifiduino
 */
#include <wifiDuino.h>

// Pin 13 has an LED connected on most Arduino boards.
#define led 13

// storage web page in flash
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

void setup() {                
  // initialize the digital pin as an output.  
  pinMode(led, OUTPUT);  
  Serial.begin(9600);
  
  wifiDuino.begin();
  wifiDuino.setMode(WIFI_MODE_ADAPTER);
  wifiDuino.setWiFiConfig("cfw","auto","zbh26513897");
  wifiDuino.setDHCP(DHCP_DISABLE);
  wifiDuino.setIpConfig("192.168.1.254","255.255.255.0","192.168.1.1");
  wifiDuino.setDnsConfig("192.168.1.254","8.8.8.8");
  wifiDuino.writeConfig();
  wifiDuino.startServer("8081");
}

// the loop routine runs over and over again forever:
void loop() {
  char request[32]="";
  if(wifiDuino.waitHttpRequest(request,32,1000)){
     if( !strcmp(request,"/ledon") ){
       digitalWrite(led, HIGH);
     }
     if( !strcmp(request,"/ledoff") ){
       digitalWrite(led, LOW);
     }
     wifiDuino.sendHttpPage(webPage);
  }
}



