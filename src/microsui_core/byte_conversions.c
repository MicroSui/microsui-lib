#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/// Hex Digits
static const char hex_digits[] = "0123456789abcdef";
/// Hex character value mapping
static inline uint8_t hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}
/// Base64 Table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/// Base64 character value mapping
static int base64_char_value(char c) {
    if ('A' <= c && c <= 'Z') return c - 'A';
    if ('a' <= c && c <= 'z') return c - 'a' + 26;
    if ('0' <= c && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1; // invalid character
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

int bytes_to_base64(const uint8_t* input, size_t input_len, char* output, size_t output_size) {
    size_t i = 0, j = 0;

    while (i + 2 < input_len) {
        if (j + 4 >= output_size) break;

        uint32_t triple = (input[i] << 16) | (input[i + 1] << 8) | input[i + 2];

        output[j++] = base64_table[(triple >> 18) & 0x3F];
        output[j++] = base64_table[(triple >> 12) & 0x3F];
        output[j++] = base64_table[(triple >> 6) & 0x3F];
        output[j++] = base64_table[triple & 0x3F];

        i += 3;
    }

    // Handling the end (1 or 2 bytes remaining)
    if (i < input_len && j + 4 < output_size) {
        uint8_t byte0 = input[i];
        uint8_t byte1 = (i + 1 < input_len) ? input[i + 1] : 0;

        output[j++] = base64_table[(byte0 >> 2) & 0x3F];
        output[j++] = base64_table[((byte0 & 0x03) << 4) | ((byte1 >> 4) & 0x0F)];

        if (i + 1 < input_len) {
            output[j++] = base64_table[(byte1 & 0x0F) << 2];
            output[j++] = '=';
        } else {
            output[j++] = '=';
            output[j++] = '=';
        }
    }

    if (j < output_size)
        output[j] = '\0';

    return 0; // success
}

int base64_to_bytes(const char* input, size_t input_len, uint8_t* output, size_t output_size) {
    if (input_len % 4 != 0) return -1;

    size_t i = 0, j = 0;

    while (i < input_len) {
        int a = base64_char_value(input[i++]);
        int b = base64_char_value(input[i++]);
        int c = (input[i] != '=') ? base64_char_value(input[i]) : 0; i++;
        int d = (input[i] != '=') ? base64_char_value(input[i]) : 0; i++;

        if (a < 0 || b < 0 || (input[i - 2] != '=' && c < 0) || (input[i - 1] != '=' && d < 0)) {
            return -1; // invalid character
        }

        uint32_t triple = (a << 18) | (b << 12) | (c << 6) | d;

        if (j + 1 > output_size) return -1;
        output[j++] = (triple >> 16) & 0xFF;

        if (input[i - 2] != '=') {
            if (j + 1 > output_size) return -1;
            output[j++] = (triple >> 8) & 0xFF;
        }

        if (input[i - 1] != '=') {
            if (j + 1 > output_size) return -1;
            output[j++] = triple & 0xFF;
        }
    }
    return 0; // success
}