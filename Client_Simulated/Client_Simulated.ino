#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>

hw_timer_t * timer = NULL;

DynamicJsonDocument root(1024);

const  char * ssid = "HUAWEI-EhP9" ; 
const  char * password = "A74jR5Gp" ;
const char * pshash = "32CAFE015" ;


const IPAddress serverIP (103,231,240,131) ;//172,16,0,52  //the address to be accessed 
uint16_t serverPort =  25280;//25280          //server port number
char stopped;
int counter = 0;
long randNumberX, randNumberY, randNumberZ;
WiFiClient client ;  //Declare a client object to connect to the server

void IRAM_ATTR onTimer() {
  counter+=1;
}

void  setup ( ) 
{ 
    Serial.begin ( 115200 ) ; 
    Serial.println ( ) ;

    WiFi.mode(WIFI_STA); 
    WiFi.setSleep ( false ) ;  //Turn off wifi sleep in STA mode to improve response speed 
    WiFi.begin("Wokwi-GUEST", "", 6); 
    while  ( WiFi.status ( )  != WL_CONNECTED ) 
    { 
        delay ( 500 ) ; 
        Serial.print ( "." ) ; 
    } 
    Serial.println ( "CLIENT: Connected" ) ; 
    Serial.print ( "CLIENT: the IP the Address:" ) ; 
    Serial.println (WiFi.localIP()) ; 

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true);
    timerAlarmEnable(timer);
    randomSeed(analogRead(0)); //for randomizer
    
}

void  loop ( ) 
{   
  Serial.println ( "\nCLIENT: Try to access the server" ) ; 
    if  ( client.connect ( serverIP , serverPort ) )
    {
      Serial.println ( "\nCLIENT: Access successful" ) ;
      Serial.println ( "\nCLIENT: Authenticating..." );
      client.print(pshash);
      Serial.println ( "\nCLIENT: Enter 'Y' to stop client connection." );
    }
      
    while  ( client.connected ( )  || client.available ( ) )  //If connected or received unread data 
    {            
          stopped = Serial.read();
          if (stopped == 'Y' || stopped == 'y')
          {
            client.print(stopped);
            Serial.println("\nCLIENT: Stopping...");
            Serial.println("\nCLIENT: Disconnecting...");
            client.stop(); //Close the client
          }
          randNumberX = random(-200,-180);
          randNumberY = random(770,800);
          randNumberZ = random(360,400);
        //Send data to the server 
        client.print("{\"time\":" + String(counter) + ", " +
                      "\"x\":" + randNumberX/100.0 + ", " + 
                      "\"y\":" + randNumberY/100.0 + ", " + 
                      "\"z\":" + randNumberZ/100.0 + "}");
        delay(2000);
        } 
    Serial.println ( "\nCLIENT: Closing connection (Authentication Failed/Intentional Close/Failed Connection)" ) ; 
    client.stop ( ) ;  //Close the client 
    delay ( 60000 ) ; 
}