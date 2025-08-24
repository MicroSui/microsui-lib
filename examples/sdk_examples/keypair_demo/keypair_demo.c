#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Keypair.h"

int main(void) {
    printf("\n\t\t\t --- MicroSui KEYPAIR DEMO ---");
    printf("\n\t Generating several MicroSuiEd25519 keypairs and demonstrating its usage...\n\n");
    
    // Generate a random keypair with a seed extra value of 300
    const uint8_t seed_extra = (uint8_t)100;    // This value can be useful in systems with poor entropy generation, in that case you can use a random value taked from some sensor or other source of entropy
    MicroSuiEd25519 keypair_random = SuiKeypair_generate(seed_extra);
    printf("\n\t\t 1 - Random Keypair generated:\n");
    const char* sk_random = keypair_random.getSecretKey(&keypair_random);
    printf("Random created Secret Key: %s\n", sk_random);

    // Create a keypair from a given secret key in Bech32 format
    MicroSuiEd25519 keypair = SuiKeypair_fromSecretKey("suiprivkey1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq509duq");
    printf("\n\t\t 2 - Keypair from Bech32 private key:\n");

    printf("\tRecover Bech32 private key from keypair:\n");
    const char* sk = keypair.getSecretKey(&keypair);
    printf("Secret Key: %s\n", sk);

    printf("\n\tPublic Key and Sui Address from the keypair:\n");
    const uint8_t* public_key = keypair.getPublicKey(&keypair);
    printf("Public Key: ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", public_key[i]);
    }
    printf("\n");
    const char* addr= keypair.toSuiAddress(&keypair);
    printf("Sui Address: %s\n", addr);

    printf("\n\tSigning a Transaction Message with the keypair...\n");
    const char* messageToSign = "0000020008c0d8a7000000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df9747202020001010000010103000000000101009c0b6f8f043efe774abf4c7142a28f164ed99a0db47b57c8a9986838a75cdea001eb703aa03ea788d4e768947ac1141432dc554fe00aba196e70329d0c8ac536bd630fd0140000000020c0b96efe5463bc3f3b148f4cb9d0bd7daf6aa2fd6865295a0941947933261d089c0b6f8f043efe774abf4c7142a28f164ed99a0db47b57c8a9986838a75cdea0e80300000000000040ab3c000000000000";
    SuiSignature sig = keypair.signTransaction(&keypair, messageToSign);
    printf("   Signature in base64: %s\n", sig.signature);
    printf("   Signature in bytes: ");
    for (int i = 0; i < 97; i++) {
        printf("%02x", sig.bytes[i]);
    }
    printf("\n");

    printf("\n\n\t\t --- END OF KEYPAIR DEMO ---\n");

    return 0;
}