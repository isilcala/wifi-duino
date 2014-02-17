/*
  wifiDuino.h - Library for Wifi-Duino.
  Wifi-Duino is a custmized arduino-compatible prototyping platform, providing wifi conectivity.
  please visiting XXXXXXXXXXXXXXXXXXXXXXXX
  Created by Winterland.
  VER1.1, 25 DEC 2013.
  Released under GPLv2.
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
    
    digitalWrite(EXIT_SERIAL_PIN,HIGH);
    delay(200);
    digitalWrite(EXIT_SERIAL_PIN,LOW); 
    delay(100);
    Serial1.println("at+ver=?");
    if ( waitACK(ACK,100)){
        return;
    }else  delay(30000);
    
    digitalWrite(EXIT_SERIAL_PIN,HIGH);
    delay(200);
    digitalWrite(EXIT_SERIAL_PIN,LOW); 
    delay(100);
    Serial1.println("at+ver=?");
    if ( !waitACK(ACK,100) ) error();
    
}

void wifiDuinoClass::setMode(char mode){
    char ACK[24]="";
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
    char ACK[24]="";
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
    char dhcpdIpConfig[] = "192.168.1.1,192.168.1.254,255.255.255.0,192.168.1.254";
    char dhcpdDnsCommand[] = "at+dhcpd_dns=";
    char dhcpdDnsConfig[] = "192.168.1.254,8.8.8.8";

    Serial1.print("at+netmode=");
    Serial1.println("?");
    if ( !waitACK(ACK,100))  error();

    if(ACK[0] == WIFI_MODE_AP){
    
        Serial1.print(dhcpdCommand);
        Serial1.println("?");
        if ( waitACK(ACK,100)){
            if ( strcmp(dhcpdConfig,ACK) !=0 ){
                commitFlag = true;
                Serial1.print(dhcpdCommand);
                Serial1.println(dhcpdConfig);
                if( !waitACK(ACK,100) ) error();
            }
        }else  error();
        
        Serial1.print(dhcpdIpCommand);
        Serial1.println("?");
        if ( waitACK(ACK,100)){
            if ( strcmp(dhcpdIpConfig,ACK) !=0 ){
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
        //commit configuration and reconnect
        Serial1.println("at+net_commit=1");
        for(uint8_t i=0;i<100;i++){  
            delay(2000);
            if ( waitACK(ACK,255) ){                
                commitFlag = false;
                break;          
            }    
            if( i==99 )  error();
        }        
                
        delay(30000);
        Serial1.println("at+ver=?");
        if ( !waitACK(ACK,255) ) error();
    } 
}

void wifiDuinoClass::startServer(char *port){
    char ACK[24]="";
    delay(450);
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
    char ACK[24];
    delay(450);
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
//wait for request
bool wifiDuinoClass::waitHttpRequest(char* request, uint8_t len, uint16_t timeout){ 
    uint8_t requestIndex = 0;
    uint8_t beforeGet = true;
    
    for(uint16_t i=0;i<timeout;){
        if(Serial1.available()){
            request[requestIndex++] = (char)Serial1.read(); 
            //find GET request
            if( beforeGet && requestIndex > 3 && request[0]=='G' && request[1]=='E' && request[2]=='T' && request[3]==' ' ){
                beforeGet = false;
                requestIndex = 0;
            }else if(!beforeGet && request[requestIndex-1] == ' ' ){
                request[requestIndex-1] = 0;
                return true;
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

void wifiDuinoClass::sendHttpPage(PGM_P webPage, PGM_P customFields){
    char bufferSRAM;
    Serial1.println("HTTP/1.1 200 OK");
    Serial1.println("Content-Type: text/html");
    Serial1.print("Content-Length: ");
    Serial1.println(strlen_P(webPage));
    if(customFields)
    while ((bufferSRAM = pgm_read_byte(customFields++)) != 0)
            Serial1.print(bufferSRAM);
    Serial1.println();
    while ((bufferSRAM = pgm_read_byte(webPage++)) != 0)
            Serial1.print(bufferSRAM);
}


void wifiDuinoClass::sendHttpMessage(char *message){
    Serial1.println("HTTP/1.1 200 OK");
    Serial1.println("Content-Type: text/plain");
    Serial1.print("Content-Length: ");
    Serial1.println(strlen(message));
    Serial1.println();
    Serial1.print(message);
}

void wifiDuinoClass::sendHttpRequest(char *url, char* body, uint8_t method, char* host, char* customFields){
    char bufferSRAM;
    switch(method){
        case HTTP_GET:            
            Serial1.print("GET ");
            break;
        case HTTP_HEAD:
            Serial1.print("HEAD ");
            break;
        case HTTP_POST:
            Serial1.print("POST ");
            break;
        case HTTP_PUT:
            Serial1.print("PUT ");
            break;
        case HTTP_DELETE:
            Serial1.print("DELETE ");
            break;
    }
    Serial1.print(url);
    Serial1.println(" HTTP/1.1");
    Serial1.print("Host: ");
    if(host)
        Serial1.print(host);
    Serial1.println();
    if(customFields)
    while ((bufferSRAM = *(customFields++)) != 0)
            Serial1.print(bufferSRAM);
    Serial1.println();    
}

bool wifiDuinoClass::waitHttpRespond(char* respond, uint16_t len, uint16_t timeout){
    uint8_t respondIndex = 0;
    uint8_t headerSection = true;
    for(uint16_t i=0;i<timeout;){
        if(Serial1.available()){
            respond[respondIndex++] = (char)Serial1.read();
            //double CRLF means header section finish
            if( headerSection &&
                respondIndex > 3 && 
                respond[respondIndex-1]=='\n' && 
                respond[respondIndex-2]=='\r' && 
                respond[respondIndex-3]=='\n' &&
                respond[respondIndex-4]=='\r'){
                //reset index
                respondIndex = 0;
                headerSection = false;
            }
            if(respondIndex == len)
                return false;              
        }else{
            i++;
            delay(1);
        }
    }
    if( respondIndex>0 ){
        respond[respondIndex-1]=0;
        return true;  
    }else return false;
}

void wifiDuinoClass::sendRawData(char *data){
    Serial1.print(data);
}

bool wifiDuinoClass::waitRawData(char* data, uint16_t len, uint16_t timeout){

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
    if(ACKFlag)
    Serial.println(ACK);
    return ACKFlag;
}  
