#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "microsui/sign.h"
#include "constants.h"


int main() {
    printf("\n\n\t\t\t --- SUI OFFLINE SIGN DEMO ---\n");
    printf("\n\tOriginal Message: %s", message_hex);

    uint8_t sui_sig[97];
    microsui_sign_message(sui_sig, message_hex, private_key);
    printf("\n\n\n  Generating Signature...\n\n");
    print_hex("\t Sui Signature", sui_sig, 97);
    printf("\n\t SIGNATURE must be sended to the Gateway to be broadcasted to the Sui Network...\n");

    return 0;
}