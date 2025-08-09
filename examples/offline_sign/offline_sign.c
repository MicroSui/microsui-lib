#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "microsui/sign.h"
#include "microsui/byte_conversions.h"
#include "constants.h"


int main() {
    printf("\n\t\t\t --- SUI OFFLINE SIGN DEMO ---\n");
    printf("\n\tOriginal Message: %s", message_hex);

    // Generating the Sui Signature from the message and private key (private_key is in constant.h)
    uint8_t sui_sig[97];
    microsui_sign_message(sui_sig, message_hex, private_key);
    printf("\n\n\n  Generating Signature...\n\n");

    // Printing the Sui Signature in hex format
    char sui_sig_hex[195]; // 2 hex chars per byte + null terminator
    bytes_to_hex(sui_sig, 97, sui_sig_hex); // 97 bytes is the length of a Sui Signature
    printf("\t Sui Signature (97 bytes): %s\n", sui_sig_hex);

    printf("\n\t SIGNATURE must be sended to the Gateway to be broadcasted to the Sui Network...\n");

    return 0;
}