#ifndef UTILS_H
#define UTILS_H

void hex_to_bytes(const char* hex_str, uint8_t* bytes, uint32_t bytes_len);

void bytes_to_hex(const uint8_t* bytes, uint32_t bytes_len, char* hex_str);

void bytes_to_base64(const uint8_t* input, size_t input_len, char* output, size_t output_size);

void base64_to_bytes(const char* input, size_t input_len, uint8_t* output, size_t output_size);

#endif