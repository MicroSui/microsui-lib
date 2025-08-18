#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "utils/string_builder.h" // For StringBuilder usage, util for string managment
#include <stddef.h>

#define HTTP_SUPPORTED_PLATFORMS ( \
    (defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)) || \
    defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__) \
)

#if !HTTP_SUPPORTED_PLATFORMS
#   error "MicroSui: HTTP support isn’t available on this platform/board yet. To use it here, add an adapter for your platform (implement microsui_http_post) and save it in src/impl/http/http_<your_platform>.extension"
#endif

char* microsui_http_post(const char* host, const char* path, int port, const char* jsonBody);

#endif