#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "sign.h"
#include "constants.h"

const char* msg_hex = "00000200080065cd1d0000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df974720202000101000001010300000000010100d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379d0180dc491e55e7caabfcdd1b0f538928d8d54107b9c1def3ed0baa3aa5106ba8674f0dd01400000000204b7e9da00f30cd1edf4d40710213c15a862e1fc175f2edb2b2c870c8559d65cdd79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379de80300000000000040ab3c000000000000";

int main() {
    printf("\n\n\t\t\t --- SUI OFFLINE SIGN DEMO ---\n");
    printf("\n\tOriginal Message: %s", msg_hex);


    uint8_t sui_sig[97];
    microsui_sign_offline(sui_sig, msg_hex, private_key);
    printf("\n\n\n  Generating Signature...\n\n");
    print_hex("\t Sui Signature", sui_sig, 97);
    printf("\n\t SIGNATURE must be sended to the Gateway to be broadcasted to the Sui Network...\n");


    return 0;
}