#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "utils/string_builder.h" // For StringBuilder usage, util for string managment
#include <stddef.h>

#if (defined(ARDUINO) && defined(ARDUINO_ARCH_ESP32)) || \
    defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#  define HTTP_SUPPORTED_PLATFORMS 1
#else
#  define HTTP_SUPPORTED_PLATFORMS 0
#endif

char* microsui_http_post(const char* host, const char* path, int port, const char* jsonBody);

#endif