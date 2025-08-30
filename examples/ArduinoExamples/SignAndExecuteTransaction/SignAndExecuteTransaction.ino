#include <MicroSui.h>

// --- WiFi credentials (example) ---
const char* wifi_ssid = "myWiFiSSID";
const char* wifi_pass = "myWiFiPass";

// --- Sui RPC URL (Testnet) ---
const char* rpc_url = "https://fullnode.testnet.sui.io:443/";

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
  Serial.println(F("=== MicroSui ESP32 — Sign & Execute Transaction ==="));

  // 1) Connect WiFi
  Serial.println(F("\n [1/5] Connecting WiFi..."));
  MicroSuiWiFi wifi = WiFi_connect(wifi_ssid, wifi_pass);

  // 2) Create RPC client
  Serial.println(F("\n [2/5] Creating RPC client..."));
  MicroSuiClient client = SuiClient_newClient(rpc_url);
  Serial.print(F("    DONE! - Created Client with RPC URL: ")); Serial.println(rpc_url);

  // 3) Load keypair from Bech32 secret key
  Serial.println(F("\n [3/5] Loading keypair from Bech32..."));
  MicroSuiEd25519 keypair = SuiKeypair_fromSecretKey(sui_private_key_bech32);
  Serial.print(F("    DONE! - Created KeyPair - userAddress: ")); Serial.println(keypair.toSuiAddress(&keypair));

  // 4) Load prebuilt TxBytes (generated externally)
  Serial.println(F("\n [4/5] Loading prebuilt TxBytes..."));
  MicroSuiTransaction tx = SuiTransaction_setPrebuiltTxBytes(message_string);
  Serial.println(F("    DONE! - Created a Sui Transaction object with prebuilt TxBytes"));

  // 5) Sign + Execute (simplest way: client handles the signature internally)
  Serial.println(F("\n [5/5] Signing and executing..."));
  SuiTransactionBlockResponse res1 = client.signAndExecuteTransaction(&client, keypair, tx);
  Serial.print(F("    DONE! - Signed Transaction sended to Sui Network!!"));


  Serial.println(F("\n\n --- RPC Response -  Transaction Result ---"));
  if (res1.digest)                  { Serial.print(F("     Digest (Tx ID): ")); Serial.println(res1.digest); }
  if (res1.checkpoint)              { Serial.print(F("     Checkpoint: ")); Serial.println(res1.checkpoint); }
  if (res1.confirmedLocalExecution) { Serial.print(F("     ConfirmedLocalExecution: ")); Serial.println(res1.confirmedLocalExecution); }

  Serial.print(F("     BalanceChanges count: "));
  Serial.println(res1.balanceChanges_len);

  for (int i = 0; i < res1.balanceChanges_len; i++) {
    Serial.print(F("      #")); Serial.println(i + 1);
    if (res1.balanceChanges[i].amount)   { Serial.print(F("        amount: "));   Serial.println(res1.balanceChanges[i].amount); }
    if (res1.balanceChanges[i].coinType) { Serial.print(F("        coinType: ")); Serial.println(res1.balanceChanges[i].coinType); }
    if (res1.balanceChanges[i].owner)    { Serial.print(F("        owner: "));    Serial.println(res1.balanceChanges[i].owner); }
  }
  Serial.print(F("\n"));

  // Clean transaction (important to avoid memory leaks)
  tx.clear(&tx);

  // Disconnect WiFi
  wifi.disconnect(&wifi);

  Serial.println(F("  Done."));
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn LED on
  delay(700);
  digitalWrite(LED_BUILTIN, LOW);   // turn LED off
  delay(700);
  Serial.println(F("\n  Done. Reboot the board to run the example again."));
}