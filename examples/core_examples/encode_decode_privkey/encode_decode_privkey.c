#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "microsui/cryptography.h"
#include "constants.h"


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