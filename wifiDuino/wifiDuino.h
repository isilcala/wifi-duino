/*
  wifiDuino.h - Library for Wifi-Duino.
  Wifi-Duino is a custmized arduino-compatible prototyping platform, providing wifi conectivity.
  please visiting XXXXXXXXXXXXXXXXXXXXXXXX
  Created by Winterland, November 3, 2013.
  Released under GPLv2.
*/

#ifndef wifiBoard_h
#define wifiBoard_h

#define WIFI_MODE_AP_SERVER 0
#define WIFI_MODE_ADAPTER_SERVER 1
#define WIFI_MODE_ADAPTER_CLIENT 2
#define DHCP_ENABLE 1
#define DHCP_DISABLE 0

#define EXIT_SERIAL_PIN 12

#include "Arduino.h"


class wifiDuinoClass
{
  public:
    //Package buffe
    static char requestbuffer[128]; 
    static char respondbuffer[1024]; 
    //Debug flag
    static bool DEBUG;

    //WiFi-Duino mode
    static uint8_t MODE;

    //Adapter configuration   
    static uint8_t DHCP;
    static char ip[16]; 
    static char netmask[16]; 
    static char gateway[16]; 
    static char dns[32]; 
    static char clientPort[6]; 

    //AP server configuration
    static char listenPort[6]; 

    //Remote target configuration
    static char remoteDomain[30]; 
    static char remotePort[6]; 

    //Environment WiFi OR AP WiFi configuration

    /*wifi SSID*/
    static char wifiSSID[32];

    /*
    wifi encrypt options:
    none:         Open network 
    wep_open:     wep encryption，open authentication method 
    wep:          wep encryption，encryption authentication 
    wpa_tkip:     wpa tkip 
    wpa_aes:      wpa aes 
    wpa2_tkip:    wpa2 tkip 
    wpa2_aes:     wpa2 aes 
    wpawpa2_tkip: wpa/wpa2 tkip 
    wpawpa2_aes:  wpa/wpa2 aes 
    auto:         auto select(default)
    */
    static char wifiEncrypt[15];

    /*wifi password*/
    static char wifiPassword[32];

    //Starting WiFi-Duino
    static void begin();

    //Check if WiFi-Duino is started up
    static bool checkState();

    //HTTP
    static bool request(uint8_t timeout); 
    static void phaseGetRequest();
    static void respond(); 
    static void respondMessage(char *message);
    
    //Get MAC address of WiFi-Duino 
    static void getMAC(char* MAC);  
   
    static void resetSerial();   
    static void enterATMode();
    static void enterSerialMode();
    static void commitSerialMode();
    static bool waitACK(uint8_t); 
    
    
};

extern wifiDuinoClass wifiDuino;

#endif
