# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
