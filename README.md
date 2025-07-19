# microsui-lib

This library is **_Arduino and PlatformIO Compatible_**

[![microsui-lib on Arduino Registry](https://img.shields.io/badge/Arduino-microsui--lib-blue.svg)](https://registry.arduino.cc/libraries/microsui-lib)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gustavogb/library/microsui-lib.svg)](https://registry.platformio.org/libraries/gustavogb/microsui-lib)

## The Library:

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
│   ├── lib/                 # Third-party dependencies
│   │   ├── monocypher/
│   │   └── compact25519/
│   │       └── c25519/
│   ├── sign.c
│   ├── cryptography.c
│   ├── utils.c
│   └── MicroSui.h           # Arduino header (must be in src)
├── examples/                # Usage examples
│   ├── ArduinoExamples/
│   ├── offline_sign/
│   ├── offline_sign_with_bech32_privkey/
│   └── encode_decode_privkey/
├── library.properties       # Enables Arduino library compatibility
├── library.json             # Enables PlatformIO library compatibility
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

## ♾ Arduino Compatibility

This library is fully compatible with Arduino and is listed in the official Arduino Library Registry.

[![microsui-lib on Arduino Registry](https://img.shields.io/badge/Arduino-microsui--lib-blue.svg)](https://registry.arduino.cc/libraries/microsui-lib)

## 👽 PlatformIO Compatibility

This library is fully compatible with PlatformIO and is listed in the official PlatformIO Library Registry.

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gustavogb/library/microsui-lib.svg)](https://registry.platformio.org/libraries/gustavogb/microsui-lib)
