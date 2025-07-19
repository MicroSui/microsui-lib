ROOT_DIR := ../..
SRC := $(ROOT_DIR)/src
INCLUDE := $(ROOT_DIR)/include
LIB := $(ROOT_DIR)/src/lib

MONOCYPHER := $(LIB)/monocypher
COMPACT := $(LIB)/compact25519
C25519 := $(COMPACT)/c25519

CFLAGS := -Wall -Wextra -I$(INCLUDE) -I$(SRC) -I$(LIB) -I$(MONOCYPHER) -I$(COMPACT) -I$(C25519)

SOURCES := offline_sign.c constants.c \
           $(SRC)/*.c \
           $(wildcard $(MONOCYPHER)/*.c) \
           $(wildcard $(COMPACT)/*.c) \
           $(wildcard $(C25519)/*.c)

OUTPUT := offline_sign.out

all: clean_before_build $(OUTPUT)

$(OUTPUT): $(SOURCES)
	@echo "Compiling example: $@"
	gcc $(CFLAGS) $^ -o $@

.PHONY: clean clean_before_build

clean_before_build:
	@echo "Cleaning old binary if it exists..."
	@rm -f $(OUTPUT)

clean:
	@echo "Removing binary..."
	@rm -f $(OUTPUT)
