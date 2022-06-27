#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>

hw_timer_t * timer = NULL;

DynamicJsonDocument root(1024);

const  char * ssid = "HUAWEI-EhP9" ; 
const  char * password = "A74jR5Gp" ;
const char * pshash = "32CAFE015" ;

const IPAddress serverIP (172,16,0,52) ;//172,16,0,52  //the address to be accessed 
uint16_t serverPort =  8080;//25280          //server port number
char stopped;
int counter = 0;

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
    WiFi.begin ( ssid , password ) ; 
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
}

void  loop ( ) 
{ 
    Serial.println ( "\nCLIENT: Try to access the server" ) ; 
    if  ( client.connect ( serverIP , serverPort ) )  //Try to access the target address 
    { 
        Serial.println ( "\nCLIENT: Access successful" ) ;
        Serial.println ( "\nCLIENT: Enter 'Y' to stop client connection." );
        while  ( client.connected ( )  || client.available ( ) )  //If connected or received unread data 
        {            
            if(Serial.available() > 0) 
            {
              stopped = Serial.read();
              if (stopped == 'Y' || stopped == 'y')
              {
                client.print(stopped);
                Serial.println("\nCLIENT: Stopping...");
                Serial.println("\nCLIENT: Disconnecting...");
                client.stop(); //Close the client
              }
            //Send data to the server 
            client.print("{\"time\":" + String(counter) + ", " +
                         "\"x\":" + "1" + ", " + 
                         "\"y\":" + "1" + ", " + 
                         "\"z\":" + "1" + "}");
            delay(1000);
            //vTaskDelay(portMAX_DELAY);
        } 
        Serial.println ( "\nCLIENT: Close the current connection" ) ; 
        client.stop ( ) ;  //Close the client 
        while(1)
            {
              //Trap
              delay(5000);
              char ans;
              ans = Serial.read();
              if(ans == 'x')
                break;             
            }
        
        //delay(60000);
    } 
    }
    else 
    { 
        Serial.println ( "\nCLIENT: Access failed" ) ;
        client.stop ( ) ;  //Close the client 
        //delay(60000);
    } 
    delay ( 5000 ) ; 
}