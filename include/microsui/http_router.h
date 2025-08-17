#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "utils/string_builder.h" // For StringBuilder usage, util for string managment
#include <stddef.h>

int microsui_http_post(const char* host, const char* path, int port, const char* jsonBody, char** responseOut);

int microsui_connect_wifi(const char* ssid, const char* password);

#endif