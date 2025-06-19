#ifndef SIGN_H
#define SIGN_H

int microsui_sign_message(uint8_t signature[97], const char* message_hex, const uint8_t private_key[32]);

void hex_to_bytes(const char* hex_str, uint8_t* bytes, uint32_t bytes_len);

void bytes_to_hex(const uint8_t* bytes, uint32_t bytes_len, char* hex_str);

#endif