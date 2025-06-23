# microsui-lib

## Let's begin:

### Build all the examples:

In root, run

```bash
make
```

You will find the `.out` binaries to run in each example folder.

## Project Directory Structure:

```yaml
microsui-lib/
├── include/                 # Public header files (API)
│   └── microsui/
│       ├── sign.h
│       ├── cryptography.h
│       └── utils.h
├── src/                     # Library source files (.c)
│   ├── sign.c
│   ├── cryptography.c
│   ├── utils.c
├── lib/                    # Third-party dependencies
│   ├── monocypher/
│   └── compact25519/
│       └── c25519/
├── examples/                # Usage examples
│   ├── offline_sign/
│   ├── offline_sign_with_bech32_privkey/
│   └── encode_decode_privkey/
├── library.json             # Enables compatibility for standard libraries in environments such as PlatformIO
└── Makefile                 # Root Makefile to build all examples
```

## ✅ Implemented So Far

### 📚 Library (`/src`)

- `sign.c`:  
  Core implementation of the message signing logic using `monocypher` and `compact25519`.
- `cryptography.c`:  
  Core cryptographic utilities for encoding, decoding, key transformations, and other format-related operations, including Bech32 support.
- `utils.c`:  
  Useful complementary functions to be used by other library functions or by the user.

### 📂 Public Headers (`/include`)

- `microsui/sign.h`:  
  Public API header exposing the MicroSui Sign functions.
- `microsui/cryptography.h`:  
  Public API header exposing the MicroSui Cryptography functions.
- `microsui/utils.h`:  
  Public API header exposing useful complementary functions.

### 📦 Dependencies (`/lib`)

- `monocypher`:  
  Lightweight cryptographic primitives.
- `compact25519`:  
  Ed25519 key format support.

### 🧪 Examples

#### Offline Sign (`/examples/offline_sign`)

- `offline_sign.c`:  
  Demonstrates how to sign a Sui transaction offline using a raw 32-byte private key.
- `constants.c` & `constants.h`:  
  Helper files providing a test private key and related constants.
- `Makefile`:  
  Builds the example with all dependencies automatically.
- Output binary: `offline_sign.out`

#### Offline Sign with Bech32 Private Key (`/examples/offline_sign_with_bech32_privkey`)

- `offline_sign_with_bech32_privkey.c`:  
  Demonstrates how to sign a Sui transaction offline using a Sui compatible bech32 private key.
- `constants.c` & `constants.h`:  
  Helper files providing a test private key and related constants.
- `Makefile`:  
  Builds the example with all dependencies automatically.
- Output binary: `offline_sign_with_bech32_privkey.out`

#### Encode & Decode Bech32 Private Keys (`/examples/encode_decode_privkey`)

- `encode_decode_privkey.c`:  
  Demonstrates how to encode and decode Sui compatible bech32 private keys.
- `constants.c` & `constants.h`:  
  Helper files providing a test private key and related constants.
- `Makefile`:  
  Builds the example with all dependencies automatically.
- Output binary: `encode_decode_privkey.out`

### 🛠️ Global Makefile

- At the root level:  
  Detects and builds all available examples automatically.

## 👽 PlatformIO Compatibility

This library is fully compatible with PlatformIO and is listed in the official PlatformIO Library Registry.

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gustavogb/library/microsui-lib.svg)](https://registry.platformio.org/libraries/gustavogb/microsui-lib)
