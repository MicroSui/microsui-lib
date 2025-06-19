# microsui-lib

---

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
│       └── utils.h
├── src/                     # Library source files (.c)
│   ├── sign.c
│   ├── utils.c
├── lib/                    # Third-party dependencies
│   ├── monocypher/
│   └── compact25519/
│       └── c25519/
├── examples/                # Usage examples
│   └── offline_sign/
│       ├── constants.c
│       ├── constants.h
│       ├── offline_sign.c
│       └── Makefile
└── Makefile                 # Root Makefile to build all examples
```

## ✅ Implemented So Far

### 📚 Library (`/src`)

- `sign.c`:  
  Core implementation of the message signing logic using `monocypher` and `compact25519`.
- `utils.c`:  
  Useful complementary functions to be used by other library functions or by the user.

### 📂 Public Headers (`/include`)

- `microsui/sign.h`:  
  Public API header exposing the MicroSui Sign functions.
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

### 🛠️ Global Makefile

- At the root level:  
  Detects and builds all available examples automatically.
