// Created by Alberto Ricci Bitti
// December 2019 - Started from EspAlexa library examples, mixed-in a Servo output to mimic an uman pressing two buttons :-)


#include <Arduino.h>
#define ESPALEXA_DEBUG
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Servo.h>
#include <fauxmoESP.h>

// Notice: credentials are .gitignore'd!
// Please add your own wificredentials.h in order to #define WIFI_CREDENTIALS_SSID and WIFI_CREDENTIALS_PASSWORD
#include "wificredentials.h"

Servo servo;
int servoDelay = 0;

const char* ssid = WIFI_CREDENTIALS_SSID;
const char* password = WIFI_CREDENTIALS_PASSWORD;

#define LED_PIN D7
#define LED_INIT pinMode(LED_PIN, OUTPUT)

#define LED_ON  digitalWrite( LED_PIN, LOW )
#define LED_OFF digitalWrite( LED_PIN, HIGH )

boolean wifiConnected = false;

fauxmoESP fauxmo;

void servoChanged(bool isOn) {
  Serial.print("Servo = ");
  Serial.println( isOn ? "ON" :"OFF" );
  int servoAngleSpan = 65;
  if (isOn)
  {
    servo.write( 90 + servoAngleSpan );
    LED_ON;
  }
  else
  {
    servo.write( 90 - servoAngleSpan );
    LED_OFF;
  }

  servoDelay = 11 * 1000 /*mS*/;
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
  LED_INIT;
  LED_OFF;
  servo.attach( 2 );
  servo.write( 90 );
  
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected)
  {    
    fauxmo.addDevice("ServoMotor");   
    //fauxmo.setState("ServoMotor", 0, 1);
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
          if ( strcmp(device_name, "ServoMotor")==0 ) 
          {
            servoChanged( state );            
            Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
          }
      });
    fauxmo.setPort(80); // required for gen3 devices
    fauxmo.enable(true);
  } 
  else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}



void loop()
{
  //espalexa.loop();
  fauxmo.handle();
  
  // check for connection breaks
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connection lost, rebooting...");
    delay(10000);
    ESP.restart();
    delay(10000);
  }

  // put servo at rest after some delay, 
  // here we can assume taking about 1mS per loop
  delay(1);
  if (servoDelay > 0)
  {
    servoDelay--;
    if ( servoDelay == 0 )
      servo.write( 90 );
  }
}