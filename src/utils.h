#ifndef UTILS_H
#define UTILS_H

void hex_to_bytes(const char* hex_str, uint8_t* bytes, uint32_t bytes_len);

void bytes_to_hex(const uint8_t* bytes, uint32_t bytes_len, char* hex_str);

#endif