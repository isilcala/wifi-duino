/*
  wifiDuino - Library for Wifi-Duino.
  Wifi-Duino is a custmized arduino-compatible prototyping platform, providing wifi conectivity.
  please visiting http://www.winterland.me/wifi-duino/ for more info.
  Created by Winterland.
  VER1.1, 25 DEC 2013.
  Released under CC BY-SA(http://creativecommons.org/licenses/by-sa/4.0/).
*/

#include "wifiDuino.h"

bool wifiDuinoClass::commitFlag = false; 
    
void wifiDuinoClass::begin(){
    char ACK[24]="";
    pinMode(EXIT_SERIAL_PIN,OUTPUT);  
    digitalWrite(EXIT_SERIAL_PIN,LOW);

    if(Serial1.available()){
        Serial1.flush();
        delay(2);
        Serial1.end();
    }    
    Serial1.begin(115200);
    
    for(int i=0;i<30;i++){  
        delay(1600);
        digitalWrite(EXIT_SERIAL_PIN,HIGH);
        delay(200);
        digitalWrite(EXIT_SERIAL_PIN,LOW); 
        delay(200);
        Serial1.println("at+ver=?");
        if ( waitACK(ACK,100)){
            break;
        }else if(i==29) error();
    }
}

void wifiDuinoClass::setMode(char mode){
    char ACK[16]="";
    char netModeCommand[] = "at+netmode=";
    Serial1.print(netModeCommand);
    Serial1.println("?");
    if ( waitACK(ACK,100)){
        if (ACK[0] != mode ){
            commitFlag = true;
            Serial1.print(netModeCommand);
            Serial1.println(mode);
            if( !waitACK(ACK,100) ) error();
        }
    }else  error();
}

void wifiDuinoClass::setWiFiConfig(char* SSID, char* encrypt, char* password){
    char ACK[128]="";
    char WiFiConfig[128]="";
    char wifiConfigCommand[] = "at+wifi_conf=";
    //copy SSID
    strcat(WiFiConfig,SSID);
    //add separator
    strcat(WiFiConfig,",");  
    //copy encrypt
    strcat(WiFiConfig,encrypt);
    //add separator
    strcat(WiFiConfig,",");   
    //copy password
    strcat(WiFiConfig,password);  
    //get current config
    Serial1.print(wifiConfigCommand);
    Serial1.println("?");
    if ( waitACK(ACK,100)){
        if( strcmp(WiFiConfig,ACK) !=0 ){        
            commitFlag = true;
            Serial1.print(wifiConfigCommand);
            Serial1.println(WiFiConfig);
            if( !waitACK(ACK,100) ) error();
        }
    }else  error();    
}

void wifiDuinoClass::setDHCP(char dhcp){
    char ACK[16]="";
    char dhcpCommand[] = "at+dhcpc=";
    Serial1.print(dhcpCommand);
    Serial1.println("?");
    if ( waitACK(ACK,100)){
        if (ACK[0] != dhcp ){
            commitFlag = true;
            Serial1.print(dhcpCommand);
            Serial1.println(dhcp);
            if( !waitACK(ACK,100) ) error();
        }
    }else  error();
}

void wifiDuinoClass::setIpConfig(char* ip, char* mask, char* gateway){
    char ACK[48]="";
    char ipConfig[48]="";
    char ipConfigCommand[] = "at+net_ip=";
    //copy ip
    strcat(ipConfig,ip);
    //add separator
    strcat(ipConfig,",");  
    //copy mask
    strcat(ipConfig,mask);
    //add separator
    strcat(ipConfig,",");   
    //copy gateway
    strcat(ipConfig,gateway);  
    //get current config
    Serial1.print(ipConfigCommand);
    Serial1.println("?");
    if ( waitACK(ACK,100)){
        if( strcmp(ipConfig,ACK) !=0 ){        
            commitFlag = true;
            Serial1.print(ipConfigCommand);
            Serial1.println(ipConfig);
            if( !waitACK(ACK,100) ) error();
        }
    }else  error();     
}

void wifiDuinoClass::setDnsConfig(char* dns1, char* dns2){
    char ACK[32]="";
    char dnsConfig[32]="";
    char dnsConfigCommand[] = "at+net_dns=";
    //copy dns1
    strcat(dnsConfig,dns1);
    //add separator
    strcat(dnsConfig,",");  
    //copy dns2
    strcat(dnsConfig,dns2);
    //get current config
    Serial1.print(dnsConfigCommand);
    Serial1.println("?");
    if ( waitACK(ACK,100)){
        if( strcmp(dnsConfig,ACK) !=0 ){        
            commitFlag = true;
            Serial1.print(dnsConfigCommand);
            Serial1.println(dnsConfig);
            if( !waitACK(ACK,100) ) error();
        }
    }else  error();     
}

