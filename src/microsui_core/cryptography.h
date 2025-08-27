#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <stdint.h>
#include <stddef.h>

#define PK_BECH32_LEN 70   // Length of Sui private key in Bech32 format

int microsui_decode_sui_privkey(const char *privkey_bech, uint8_t privkey_bytes_output[32]);

int microsui_encode_sui_privkey(const uint8_t *privkey_bytes, char *privkey_bech_output);

int microsui_pubkey_to_sui_address(const uint8_t pubkey[32], uint8_t sui_address_out[32]);

int get_public_key_from_private_key(const uint8_t private_key[32], uint8_t public_key[32]);

#endif