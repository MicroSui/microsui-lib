#include <MicroSui.h>

// --- PRIVATE KEY (EXAMPLE / DO NOT USE IN PRODUCTION) ---
const char* sui_private_key_bech32 =
  "suiprivkey1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq509duq";

// --- Prebuilt TxBytes (generated externally, for now no BCS building inside MicroSui) ---
// --- For now, generate your Tx Build Message in HEX with `await tx.build({ client });` in the Mysten Sui Typescript SDK ---
const char* message_string =
  "000002000800e1f5050000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df9747202020001010000010103000000000101007a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6012901f275336715aa2d266fcdc57b3733fca4b135637160d6364acf4b7874590d0d0bd0140000000020a17f8121673e79df6e2070474050a4a4e3b51dee9887b90d87eafae9af4606257a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6e80300000000000040ab3c000000000000";

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(500);

  Serial.println();
  Serial.println(F("=== MicroSui ESP32 — Offline Sign Transaction Message - Obtain Sui Signature with PK ==="));

  // 1) Load keypair from Bech32 secret key
  Serial.println(F("\n [1/2] Loading keypair from Bech32..."));
  MicroSuiEd25519 keypair = SuiKeypair_fromSecretKey(sui_private_key_bech32);
  Serial.print(F("    DONE! - Created KeyPair - userAddress: ")); Serial.println(keypair.toSuiAddress(&keypair));

  // 4) Load prebuilt TxBytes (generated externally)
  Serial.println(F("\n [2/2] Generate Signature Object from Prebuilt Transaction Message..."));
  SuiSignature sig = keypair.signTransaction(&keypair, message_string); // We obtain the signature in base64 format
  Serial.println(F("    DONE! - Created Signature!"));

  Serial.println(F("\n --- Signature Result ---"));
  if (sig.signature) { Serial.print(F("     Signature in BASE64 format: ")); Serial.println(sig.signature); }
  if (sig.bytes) { 
    char sui_sig_hex[195]; // 2 hex chars per byte + null terminator
    bytes_to_hex(sig.bytes, 97, sui_sig_hex); // 97 bytes is the length of a Sui Signature
    
    Serial.println(F("     Signature in Hexa Bytes format: ")); Serial.println(sui_sig_hex); 
  }

  Serial.println(F("\n  Done."));
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn LED on
  delay(700);
  digitalWrite(LED_BUILTIN, LOW);   // turn LED off
  delay(700);
  Serial.println(F("\n  Done. Reboot the board to run the example again."));
}