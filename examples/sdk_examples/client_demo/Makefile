ROOT_DIR := ../../..
SRC := $(ROOT_DIR)/src
INCLUDE := $(ROOT_DIR)/include
MICROSUI_CORE := $(ROOT_DIR)/src/microsui_core
UTILS := $(MICROSUI_CORE)/utils
LIB := $(MICROSUI_CORE)/lib

MONOCYPHER := $(LIB)/monocypher
COMPACT := $(LIB)/compact25519
C25519 := $(COMPACT)/c25519
JSMN := $(LIB)/JSMN

HTTP_CURL_FILE = $(MICROSUI_CORE)/impl/http/http_curl.c # This Makefile needs cURL installed in the system
HTTP_UNSUPPORTED_FILE = $(MICROSUI_CORE)/impl/http/http_unsupported.c # This file is used when make is made on a system without cURL support
HTTP_LIBS = -lcurl $(if $(filter Windows_NT,$(OS)),-lws2_32 -lbcrypt -lwldap32 -lcrypt32)

WIFI_DESKTOP_FILE = $(MICROSUI_CORE)/impl/wifi/wifi_desktop.c
WIFI_UNSUPPORTED_FILE = $(MICROSUI_CORE)/impl/wifi/wifi_unsupported.c # This file is used when make is made on a system without WiFi support

CFLAGS := -Wall -Wextra -I$(INCLUDE) -I$(SRC) -I$(MICROSUI_CORE) -I$(UTILS) -I$(LIB) -I$(MONOCYPHER) -I$(COMPACT) -I$(C25519) -I$(JSMN)

SOURCES := client_demo.c \
			$(SRC)/*.c \
			$(MICROSUI_CORE)/*.c \
			$(UTILS)/*.c \
			$(JSMN)/*.c \
			$(wildcard $(MONOCYPHER)/*.c) \
			$(wildcard $(COMPACT)/*.c) \
			$(wildcard $(C25519)/*.c) \
			$(HTTP_CURL_FILE) \
			$(HTTP_UNSUPPORTED_FILE) \
			$(WIFI_DESKTOP_FILE) \
			$(WIFI_UNSUPPORTED_FILE)

OUTPUT := client_demo.out

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