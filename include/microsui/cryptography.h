#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

int microsui_decode_sui_privkey(const char *bech, uint8_t secret32[32]);

int microsui_encode_sui_privkey(const uint8_t *privkey_bytes, char *output);

#endif