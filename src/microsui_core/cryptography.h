#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <stdint.h>
#include <stddef.h>

#define PK_BECH32_LEN 70   // Length of Sui private key in Bech32 format

int microsui_decode_sui_privkey(const char *privkey_bech, uint8_t privkey_bytes_output[32]);

int microsui_encode_sui_privkey(const uint8_t *privkey_bytes, char *privkey_bech_output);

#endif