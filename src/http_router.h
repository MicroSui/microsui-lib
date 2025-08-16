#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int microsui_http_post(const char* host,
              const char* path,
              int port,
              const char* jsonBody,
              char* responseOut,
              size_t maxLen);

#ifdef __cplusplus
}
#endif

#endif