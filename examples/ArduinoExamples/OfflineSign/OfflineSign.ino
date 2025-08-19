#include <MicroSui.h>

//// Sui Private Keys - Its the same key in two different formats, so in both examples we will obtain the same signature
// PrivateKey in Bech32 Sui Compatible format `suiprivkey1...`, This format is used in most web3 Sui wallets.
const char* private_key_1_bech32 = "suiprivkey1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq509duq";

// PrivateKey in hexa format, must be 32 bytes long
const uint8_t private_key_2_hexa[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//// Sui Message to Sign - Generate your TX Build Message in HEX with `await tx.build({ client });` in Mysten Typescript SDK
const char* message_hex = "00000200080065cd1d0000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df974720202000101000001010300000000010100d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379d0180dc491e55e7caabfcdd1b0f538928d8d54107b9c1def3ed0baa3aa5106ba8674f0dd01400000000204b7e9da00f30cd1edf4d40710213c15a862e1fc175f2edb2b2c870c8559d65cdd79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379de80300000000000040ab3c000000000000";

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(1000);

  Serial.println("\n\t\t\t --- SUI OFFLINE SIGN Examples ---\n");

  // Converting the message from hex to bytes necessary for the signing function
  size_t message_len = strlen(message_hex) / 2; // 2 hex chars = 1 byte
  uint8_t message[message_len];
  hex_to_bytes(message_hex, message, message_len);

  ////////////////  EXAMPLE 1  ////////////////

  Serial.println("\t\t - Example 1: Sign with BECH32 Private Key (Bech32 Sui Compatible)");
  Serial.print("\n\t Original Message: ");
  Serial.write(message_hex);

  uint8_t private_key_1[32];
  // To obtain the signature the PK must be in hexadecimal format, so we will use the Bech32 MicroSui decoder
  if (microsui_decode_sui_privkey(private_key_1_bech32, private_key_1) != 0) {
    Serial.println("Invalid Bech32 private key.\n");
    return;
  }

  uint8_t sui_sig_1[97];
  // We obtain the signature here
  microsui_sign_message(sui_sig_1, message, message_len, private_key_1);

  char sui_sig_hex_1[195];
  // This MicroSui function makes an array hexa into a string for a better visualization (not needed if not printed)
  bytes_to_hex(sui_sig_1, 97, sui_sig_hex_1);

  Serial.print("\n\n\t Sui Signature (Example 1) - (97 bytes): ");
  Serial.write(sui_sig_hex_1);

  Serial.println("\n\t SIGNATURE must be sended to the Gateway to be broadcasted to the Sui Network...\n");



  ////////////////  EXAMPLE 2  ////////////////

  Serial.println("\n\n\t\t - Example 2: Sign with HEXA Private Key (No pk conversion needed)");
  Serial.print("\n\t Original Message: ");
  Serial.write(message_hex);


  uint8_t sui_sig_2[97];
  // We obtain the signature here
  microsui_sign_message(sui_sig_2, message, message_len, private_key_2_hexa);

  char sui_sig_hex_2[195];
  // This MicroSui function makes an array hexa into a string for a better visualization (not needed if not printed)
  bytes_to_hex(sui_sig_2, 97, sui_sig_hex_2);

  Serial.print("\n\n\t Sui Signature (Example 2) - (97 bytes): ");
  Serial.write(sui_sig_hex_2);

  Serial.println("\n\t SIGNATURE must be sended to the Gateway to be broadcasted to the Sui Network...\n");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(700);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(700);                      // wait for a second
}