#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "microsui/cryptography.h"

const char *private_key_1_bech32 = "suiprivkey1qqqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqa4ffsr";

const uint8_t private_key_2[32] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
};

int main() {
    printf("\t\t ENCODE / DECODE Sui bech32 private keys example:\n");

    printf("\t1 - decode_sui_privkey:\n");
    uint8_t private_key_output[32];
    if (microsui_decode_sui_privkey(private_key_1_bech32, private_key_output) == 0) {
        printf("Original Bech32 Key: %s\n", private_key_1_bech32);
        printf("Decoded Key: ");
        for (int i = 0; i < 32; i++) {
            printf("%02x", private_key_output[i]);
        }
    } else {
        printf("Invalid Bech32 key.\n");
        return 1;
    }

    printf("\n\n\t2 - encode_sui_privkey:\n");
    char private_key_bech32_output[71];
    if (microsui_encode_sui_privkey(private_key_2, private_key_bech32_output) == 0) {
        printf("Original Hexa Key: ");
        for (int i = 0; i < 32; i++) {
            printf("%02x", private_key_2[i]);
        }
        printf("\nEncoded Key: %s\n", private_key_bech32_output);
    } else {
        printf("Invalid Hexa key\n");
    }
    return 0;
}