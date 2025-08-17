#if defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)

extern "C" {
#include "http_router.h"
}
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setClock() {
    configTime(0, 0, "pool.ntp.org");

    //Serial.print(F("Waiting for NTP time sync: "));
    time_t nowSecs = time(nullptr);
    while (nowSecs < 8 * 3600 * 2) {
        delay(500);
        Serial.print(F("."));
        yield();
        nowSecs = time(nullptr);
    }

    Serial.println();
    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);
    Serial.print(F("Current time: "));
    Serial.print(asctime(&timeinfo));
}

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


int microsui_http_post(const char* host, const char* path, int port, const char* jsonBody, char* responseOut, size_t maxLen) {
    //setClock(); // Uncomment this line if you want to set the clock using NTP

    NetworkClientSecure *client = new NetworkClientSecure;
    if (client) {
        // client->setCACert(rootCACertificate); // Uncomment this line if you have a specific CA secure certificate to use
        client->setInsecure();

        HTTPClient https;
        Serial.print("[HTTPS] begin...\n");

        String url = "https://" + String(host) + ":" + String(port) + String(path);
        if (https.begin(*client, url)) {
            https.addHeader("Content-Type", "application/json");

            Serial.print("[HTTPS] POST...\n");
            int httpCode = https.POST(jsonBody);

            if (httpCode > 0) {
                Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
                    String payload = https.getString();
                    //Serial.println(payload);

                    // Prepare the JSON-RPC request string - Using a StringBuilder for efficient string concatenation
                    StringBuilder sb;
                    if (!sb_init(&sb, 1600, 10000)) {
                        return -1;
                    }

                    // Append the response to the StringBuilder
                    sb_append(&sb, payload.c_str());

                    responseOut = sb_detach(&sb);

                    https.end();
                    delete client;

                    return 0;
                }
            } else {
                Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
            }
            https.end();
        } else {
            Serial.printf("[HTTPS] Unable to connect to %s\n", url.c_str());
        }
        delete client;
    } else {
        Serial.println("Unable to create client");
    }
    return -1; // Default error case
}

#endif