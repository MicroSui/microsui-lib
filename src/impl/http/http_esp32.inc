#include <Arduino.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
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


int http_post(const char* host, const char* path, int port, const char* jsonBody, char* responseOut, size_t maxLen) {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Blueground1651", "showupstartliving");

  //Serial.print("Waiting for WiFi to connect...");
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected");

  //setClock();

  NetworkClientSecure *client = new NetworkClientSecure;
  if (client) {
    client->setInsecure();

    // client->setCACert(rootCACertificate);

    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client,
                    "https://fullnode.testnet.sui.io:443")) {  // URL de ejemplo
      https.addHeader("Content-Type", "application/json");

      Serial.print("[HTTPS] POST...\n");
      int httpCode = https.POST(jsonBody);

      if (httpCode > 0) {
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n",
                      https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }

    delete client;
  } else {
    Serial.println("Unable to create client");
  }
  return 0;
}