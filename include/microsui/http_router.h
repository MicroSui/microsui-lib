#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include <stddef.h>

int http_post(const char* host,
              const char* path,
              int port,
              const char* jsonBody,
              char* responseOut,
              size_t maxLen);

#endif
