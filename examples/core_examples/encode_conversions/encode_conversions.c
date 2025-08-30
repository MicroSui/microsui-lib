#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "microsui/byte_conversions.h"


int main() {
    printf("\t\t Byte / HEXA and BASE64 CONVERSION examples:\n");

    printf("\n\t In this examplse we will use the same signature in all the examples. \n");
    printf("\t The signature is a Sui Signature example, which is 97 bytes long.\n");
    printf("\t The signature will be converted to Base64 and Hexadecimal formats, and then back to Bytes.\n");

    // CONVERSION 1 - Input Bytes output Base64 and Hexa
    printf("\n\n\t 1 - Byte to BASE64 and HEXA :\n");
    const uint8_t signature_bytes[97] = {
        0x00, 0x41, 0x31, 0x44, 0x6a, 0x85, 0x41, 0xb3, 0xc4, 0x08, 0xe1, 0x80, 0x54, 0xf2, 0xe1, 0x75,
        0xf1, 0xb0, 0x4a, 0x93, 0xf6, 0xab, 0x1b, 0xbf, 0x38, 0xd1, 0x23, 0x4a, 0x3b, 0x6e, 0xd6, 0x5a,
        0xdf, 0x50, 0x8e, 0x9a, 0x03, 0x55, 0x52, 0x69, 0x22, 0x25, 0x36, 0xe0, 0xec, 0xdb, 0xf4, 0xd4,
        0x96, 0xde, 0x72, 0x12, 0xc5, 0xe2, 0xf9, 0xc4, 0xde, 0xc2, 0x25, 0x12, 0x26, 0xb7, 0xfc, 0xf8,
        0x03, 0x63, 0x09, 0xed, 0xe0, 0xa4, 0x80, 0x22, 0x9c, 0x12, 0xb5, 0x78, 0x36, 0x4c, 0xa1, 0x3b,
        0xc3, 0x6c, 0xbf, 0x61, 0x78, 0x6c, 0x47, 0xaa, 0x75, 0xf3, 0x23, 0x12, 0x74, 0x62, 0xb3, 0x24,
        0x05
    };

    char output_signature_base64[(97 + 2) / 3 * 4 + 1]; // Base64 output size calculation + 1 for null terminator
    bytes_to_base64(signature_bytes, sizeof(signature_bytes), output_signature_base64, sizeof(output_signature_base64));
    printf("-> Signature in Base64: %s\n", output_signature_base64);

    char output_signature_hex[(97 * 2) + 1]; // Hex output size calculation + 1 for null terminator
    bytes_to_hex(signature_bytes, sizeof(signature_bytes), output_signature_hex);
    printf("-> Signature in Hexa: %s\n", output_signature_hex);


    // CONVERSION 2 - Input Hexa output Bytes
    printf("\n\n\t 2 - HEXA to Bytes:\n");
    const char signature_hex[(97 * 2) + 1] = "004131446a8541b3c408e18054f2e175f1b04a93f6ab1bbf38d1234a3b6ed65adf508e9a03555269222536e0ecdbf4d496de7212c5e2f9c4dec2251226b7fcf8036309ede0a480229c12b578364ca13bc36cbf61786c47aa75f323127462b32405";
    uint8_t output_signature_bytes_from_hex[97];
    hex_to_bytes(signature_hex, output_signature_bytes_from_hex, sizeof(output_signature_bytes_from_hex));
    printf("-> Signature in Bytes: [");
    for (size_t i = 0; i < sizeof(output_signature_bytes_from_hex); i++) {
        if (i+1 < sizeof(output_signature_bytes_from_hex)) {
            printf("0x%02x,", output_signature_bytes_from_hex[i]);
        } else {
            printf("0x%02x", output_signature_bytes_from_hex[i]);
        }
    }
    printf("]\n");

    // CONVERSION 3 - Input Base64 output Bytes
    printf("\n\n\t 3 - BASE64 to Bytes:\n");
    const char signature_base64[(97 + 2) / 3 * 4 + 1] = "AEExRGqFQbPECOGAVPLhdfGwSpP2qxu/ONEjSjtu1lrfUI6aA1VSaSIlNuDs2/TUlt5yEsXi+cTewiUSJrf8+ANjCe3gpIAinBK1eDZMoTvDbL9heGxHqnXzIxJ0YrMkBQ==";
    
    size_t signature_base64_len = strlen(signature_base64);
    size_t output_signature_bytes_from_base64_len = (signature_base64_len / 4) * 3;
    if (signature_base64[signature_base64_len - 1] == '=') output_signature_bytes_from_base64_len--;
    if (signature_base64[signature_base64_len - 2] == '=') output_signature_bytes_from_base64_len--;
    
    uint8_t output_signature_bytes_from_base64[output_signature_bytes_from_base64_len];
    base64_to_bytes(signature_base64, signature_base64_len, output_signature_bytes_from_base64, output_signature_bytes_from_base64_len);
    printf("-> Signature in Bytes: [");
    for (size_t i = 0; i < sizeof(output_signature_bytes_from_base64); i++) {
        if (i+1 < sizeof(output_signature_bytes_from_base64)) {
            printf("0x%02x,", output_signature_bytes_from_base64[i]);
        } else {
            printf("0x%02x", output_signature_bytes_from_base64[i]);
        }
    }
    printf("]\n");


    return 0;
}