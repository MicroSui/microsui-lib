#ifndef SIGN_H
#define SIGN_H

int microsui_sign_message(uint8_t signature[97], const char* message_hex, const uint8_t private_key[32]);

#endif