#ifndef KEY_MANAGEMENT_H
#define KEY_MANAGEMENT_H

#include <stdint.h>
#include <stddef.h>

int microsui_derive_sui_address_ed25519(uint8_t sui_address_out[32], const uint8_t pubkey[32]);

int microsui_derive_public_key_ed25519(uint8_t public_key[32], const uint8_t seed[32]);

#endif