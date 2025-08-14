ROOT_DIR := ../../..
SRC := $(ROOT_DIR)/src
UTILS := $(SRC)/utils
INCLUDE := $(ROOT_DIR)/include
LIB := $(ROOT_DIR)/src/lib

MONOCYPHER := $(LIB)/monocypher
COMPACT := $(LIB)/compact25519
C25519 := $(COMPACT)/c25519

HTTP_LIBS = -lcurl $(if $(filter Windows_NT,$(OS)),-lws2_32 -lbcrypt -lwldap32 -lcrypt32)

CFLAGS := -Wall -Wextra -I$(INCLUDE) -I$(SRC) -I$(UTILS) -I$(LIB) -I$(MONOCYPHER) -I$(COMPACT) -I$(C25519)

SOURCES := prepare_transaction_json.c \
           $(SRC)/*.c \
           $(UTILS)/*.c \
           $(wildcard $(MONOCYPHER)/*.c) \
           $(wildcard $(COMPACT)/*.c) \
           $(wildcard $(C25519)/*.c)

OUTPUT := prepare_transaction_json.out

all: clean_before_build $(OUTPUT)

$(OUTPUT): $(SOURCES)
	@echo "Compiling example: $@"
	gcc $(CFLAGS) $^ -o $@ $(HTTP_LIBS)

.PHONY: clean clean_before_build

clean_before_build:
	@echo "Cleaning old binary if it exists..."
	@rm -f $(OUTPUT)

clean:
	@echo "Removing binary..."
	@rm -f $(OUTPUT)
