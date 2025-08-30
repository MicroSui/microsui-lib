# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## \[0.3.0] - 2025-08-30

- Added `microsui_http_post`, the first implementation of end-to-end transaction execution towards the Sui Network using HTTP(S) POST.
  This is the cornerstone that enables embedded devices to send signed transactions directly to a Sui fullnode RPC.
- Introduced an OO-style API layer (object-oriented style in pure C using structs + function pointers).
  - Added `Keypair.c` with `MicroSuiEd25519` struct: supports key generation, loading from Bech32, signing, and deriving Sui addresses.
  - Added `Transaction.c` with `MicroSuiTransaction` struct: supports initialization, loading prebuilt TxBytes, building (placeholder), and memory cleanup.
  - Added `Client.c` with `MicroSuiClient` struct: supports creating a client bound to a RPC URL, signing & executing transactions, and executing with a prebuilt signature.
  - Added `WiFi.c` with `MicroSuiWiFi` struct: simplified connect/disconnect helpers for embedded use.
- Added `microsui_generate_tx_block_response_from_json`, which parses the JSON-RPC response from a Sui node into a structured `SuiTransactionBlockResponse`.
  This function uses the lightweight `jsmn` library for JSON decoding, making it suitable for embedded environments.  
  It allows code to easily access digest, checkpoint, balance changes, and execution flags in a strongly-typed way.
- Added `microsui_prepare_executeTransactionBlock`, which generates the JSON-RPC request body for `sui_executeTransactionBlock`.  
  It encodes the transaction bytes and signature into Base64 and builds a complete JSON string ready to be sent via `microsui_http_post` to a Sui RPC node.
- All modules are heavily documented with Doxygen-style comments for clarity and to help beginners understand the API.
- Added beginner-friendly examples demonstrating end-to-end transaction signing and execution from an ESP32 device.
- Refactored internal memory management: constructors allocate/own memory, and explicit `clear()`/`disconnect()` methods were added to avoid leaks.
- Set foundation for future expansion of the `Client` object to include blockchain data queries (get objects, balances, checkpoints, etc.).
- Updated _Arduino_ Library and _PlatformIO_ Library versions to `0.3.0`.

## \[0.2.2] - 2025-07-19

- Added Arduino library compatibility, now MicroSui is available to be used as library in Arduino IDE. Some changes had to be made to the structure of the libraries to make them compatible with Arduino.

## \[0.2.1] - 2025-06-23

- Added `library.json` to standardize the library for use as a PlatformIO library.

## \[0.2.0] - 2025-06-20

- Added Bech32 support, enabling encoding and decoding of private keys compatible with Sui.
- Included usage examples for Bech32 operations.

## \[0.1.3] - 2025-06-19

- Refactored utility functions: moved helper functions from `sign.c` to dedicated `utils.c` file.

## \[0.1.2] - 2025-06-07

- Renamed function `sign_offline` to `microsui_sign_message` for clearer intent.

## \[0.1.1] - 2025-06-07

- Improved repository structure according to C library standards.
- Added `Makefile` configurations for build automation.

## \[0.1.0] - 2025-05-23

- Initial release introducing `sign_offline` function with example usage of signing with a hexadecimal private key.
