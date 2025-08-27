#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "microsui/sign.h"
#include "microsui/byte_conversions.h"

// Sui Message in hex format (this message must be signed)
const char* message_hex = "00000200080065cd1d0000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df974720202000101000001010300000000010100d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379d0180dc491e55e7caabfcdd1b0f538928d8d54107b9c1def3ed0baa3aa5106ba8674f0dd01400000000204b7e9da00f30cd1edf4d40710213c15a862e1fc175f2edb2b2c870c8559d65cdd79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379de80300000000000040ab3c000000000000";

// You must place your PK (in bytes format) here.
const uint8_t private_key[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int main() {
    printf("\n\t\t\t --- SUI OFFLINE SIGN DEMO ---\n");
    printf("\n\tOriginal Message: %s", message_hex);

    // Converting the message from hex to bytes
    size_t message_len = strlen(message_hex) / 2; // 2 hex chars = 1 byte
    uint8_t message[message_len];
    hex_to_bytes(message_hex, message, message_len);

    // Generating the Sui Signature from the message and private key (private_key is in constant.h)
    uint8_t sui_sig[97];
    microsui_sign_ed25519(sui_sig, message, message_len, private_key);
    printf("\n\n\n  Generating Signature...\n\n");

    // Printing the Sui Signature in hex format
    char sui_sig_hex[195]; // 2 hex chars per byte + null terminator
    bytes_to_hex(sui_sig, 97, sui_sig_hex); // 97 bytes is the length of a Sui Signature
    printf("\t Sui Signature (97 bytes): %s\n", sui_sig_hex);

    printf("\n\t SIGNATURE must be sended to the Gateway to be broadcasted to the Sui Network...\n");

    return 0;
}