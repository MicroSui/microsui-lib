#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "MicroSui.h"

int main(void) {
    printf("\n\t\t\t --- MicroSui VALIDATING SUI SIGNATURE DEMO ---");
    printf("\n\t This demo shows how to validate a Sui Signature.\n");

    const char* private_key = "suiprivkey1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq509duq";
    const char* message1_string = "0001020304050607080910";
    const char* message2_string = "00020406081012141a1c1e";

    // Create a keypair from a given secret key in Bech32 format
    MicroSuiEd25519 keypair = SuiKeypair_fromSecretKey(private_key);
    // Create two transactions with the prebuilted TxBytes
    MicroSuiTransaction tx1 = SuiTransaction_setPrebuiltTxBytes(message1_string);
    MicroSuiTransaction tx2 = SuiTransaction_setPrebuiltTxBytes(message2_string);

    printf("\n\t Signing the Transaction Message1 with the keypair...\n\n");
    SuiSignature sig = keypair.signTransaction(&keypair, message1_string);

    printf("     Signature in base64: %s\n", sig.signature);
    printf("     Signature in bytes: ");
    for (int i = 0; i < 97; i++) {
    printf("%02x", sig.bytes[i]);
    }
    printf("\n\n");

    // Signature validation must be correct for Message 1 and failed for Message 2
    // Validation with message 1 (correct)
    printf("\t Signature validation must be correct for Message 1 and failed for Message 2\n");

    int verification_result_1 = microsui_verify_signature(sig.bytes, tx1.tx_bytes.data, tx1.tx_bytes.length);
    if(verification_result_1 == 0)
        printf("\t   - Signature Verification is CORRECT for Message 1 -- OK\n");
    // Validation with message 2 (incorrect)
    int verification_result_2 = microsui_verify_signature(sig.bytes, tx2.tx_bytes.data, tx2.tx_bytes.length);
    if(verification_result_2 != 0)
        printf("\t   - Signature Verification is FAILED for Message 2 -- OK\n");

    printf("\n\n\t\t --- END OF TRANSACTION DEMO ---\n");
    return 0;
}