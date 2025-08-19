#ifndef SIGN_H
#define SIGN_H

#include <stdint.h>
#include <stddef.h>

int microsui_sign_message(uint8_t sui_sig[97], const uint8_t* message, const size_t message_len, const uint8_t private_key[32]);

#endif