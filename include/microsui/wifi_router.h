#ifndef WIFI_ROUTER_H
#define WIFI_ROUTER_H

#include <stddef.h>

#define WIFI_SUPPORTED_PLATFORMS ( \
    (defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)) \
)

int microsui_connect_wifi(const char* ssid, const char* password);

#endif