void wifiDuinoClass::writeConfig(){
    char ACK[64]="";
    char dhcpdCommand[] = "at+dhcpd=";
    char dhcpdConfig[] = "1";
    char dhcpdIpCommand[] = "at+dhcpd_ip=";
    char dhcpdIpConfig[] = "192.168.1.1,192.168.1.253,255.255.255.0,192.168.1.254";
    char dhcpdDnsCommand[] = "at+dhcpd_dns=";
    char dhcpdDnsConfig[] = "192.168.1.254,8.8.8.8";

    Serial1.print("at+netmode=");
    Serial1.println("?");
    if ( waitACK(ACK,100)) ;        
    else  error();

    if(ACK[0] == WIFI_MODE_AP){
    
        Serial1.print(dhcpdCommand);
        Serial1.println("?");
        if ( waitACK(ACK,100)){
            if ( strcmp(dhcpdConfig,ACK) ){
                commitFlag = true;
                Serial1.print(dhcpdCommand);
                Serial1.println(dhcpdConfig);
                if( !waitACK(ACK,100) ) error();
            }
        }else  error();
        
        Serial1.print(dhcpdIpCommand);
        Serial1.println("?");
        if ( waitACK(ACK,100)){
            if ( strcmp(dhcpdIpConfig,ACK) ){
                commitFlag = true;
                Serial1.print(dhcpdIpCommand);
                Serial1.println(dhcpdIpConfig);
                if( !waitACK(ACK,100) ) error();
            }
        }else  error();

        Serial1.print(dhcpdDnsCommand);
        Serial1.println("?");
        if ( waitACK(ACK,100)){
            if ( strcmp(dhcpdDnsConfig,ACK) ){
                commitFlag = true;
                Serial1.print(dhcpdDnsCommand);
                Serial1.println(dhcpdDnsConfig);
                if( !waitACK(ACK,100) ) error();
            }
        }else  error();
        
        setIpConfig("192.168.1.254","255.255.255.0","192.168.1.1");
        setDnsConfig("192.168.1.254","8.8.8.8");

    }
    if(commitFlag){
        commitFlag = false;
        //commit configuration and reconnect
        Serial1.println("at+net_commit=1");
        delay(2000);
        if ( !waitACK(ACK,100))  error();        
    }
    for(int i=0;i<30;i++){  
        Serial1.println("at+ver=?");
        if ( waitACK(ACK,100)){
            break;
        }else if(i==29) error();
    }

}

void wifiDuinoClass::startServer(char *port){
    char ACK[16];
    delay(600);
    digitalWrite(EXIT_SERIAL_PIN,HIGH);
    delay(150);
    digitalWrite(EXIT_SERIAL_PIN,LOW); 
    

    Serial1.print("at+remoteport=");
    Serial1.println(port);
    if( !waitACK(ACK,100) ) error();    
    Serial1.println("at+remotepro=tcp");
    if( !waitACK(ACK,100) ) error();   
    Serial1.println("at+timeout=0");
    if( !waitACK(ACK,100) ) error();   
    Serial1.println("at+mode=server");
    if( !waitACK(ACK,100) ) error();   
    //commit configuration and reconnect
    Serial1.println("at+save=1");
    if( !waitACK(ACK,255) ) error();
}

void wifiDuinoClass::startClient(char *remoteDomain, char *remotePort){
    char ACK[16];
    delay(600);
    digitalWrite(EXIT_SERIAL_PIN,HIGH);
    delay(150);
    digitalWrite(EXIT_SERIAL_PIN,LOW); 
    //Setting remote target configuration
    Serial1.print("at+remoteip=");
    Serial1.println(remoteDomain);
    Serial1.print("at+remoteport=");
    Serial1.println(remotePort);

    //Setting adapter client configuration
    Serial1.println("at+remotepro=tcp");
    Serial1.println("at+mode=client");
    
    //commit configuration and reconnect
    Serial1.println("at+save=1");
    if( !waitACK(ACK,100) ) error();
}

/*wait for request*/
bool wifiDuinoClass::waitHttpRequest(char* request, uint8_t len, uint16_t timeout){ 
    uint8_t requestIndex = 0;

    for(uint16_t i=0;i<timeout;){
        if(Serial1.available()){
            request[requestIndex] = (char)Serial1.read(); 
            if(request[requestIndex++] == '\n'){
                if( requestIndex > 4 && request[0]=='G' && request[1]=='E' && request[2]=='T' && request[3]==' ' ){
                    phaseGetRequest(request,len);
                    return true;
                }
                requestIndex = 0;
            }  
            if(requestIndex == len)
                return false;              
        }else{
            i++;
            delay(1);
        }
    }
    return false;
}   

/*phase request*/
void wifiDuinoClass::phaseGetRequest(char* request, uint8_t len){
    int requestIndex = 0;
    while(true){
        request[requestIndex]=request[requestIndex+4];
        requestIndex++;
        if(request[requestIndex+4] == ' ' || requestIndex == len-4 )
            break;
    }
    request[requestIndex] = 0;    
}

void wifiDuinoClass::sendHttpPage(PGM_P webPage){
    char bufferSRAM;
    Serial1.println("HTTP/1.1 200 OK");
    Serial1.println("Content-Type: text/html");
    Serial1.print("Content-Length: ");
    Serial1.println(strlen_P(webPage));
    Serial1.println("");
    while ((bufferSRAM = pgm_read_byte(webPage++)) != 0)
            Serial1.print(bufferSRAM);
}


void wifiDuinoClass::sendHttpMessage(char *message){
    Serial1.println("HTTP/1.1 200 OK");
    Serial1.println("Content-Type: text/plain");
    Serial1.print("Content-Length: ");
    Serial1.println(strlen(message));
    Serial1.println("");
    Serial1.print(message);
}


void wifiDuinoClass::error(){
    pinMode(LED_PIN,OUTPUT);  
    while(1)  digitalWrite(LED_PIN,HIGH);
}
/*Wait for command ACK*/
bool wifiDuinoClass::waitACK(char* ACK, uint8_t timeout){
    bool ACKFlag = false;
    uint8_t ACKIndex = 0;
    for(uint8_t i=0;i<timeout;){
        if(Serial1.available()){  
            ACKFlag = true;
            char inChar = (char)Serial1.read(); 
            if( inChar == '\r'){
                //ignore '\r'
            }
            else if( inChar == '\n'){
                ACK[ACKIndex] = 0;
                ACKIndex = 0;                
            }
            else
                ACK[ACKIndex++] = inChar;                
        }else{
            i++;
            delay(1);
        }
    }
    Serial.println(ACK);
    return ACKFlag;
}  
