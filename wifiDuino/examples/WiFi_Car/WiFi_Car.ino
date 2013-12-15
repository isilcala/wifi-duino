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
//Arduino PWM Speed Control：
#define DIR1 3  
#define PWM1 9
#define DIR2 4                     
#define PWM2 10 

char wifiSSID[18] = "WiFi-Duino";
char wifiEncrypt[12] = "wpawpa2_aes";
char wifiPassword[11] = "hellocar";
char listenPort[5] = "8081";

char webPage[] PROGMEM =
"<html>"
  "<head><title>"
    "WiFi Car"
  "</title>" 
  "<script>"    
  "function httpGet(theUrl){"
      "var xmlHttp=null;"
      "xmlHttp=new XMLHttpRequest();"
      "xmlHttp.open('GET',theUrl,true);"      
      "xmlHttp.timeout=300;"
      "xmlHttp.send( null );"
  "}"
  "</script>"
  "<style>"
  "button{height:96px;width:256px;font-size:48px;margin:24px;}"
  "body{text-align:center}"
  "</style>"
  "</head>"
  "<body>"  
    "<button onclick=\"httpGet('start')\">start</button>"
    "<p><button onclick=\"httpGet('left')\">left</button>"
    "<button onclick=\"httpGet('back')\">back</button>"
    "<button onclick=\"httpGet('right')\">right</button></p>"
    "<button onclick=\"httpGet('stop')\">stop</button>"
  "</body>"   
"</html>" 
;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.  
  pinMode(led, OUTPUT);  
  pinMode(DIR1, OUTPUT);   
  pinMode(DIR2, OUTPUT); 
  digitalWrite(DIR1,LOW);   
  digitalWrite(DIR2,LOW);  
  analogWrite(PWM1, 0);  
  analogWrite(PWM2, 0); 
  
  delay(500);
//  Serial.begin(9600);  // while the serial stream is not open, do nothing:
//   while (!Serial) ;
   
  //Check if WiFi-Duino ready
  while( !wifiDuino.checkState() ) delay(2000);
//      Serial.println("WiFi Duino Found");
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
    if( strcmp(wifiDuino.requestbuffer,"/") == 0 ){
      wifiDuino.respond(); 
    }
    else if( strcmp(wifiDuino.requestbuffer,"/left") == 0 ){
      digitalWrite(DIR1,LOW);   
      digitalWrite(DIR2,LOW); 
      analogWrite(PWM1, 255);  
      analogWrite(PWM2, 100);  
      wifiDuino.respondMessage("OK"); 
    }
    else if( strcmp(wifiDuino.requestbuffer,"/right") == 0 ){
      digitalWrite(DIR1,LOW);   
      digitalWrite(DIR2,LOW); 
      analogWrite(PWM1, 100); 
      analogWrite(PWM2, 255);   
      wifiDuino.respondMessage("OK"); 
    }
    else if( strcmp(wifiDuino.requestbuffer,"/back") == 0 ){
      digitalWrite(DIR1,HIGH);   
      digitalWrite(DIR2,HIGH); 
      analogWrite(PWM1, 100); 
      analogWrite(PWM2, 100);   
      wifiDuino.respondMessage("OK"); 
    }
    else if( strcmp(wifiDuino.requestbuffer,"/stop") == 0 ){ 
      digitalWrite(DIR1,LOW);   
      digitalWrite(DIR2,LOW);      
      analogWrite(PWM1, 0);  
      analogWrite(PWM2, 0);  
      wifiDuino.respondMessage("OK"); 
    }   
    else if( strcmp(wifiDuino.requestbuffer,"/start") == 0 ){
      digitalWrite(DIR1,LOW);   
      digitalWrite(DIR2,LOW); 
      analogWrite(PWM1, 200);  
      analogWrite(PWM2, 200);  
      wifiDuino.respondMessage("OK"); 
    }
    else{
      wifiDuino.respondMessage("404 Error"); 
    }
  }
}


