# AlexaServo
ESP32 / ESP8266 voice-controlled wifi servo. I use it to flip switches! :-D

Tested with Amazon Echo Dot 3rd gen and Amazon Echo Studio
ESP8266 (Wemos D1 mini) ESP32 (TTGO T-DISPLAY)

# Build environment
Arduino for ESP32 on Visual Studio Code + PlatformIO

# Wifi credentials
To compile you need to write your own "wificredentials.h" file, in
order to let the ESP connect to the same router as Alexa:

    #ifndef WIFI_CREDENTIALS_H
        #define WIFI_CREDENTIALS_SSID "MY_WIFI_ROUTER_SSID"
        #define WIFI_CREDENTIALS_PASSWORD "MY_VERY_SECRET_WIFI_PASSWORD"
    #endif
