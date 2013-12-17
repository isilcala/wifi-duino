#include "wifiDuino.h"

bool wifiDuinoClass::DEBUG = false;
uint8_t wifiDuinoClass::MODE = WIFI_MODE_AP_SERVER;
uint8_t wifiDuinoClass::DHCP = 0;
char wifiDuinoClass::requestbuffer[128] = ""; 
char wifiDuinoClass::respondbuffer[1024] = ""; 
char wifiDuinoClass::ip[16] = "192.168.0.1"; 
char wifiDuinoClass::netmask[16] = "255.255.255.0"; 
char wifiDuinoClass::gateway[16] = "192.168.0.1"; 
char wifiDuinoClass::dns[32] = "192.168.0.1,0,0,0,0"; 
char wifiDuinoClass::remoteDomain[30] ="www.baidu.com"; 
char wifiDuinoClass::remotePort[6] = "8080";
char wifiDuinoClass::clientPort[6] = "3136";
char wifiDuinoClass::listenPort[6] = "5432";
char wifiDuinoClass::wifiSSID[32] = "";
char wifiDuinoClass::wifiEncrypt[15] = "auto";
char wifiDuinoClass::wifiPassword[32] = "";

void wifiDuinoClass::begin(){
    if(DEBUG){
        Serial.println("Starting wifiDuino!");
    }
    resetSerial();
    enterATMode();

    if(MODE == WIFI_MODE_AP_SERVER){
        //Setting to AP server mode
        Serial1.println("at+netmode=3");

        //Setting DHCP configuration
        Serial1.println("at+dhcpd=1");
        Serial1.println("at+dhcpd_ip=192.168.1.1,192.168.1.254,255.255.255.0,192.168.1.254");
        Serial1.println("at+dhcpd_dns=192.168.1.254,0.0.0.0");
        Serial1.println("at+dhcpd_time=86400");

        //Setting AP WiFi configuration
        Serial1.print("at+wifi_conf=");
        Serial1.print(wifiSSID);
        Serial1.print(',');
        Serial1.print(wifiEncrypt);
        Serial1.print(',');
        Serial1.println(wifiPassword);

        //Setting AP server configuration
        Serial1.println("at+net_ip=192.168.1.254,255.255.255.0,192.168.1.254");
        Serial1.println("at+net_dns=192.168.1.254,0.0.0.0");
        Serial1.print("at+remoteport=");
        Serial1.println(listenPort);
        Serial1.println("at+remotepro=tcp");
        Serial1.println("at+timeout=0"); 

    }
    else if(MODE == WIFI_MODE_ADAPTER_SERVER){
        //Setting to adapter client mode
        Serial1.println("at+netmode=2");

        //Setting environment WiFi configuration
        Serial1.print("at+wifi_conf=");
        Serial1.print(wifiSSID);
        Serial1.print(',');
        Serial1.print(wifiEncrypt);
        Serial1.print(',');
        Serial1.println(wifiPassword);

        //Setting DHCP configuration
        Serial1.print("at+dhcpc=");
        Serial1.println(DHCP);

        //Setting Non-DHCP configuration
        if(!DHCP){
            Serial1.print("at+net_ip=");
            Serial1.print(ip);
            Serial1.print(',');
            Serial1.print(netmask);
            Serial1.print(',');
            Serial1.println(gateway);
            Serial1.print("at+net_dns");
            Serial1.println(dns);
        }

        //Setting adapter server configuration
        Serial1.print("at+CLport=");
        Serial1.println(clientPort);
        Serial1.println("at+remotepro=tcp");
        Serial1.println("at+timeout=0");
        Serial1.println("at+mode=server");

    }else if(MODE == WIFI_MODE_ADAPTER_CLIENT){
        //Setting to adapter client mode
        Serial1.println("at+netmode=2");

        //Setting environment WiFi configuration
        Serial1.print("at+wifi_conf=");
        Serial1.print(wifiSSID);
        Serial1.print(',');
        Serial1.print(wifiEncrypt);
        Serial1.print(',');
        Serial1.println(wifiPassword);

        //Setting DHCP configuration
        Serial1.print("at+dhcpc=");
        Serial1.println(DHCP);

        //Setting Non-DHCP configuration
        if(!DHCP){
            Serial1.print("at+net_ip=");
            Serial1.print(ip);
            Serial1.print(',');
            Serial1.print(netmask);
            Serial1.print(',');
            Serial1.println(gateway);
            Serial1.print("at+net_dns");
            Serial1.println(dns);
        }


        //Setting remote target configuration
        Serial1.print("at+remoteip=");
        Serial1.println(remoteDomain);
        Serial1.print("at+remoteport=");
        Serial1.println(remotePort);

        //Setting adapter client configuration
        Serial1.println("at+remotepro=tcp");
        Serial1.println("at+mode=client");
    }
    //commit configuration and reconn
    Serial1.println("at+net_commit=1");
    Serial1.println("at+reconn=1");  
  
}

