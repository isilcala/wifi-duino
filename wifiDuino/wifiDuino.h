/*
  wifiDuino.h - Library for Wifi-Duino.
  Wifi-Duino is a custmized arduino-compatible prototyping platform, providing wifi conectivity.
  please visiting XXXXXXXXXXXXXXXXXXXXXXXX
  Created by Winterland.
  VER1.1, 25 DEC 2013.
  Released under GPLv2.
*/
#include <Arduino.h>
#ifndef wifiBoard_h
#define wifiBoard_h

//WiFi-Duino Mode
#define WIFI_MODE_AP '3'
#define WIFI_MODE_ADAPTER '2'

#define DHCP_ENABLE '1'
#define DHCP_DISABLE '0'

#define HTTP_GET  1
#define HTTP_HEAD 2
#define HTTP_POST 3
#define HTTP_PUT  4
#define HTTP_DELETE 5

#define EXIT_SERIAL_PIN 12
#define LED_PIN 13

class wifiDuinoClass
{
  private:   
    //Turn LED on to notify there is an error 
    static void error();
    //Wait for an AT command ACK in a timeout.
    static bool waitACK(char* ACK, uint8_t timeout);
    //Flag recording if configurations need commit.
    static bool commitFlag;

  public:
    //wait for WiFi-Duino started and enter AT command mode.
    static void begin();   
    
    /*Set working mode, available options are:
        WIFI_MODE_AP: Create a WiFi access point with WiFi configurations.
        WIFI_MODE_ADAPTER: Connect to an environment WiFi access point with WiFi configurations.
    */
    static void setMode(char mode);
    /*Environment WiFi(in adapter mode)OR AP WiFi(in AP mode)configurations in following format:
        wifi SSID, wifi encrypt options, wifi password
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
    static void setWiFiConfig(char* SSID, char* encrypt, char* password);
    
    /*Set DHCP configurations(in adapter mode), available options are:
        DHCP_ENABLE: WiFi-Duino will automatic get ip and dns configurations from router. Check if your router enable DHCP function.
        DHCP_DISABLE: Manually set ip and dns configurations, Check your network for available configurations.
    */
    static void setDHCP(char dhcp);
     
    //Manually set ip configurations(in adapter mode and DHCP == DHCP_DISABLE)
    static void setIpConfig(char* ip, char* mask, char* gateway);
    
    //Manually set dns configurations(in adapter mode and DHCP == DHCP_DISABLE)
    static void setDnsConfig(char* dns1, char* dns2);
    
    //Commit all net configurations if needed(takes about 20s~30s), do nothing if no changes found.  
    static void writeConfig();    
    
    //Start server  
    static void startServer(char *port);
    
    //Start client  
    static void startClient(char *remoteDomain, char *remotePort);
    
    //Server functions:
    //Waiting for HTTP request
    static bool waitHttpRequest(char* url, uint8_t url_len, uint16_t timeout); 

    //Send respond page in flash
    static void sendHttpPage(PGM_P webpage, PGM_P customFields = NULL); 
        
    //Send short respond message in SRAM
    static void sendHttpMessage(char *message); 
    
    //Client functions:
    //Send HTTP request
    static void sendHttpRequest(char *url, char* host = NULL, uint8_t method = HTTP_GET, char* body = NULL, char* customFields = NULL);
    
    //Waiting HTTP respond
    static bool waitHttpRespond(char* respond, uint16_t len, uint16_t timeout);
    
    //TCP functions 
    //Send TCP raw date
    static void sendRawData(char *data);
    //Send TCP raw date
    static void sendRawDataPGM(PGM_P data);
    //Waiting TCP raw date
    static bool waitRawData(char* data, uint16_t len, uint16_t timeout);

};

static wifiDuinoClass wifiDuino;
#endif
