#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static const char hex_digits[] = "0123456789abcdef";

static inline uint8_t hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

void hex_to_bytes(const char* hex_str, uint8_t* bytes, uint32_t bytes_len) {
    for (uint32_t i = 0; i < bytes_len; i++) {
        uint8_t hi = hex_val(hex_str[2*i    ]);
        uint8_t lo = hex_val(hex_str[2*i + 1]);
        bytes[i] = (hi << 4) | lo;
    }
}

void bytes_to_hex(const uint8_t* bytes, uint32_t bytes_len, char* hex_str) {
    for (uint32_t i = 0; i < bytes_len; i++) {
        uint8_t b = bytes[i];
        hex_str[2*i    ] = hex_digits[(b >> 4) & 0x0F];
        hex_str[2*i + 1] = hex_digits[b & 0x0F];
    }
    hex_str[2 * bytes_len] = '\0';
}