#ifndef WIFI_ROUTER_H
#define WIFI_ROUTER_H

#include <stddef.h>

#define WIFI_SUPPORTED_PLATFORMS ( \
    (defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)) \
)

#if !WIFI_SUPPORTED_PLATFORMS
#   error "MicroSui: WiFi support isn’t available on this platform/board yet. To use it here, add an adapter for your platform (implement microsui_http_post) and save it in src/impl/wifi/wifi_<your_platform>.extension"
#endif

int microsui_connect_wifi(const char* ssid, const char* password);

#endif