/*Check WiFi Connection State*/
bool wifiDuinoClass::checkState(){
    resetSerial();
    enterATMode();
    Serial1.println("at+ver=?");
    if ( waitACK(100) ){
        Serial1.println("at+reconn=1"); 
        return true; 
    }else return false; 
}

/*Enter AT Command Mode*/
void wifiDuinoClass::enterATMode(){
  pinMode(EXIT_SERIAL_PIN,OUTPUT);  
  digitalWrite(EXIT_SERIAL_PIN,HIGH);
  delay(300);
  digitalWrite(EXIT_SERIAL_PIN,LOW);
  delay(100);
}   
   
/*Enter Serial Mode*/
void wifiDuinoClass::enterSerialMode(){
  resetSerial();
  Serial1.println("at+reconn=1");
}  

/*Enter Serial Mode*/
void wifiDuinoClass::commitSerialMode(){
  resetSerial();
  Serial1.println("at+save=1");
}  

/*wait for acknowledge*/
bool wifiDuinoClass::waitACK(uint8_t timeout){
    bool ACK = false;
    for(uint8_t i=0;i<timeout;i++){
        if(Serial1.available()){
            ACK = true;
            char inChar = (char)Serial1.read(); 
            Serial.print(inChar);
            i--;
        }else{
            delay(1);
        }
    }
    return ACK;
}   

/*reset Serial1*/
void wifiDuinoClass::resetSerial(){
    if(Serial1){
        Serial1.flush();
        delay(2);
        Serial1.end();
    }
    Serial1.begin(115200);
}

/*wait for request*/
bool wifiDuinoClass::request(uint8_t timeout){ 
    int8_t requestIndex = 0;

    if(DEBUG)
        Serial.println("Waiting for requests...");
    resetSerial();
    for(uint8_t i=0;i<timeout;){
        if(Serial1.available()){
            requestbuffer[requestIndex] = (char)Serial1.read(); 
            if(requestbuffer[requestIndex] == '\n'){
                if( requestIndex > 2 && requestbuffer[0]=='G' && requestbuffer[1]=='E' && requestbuffer[2]=='T' )
                    return true;

                requestIndex = -1;
            }  
            requestIndex++; 
            if(requestIndex == 128)
                return false;              
        }else{
            i++;
            delay(1);
        }
    }
    return false;
}   

/*phase request*/
void wifiDuinoClass::phaseGetRequest(){
    int requestIndex = 0;
    if(requestbuffer[0]=='G' && requestbuffer[1]=='E' && requestbuffer[2]=='T' ) {
        while(true){
            requestbuffer[requestIndex]=requestbuffer[requestIndex+4];
            requestIndex++;
            if(requestbuffer[requestIndex+4] == 0 || requestbuffer[requestIndex+4] == ' ' || requestIndex == 128 )
                break;
        }
        requestbuffer[requestIndex] == 0;
        for(;requestIndex<128;requestIndex++){
            requestbuffer[requestIndex] = 0;
        }
    }
}

/*send respond*/
void wifiDuinoClass::respond(){
    Serial1.print("HTTP/1.1 200 OK\r\n");
    Serial1.print("Content-Type: text/html\r\n");
    Serial1.print("Content-Length: ");
    Serial1.print(strlen(respondbuffer));
    Serial1.print("\r\n");
    Serial1.print("\r\n");
    Serial1.print(respondbuffer);
}

/*send short respond message*/
void wifiDuinoClass::respondMessage(char *message){
    Serial1.print("HTTP/1.1 200 OK\r\n");
    Serial1.print("Content-Type: text/plain\r\n");
    Serial1.print("Content-Length: ");
    Serial1.print(strlen(message));
    Serial1.print("\r\n");
    Serial1.print("\r\n");
    Serial1.print(message);
}
