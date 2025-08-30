#ifndef KEY_MANAGEMENT_H
#define KEY_MANAGEMENT_H

#include <stdint.h>
#include <stddef.h>

int microsui_pubkey_to_sui_address(const uint8_t pubkey[32], uint8_t sui_address_out[32]);

int get_public_key_from_private_key(const uint8_t private_key[32], uint8_t public_key[32]);

#endif