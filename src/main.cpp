// Created by Alberto Ricci Bitti
// December 2019 - Started from EspAlexa library examples, mixed-in a Servo output to mimic an uman pressing two buttons :-)


#include <Arduino.h>
#define ESPALEXA_DEBUG
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include <Servo.h>

// Notice: credentials are .gitignore'd!
// Please add your own wificredentials.h in order to #define WIFI_CREDENTIALS_SSID and WIFI_CREDENTIALS_PASSWORD
#include "wificredentials.h"

Servo servo;
int servoAngleSpan = 10;
int servoDelay = 0;


const char* ssid = WIFI_CREDENTIALS_SSID;
const char* password = WIFI_CREDENTIALS_PASSWORD;

boolean wifiConnected = false;

Espalexa espalexa;

EspalexaDevice* device3; //this is optional

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);



//our callback functions
void firstLightChanged(uint8_t brightness) {
    Serial.print("Device 1 changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
    }
    else  {
      Serial.println("OFF");
    }
}

void servoChanged(uint8_t brightness) {
  Serial.print("Servo = ");
  Serial.println( brightness );
  servoAngleSpan = 40;
  if (brightness > 10 )
    servo.write( 90 + servoAngleSpan +15 );
  else
    servo.write( 90 - servoAngleSpan );
 
  servoDelay = 11 * 1000 /*mS*/;
 
}

void thirdLightChanged(uint8_t brightness) {
  //do what you need to do here
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void setup()
{
  servo.attach( 2 );
  servo.write( 90 );
  
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    
    // Define your devices here. 
    espalexa.addDevice("Relay 1", firstLightChanged); //simplest definition, default state off
    espalexa.addDevice("ServoMotor", servoChanged, 0); //third parameter is beginning state (here fully on)
    
    device3 = new EspalexaDevice("Relay 3", thirdLightChanged); //you can also create the Device objects yourself like here
    espalexa.addDevice(device3); //and then add them
    device3->setValue(128); //this allows you to e.g. update their state value at any time!

    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}



void loop()
{
  espalexa.loop();
  delay(1);
  if (servoDelay > 0)
  {
    servoDelay--;
    if ( servoDelay == 0 )
      servo.write( 90 );
  }
}