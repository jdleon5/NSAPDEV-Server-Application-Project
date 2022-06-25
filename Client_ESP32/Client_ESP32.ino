#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ArduinoJson.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12346);
hw_timer_t * timer = NULL;

DynamicJsonDocument root(1024);
sensors_event_t event;

const  char * ssid = "TP-Link_759C" ; 
const  char * password = "PLDTWIFIzgxct_0" ;
const char * pshash = "32CAFE015"

const IPAddress serverIP (103,231,240,131) ;//172,16,0,52  //the address to be accessed 
uint16_t serverPort =  25280;//25280          //server port number
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

    //Check sensor
    if (!accel.begin()) {
      Serial.println("Accelerator not detected.");
      while(1);
    }
    accel.setRange(ADXL345_RANGE_16_G);
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
            }
            //Input data timer print here 
            accel.getEvent(&event);
            //Send data to the server 
            client.print("{\"time\":" + String(counter) + ", " +
                         "\"x\":" + event.acceleration.x + ", " + 
                         "\"y\":" + event.acceleration.y + ", " + 
                         "\"z\":" + event.acceleration.z + "}");
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
    else 
    { 
        Serial.println ( "\nCLIENT: Access failed" ) ;
        client.stop ( ) ;  //Close the client 
        //delay(60000);
    } 
    delay ( 5000 ) ; 
}
