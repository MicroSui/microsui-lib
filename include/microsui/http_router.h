#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include <stddef.h>

int microsui_http_post(const char* host,
              const char* path,
              int port,
              const char* jsonBody,
              char* responseOut,
              size_t maxLen);

int microsui_connect_wifi(const char* ssid, const char* password);

#endif