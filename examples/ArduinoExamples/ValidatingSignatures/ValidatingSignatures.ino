#include <MicroSui.h>

// --- PRIVATE KEY (EXAMPLE / DO NOT USE IN PRODUCTION) ---
const char *sui_private_key_bech32 =
    "suiprivkey1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq509duq";

// --- Messages to Sign (only the 1 will be signed, the second will be used to get a failed validation) ---
const char *message1_string = "0001020304050607080910";
const char *message2_string = "00020406081012141a1c1e";

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    delay(500);

    Serial.println();
    Serial.println(F("=== MicroSui ESP32 — Validation of a Sui Signature ==="));

    // 1) Generating Keypair, and Transactions Objects
    Serial.println(F("\n [1/5] Generating Keypair, and Transactions Objects..."));
    // Create a keypair from a given secret key in Bech32 format
    MicroSuiEd25519 keypair = SuiKeypair_fromSecretKey(sui_private_key_bech32);
    // Create two transactions with the prebuilted TxBytes
    MicroSuiTransaction tx1 = SuiTransaction_setPrebuiltTxBytes(message1_string);
    MicroSuiTransaction tx2 = SuiTransaction_setPrebuiltTxBytes(message2_string);

    // 2) Generating Keypair, and Transactions Objects
    Serial.println(F("\n [2/5] Signing the Transaction Message1 with the keypair..."));
    SuiSignature sig = keypair.signTransaction(&keypair, message1_string);
    
    Serial.println(F("\n --- Signature Result ---"));
    if (sig.signature) { Serial.print(F("     Signature in BASE64 format: ")); Serial.println(sig.signature); }
    if (sig.bytes) {
        char sui_sig_hex[195]; // 2 hex chars per byte + null terminator
        bytes_to_hex(sig.bytes, 97, sui_sig_hex); // 97 bytes is the length of a Sui Signature

        Serial.println(F("     Signature in Hexa Bytes format: ")); Serial.println(sui_sig_hex);
    }

    // 3) Validating Message1 with the signature (Must be Correct)
    Serial.println(F("\n [3/5] Validating Message1 with the signature (Must be Correct)..."));
    int verification_result_1 = microsui_verify_signature(sig.bytes, tx1.tx_bytes.data, tx1.tx_bytes.length);
    if (verification_result_1 == 0)
        Serial.println(F("\t   - Signature Verification is CORRECT for Message 1 -- OK\n"));

    // 4) Validating Message2 with the signature (Must be FAILED)
    Serial.println(F("\n [3/5] Validating Message2 with the signature (Must be FAILED)..."));
    int verification_result_2 = microsui_verify_signature(sig.bytes, tx2.tx_bytes.data, tx2.tx_bytes.length);
    if (verification_result_2 == 0)
        Serial.println(F("\t   - Signature Verification is FAILED for Message 2 -- OK\n\n"));

    Serial.println(F("  Done."));
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn LED on
    delay(700);
    digitalWrite(LED_BUILTIN, LOW); // turn LED off
    delay(700);
    Serial.println(F("\n  Done. Reboot the board to run the example again."));
}