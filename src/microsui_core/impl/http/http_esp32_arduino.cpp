#if defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)

extern "C" {
#include "microsui_core/http_router.h"
}
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>

extern WiFiMulti WiFiMulti; // use the same object created in the WiFi file

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

char* microsui_http_post(const char* host, const char* path, int port, const char* jsonBody) {
    if (WiFiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi not connected! Please connect to WiFi first.");
        return NULL;
    }

    //setClock(); // Uncomment this line if you want to set the clock using NTP

    NetworkClientSecure *client = new NetworkClientSecure;
    if (client) {
        // client->setCACert(rootCACertificate); // Uncomment this line if you have a specific CA secure certificate to use
        client->setInsecure();

        HTTPClient https;

        String url = "https://" + String(host) + ":" + String(port) + String(path);
        if (https.begin(*client, url)) {
            https.addHeader("Content-Type", "application/json");

            Serial.println("[MicroSui HTTP Client]: Sending Transaction to the Sui Network...");
            int httpCode = https.POST(jsonBody);

            if (httpCode > 0) {
                Serial.println("[MicroSui HTTP Client]: Transaction sent to the Sui Network");
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
                    
                    String payload = https.getString();

                    // Allocate memory for the response
                    size_t L = payload.length();
                    char* buf = (char*)malloc(L + 1);
                    if (!buf) {
                        return NULL; // No enough memory
                    }
                    memcpy(buf, payload.c_str(), L);
                    buf[L] = '\0';

                    https.end();
                    delete client;

                    return buf;
                }
            } else {
                Serial.printf("[MicroSui HTTP Client]: Transaction send operation failed, error: %s\n", https.errorToString(httpCode).c_str());
            }
            https.end();
        } else {
            Serial.printf("[MicroSui HTTP Client]: Unable to connect to RPC: '%s'\n", url.c_str());
        }
        delete client;
    } else {
        Serial.println("[MicroSui HTTP Client]: Unable to create HTTP client");
    }
    return NULL; // Default error case
}

#endif