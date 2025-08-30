# MicroSui C Library --- _microsui-lib_

MicroSui Library is a lightweight C library designed primarily for embedded systems to interact with the Sui Network Blockchain.

Full compatibility with _ESP32 microcontrollers_ and _traditional desktop OS_. More device support coming soon.

This library is **_Arduino and PlatformIO Compatible_**

[![microsui-lib on Arduino Registry](https://www.ardu-badge.com/badge/MicroSui.svg)](https://www.ardu-badge.com/MicroSui/ide)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gustavogb/library/microsui-lib.svg)](https://registry.platformio.org/libraries/gustavogb/microsui-lib)

## The Library:

### Build all the examples:

#### Requirements:

In Windows, macOS or Linux it will be necessary to install `cURL`:

##### In Linux:

```
sudo apt install -y libcurl4-openssl-dev pkg-config build-essential
```

##### In macOS:

```
brew install curl pkg-config
```

##### In Windows:

```
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-curl make
```

#### Compile and Build examples and tests

In root, run

```bash
make
```

You will find the `.out` binaries to run in each example folder.

## Project Directory Structure:

```yaml
microsui-lib/
├── include/                        # Public header files (API)
│   └── microsui/
│       └── *.h
├── src/                            # Library source files (.c)
│   ├── microsui_core
│   │   ├── lib/                    # Third-party dependencies
│   │   │   ├── monocypher/
│   │   │   ├── compact25519/
│   │   │   │   └── c25519/
│   │   │   └── jsmn/
│   │   ├── impl/                   # Platform/board specific implementation
│   │   │   ├── http/
│   │   │   └── wifi/
│   │   ├── utils/                  # MicroSui utils
│   │   ├── byte_conversions.c
│   │   ├── cryptography.c
│   │   ├── http_router.h  #(implementation)
│   │   ├── key_managment.c
│   │   ├── rpc_json_builder.c
│   │   ├── sign.c
│   │   └── wifi_router.h  #(implementation)
│   ├── Client.c                    # OO-style Client.c
│   ├── Keypair.c                   # OO-style Keypair.c
│   ├── Transaction.c               # OO-style Transaction.c
│   ├── WiFi.c                      # OO-style WiFi.c
│   └── MicroSui.h                  # Arduino header (must be in src)
├── examples/                       # Usage examples
│   ├── ArduinoExamples/
│   ├── core-examples/
│   ├── sdk-examples/
├── library.properties              # Enables Arduino library compatibility
├── library.json                    # Enables PlatformIO library compatibility
└── Makefile                        # Root Makefile to build all examples
```

### 📦 Dependencies (`/lib`)

- `monocypher`:  
  Lightweight cryptographic primitives.
- `compact25519`:  
  Ed25519 key format support.
- `jsmn`:
  Lightweight minimalistic JSON parser for C.

### 🛠️ Global Makefile

- At the root level:  
  Detects and builds all available examples automatically.

## ♾ Arduino Compatibility

This library is fully compatible with Arduino and is listed in the official Arduino Library Registry.

[![microsui-lib on Arduino Registry](https://www.ardu-badge.com/badge/MicroSui.svg)](https://www.ardu-badge.com/MicroSui/ide)

## 👽 PlatformIO Compatibility

This library is fully compatible with PlatformIO and is listed in the official PlatformIO Library Registry.

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gustavogb/library/microsui-lib.svg)](https://registry.platformio.org/libraries/gustavogb/microsui-lib)
