#if defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)

extern "C" {
#include "microsui_core/wifi_router.h"
}
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

int microsui_connect_wifi(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);

    Serial.print("Waiting for WiFi to connect...");
    while ((WiFiMulti.run() != WL_CONNECTED)) {
        Serial.print(".");
        delay(500);
    }
    Serial.println(" connected");
    
    return 0;
}

int microsui_disconnect_wifi() {
    Serial.println("Disconnecting WiFi...");
  
    // Disconnect from the current AP - true:erase current WiFi connection and true: erase stored credentials in flash
    WiFi.disconnect(true, true);

    // Stop WiFi driver completely
    WiFi.mode(WIFI_OFF);
  
    Serial.println("WiFi disconnected and resources freed");
    return 0;
  }

#endif