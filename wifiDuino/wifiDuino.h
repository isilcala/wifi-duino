/*
  wifiDuino.h - Library for Wifi-Duino.
  Wifi-Duino is a custmized arduino-compatible prototyping platform, providing wifi conectivity.
  please visiting XXXXXXXXXXXXXXXXXXXXXXXX
  Created by Winterland, November 3, 2013.
  Released under GPLv2.
*/

#ifndef wifiBoard_h
#define wifiBoard_h

#ifndef WIFI_DUINO_DEBUG
#define WIFI_DUINO_DEBUG 0
#endif

#define WIFI_MODE_AP_SERVER 0

#define WIFI_MODE_AP_SERVER 0
#define WIFI_MODE_ADAPTER_SERVER 1
#define WIFI_MODE_ADAPTER_CLIENT 2
#define DHCP_ENABLE 1
#define DHCP_DISABLE 0

#define EXIT_SERIAL_PIN 12

#include <Arduino.h>

class wifiDuinoClass
{
  public:
    //Package buffe
    static char requestbuffer[128]; 
    static char respondbuffer[1024]; 
    static char commandbuffer[32]; 

    //WiFi-Duino mode
    static uint8_t MODE;

    //Adapter configuration   
    static uint8_t DHCP;
    static char ip[16]; 
    static char netmask[16]; 
    static char gateway[16]; 
    static char dns[32]; 
    
    //Client configuration
    static char remoteDomain[32]; 
    static char remotePort[6]; 

    //Server configuration
    static char listenPort[6]; 

    //Environment WiFi OR AP WiFi configurations
    //wifi SSID
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
    //wifi password
    static char wifiPassword[32];
    
    //Starting WiFi-Duino with Serial1
    static void begin();    
    //Commit net changes, eg: change between AP to Adapter, ip setting changes, etc.
    static void commit();    
    //Check if WiFi-Duino is started up
    static bool checkState();    
    //Check mode saved configurations
    static bool verifyConfig(); 

    //Start in server  
    static void startServer();
    
    static void startClient();
    //handle HTTP request
    static bool getRequest(uint8_t timeout); 
    //send buffer content as HTTP respond
    static void sendRespond(); 
    //send short HTTP respond message
    static void sendRespondMessage(char *message);    
    //send HTTP request
    static void sendRequest(char *url);
    //get HTTP respond
    static void getRespond();
    
    
    //Get MAC address of WiFi-Duino 
    static void getMAC(char* MAC);  
   
  private:
    static void phaseGetRequest();
    static void enterATMode();
    static bool waitACK(uint8_t timeout);
    
};

extern wifiDuinoClass wifiDuino;

#endif
