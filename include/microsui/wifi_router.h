#ifndef WIFI_ROUTER_H
#define WIFI_ROUTER_H

#include <stddef.h>

#if (defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)) || \
    defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#   define WIFI_SUPPORTED_PLATFORMS 1
#else
#  define WIFI_SUPPORTED_PLATFORMS 0
#endif

int microsui_connect_wifi(const char* ssid, const char* password);

int microsui_disconnect_wifi();

#endif