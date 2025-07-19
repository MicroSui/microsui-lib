#include <MicroSui.h>

//// Sui Private Keys - Its the same key in two different formats, so in both examples we will obtain the same signature
// PrivateKey in Bech32 Sui Compatible format `suiprivkey1...`, This format is used in most web3 Sui wallets.
const char* private_key_1_bech32 = "suiprivkey1qqqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqa4ffsr";

// PrivateKey in hexa format, must be 32 bytes long
const uint8_t private_key_2_hexa[32] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
};

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(1000);

  Serial.println("\n\n\n\t\t\t --- ENCODE/DECODE SUI Bech32 PrivateKeys Examples ---\n");


  ////////////////  EXAMPLE 1  ////////////////

  Serial.println("\n\t\t - Example 1: decode_sui_privkey (We obtain an Hexa key from a Bech32 one)");
  
  uint8_t private_key_hexa_output[32];
  if (microsui_decode_sui_privkey(private_key_1_bech32, private_key_hexa_output) == 0) {
    Serial.print("\n\t Original Bech32 Key: ");
    Serial.write(private_key_1_bech32);

    char key_1_string[65];
    bytes_to_hex(private_key_hexa_output, 32, key_1_string);
    Serial.print("\n\n\t Decoded Key: ");
    Serial.write(key_1_string);
  } else {
    Serial.println("\n\t Invalid Bech32 key.\n");
  }


  ////////////////  EXAMPLE 2  ////////////////

  Serial.println("\n\n\n\t\t - Example 2: encode_sui_privkey (We obtain an Bech32 key from a Hexa one)");
  
  char private_key_bech32_output[71];
  if (microsui_encode_sui_privkey(private_key_2_hexa, private_key_bech32_output) == 0) {
  char key_2_string[65];
  bytes_to_hex(private_key_2_hexa, 32, key_2_string);
  Serial.print("\n\t Original Hexa Key: ");
  Serial.write(key_2_string);

  Serial.print("\n\t Encoded Bech32 Key: ");
  Serial.write(private_key_1_bech32);
  } else {
    Serial.println("\n\t Invalid hexa key.\n");
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(700);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(700);                      // wait for a second
}