#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <stdint.h>
#include <stddef.h>

#define PK_BECH32_LEN 70   // Length of Sui private key in Bech32 format

int microsui_decode_bech32_private_key(uint8_t seed_out[32], const char *private_key_bech);

int microsui_encode_bech32_private_key(char *private_key_bech_out, const uint8_t *seed);

#endif