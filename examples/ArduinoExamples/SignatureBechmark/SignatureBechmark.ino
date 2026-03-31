#include <Arduino.h>
#include <MicroSui.h>

// Sample Sui message and private key for testing
const uint8_t private_key[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const char *message_hex = "00000200080065cd1d0000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df974720202000101000001010300000000010100d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379d0180dc491e55e7caabfcdd1b0f538928d8d54107b9c1def3ed0baa3aa5106ba8674f0dd01400000000204b7e9da00f30cd1edf4d40710213c15a862e1fc175f2edb2b2c870c8559d65cdd79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379de80300000000000040ab3c000000000000";

size_t message_len;
uint8_t message[512];
uint8_t sui_sig[97];

void setup()
{
  Serial.begin(115200);

  message_len = strlen(message_hex) / 2; // 2 hex chars = 1 byte
  hex_to_bytes(message_hex, message, message_len);
}

void loop()
{ 
  Serial.println(F("=== MicroSui Arduino — Bechmark Signature and Verification ==="));
  // Creating Signature
  Serial.println(F("\n\n  Generating Signature...\n"));

  unsigned long start_time = millis();
  microsui_sign_ed25519(sui_sig, message, message_len, private_key);
  unsigned long end_time = millis();
  // Calculate the time taken for signing
  unsigned long time_taken_signature = end_time - start_time;

  Serial.println(F("\t Signature created successfully\n"));

  // Validating Signature
  Serial.println(F("  Verifying Signature...\n"));
  start_time = millis();
  int verification_result = microsui_verify_signature(sui_sig, message, message_len);
  end_time = millis();
  // Calculate the time taken for verification
  unsigned long time_taken_verification = end_time - start_time;

  if(verification_result == 0) {
      Serial.println(F("\t Signature verified successfully\n\n"));
  } else {
      Serial.println(F("\t Signature verification failed\n"));
      Serial.print(F("\t Error code: "));
      Serial.println(verification_result);
  }


  Serial.print(F(" Time taken for signing: "));
  Serial.print(time_taken_signature);
  Serial.println(F(" ms"));

  Serial.print(F(" Time taken for verification: "));
  Serial.print(time_taken_verification);
  Serial.println(F(" ms\n"));

  Serial.println(F("  Done."));

  delay(1000);
}