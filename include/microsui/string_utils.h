#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdint.h>
#include <stddef.h>

int parse_url(const char *url, char *host, size_t host_size, char *path, size_t path_size, int *port_out);

#endif
