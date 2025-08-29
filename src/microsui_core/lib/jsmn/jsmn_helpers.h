// jsmn_helpers.h
// Small navigation helpers on top of jsmn's flat token array.
#ifndef JSMN_HELPERS_H
#define JSMN_HELPERS_H

#include <stddef.h>
#include "jsmn.h"

// Compares a STRING token with a C string literal. Returns 0 if equal.
int jsmn_jsoneq(const char *json, const jsmntok_t *tok, const char *s);

// Copies token text into out (NUL-terminated; truncated if needed).
void jsmn_tok_copy_cstr(const char *json, const jsmntok_t *tok,
                        char *out, size_t out_sz);

// Skips a token subtree and returns the index of the next sibling.
// Works without JSMN_PARENT_LINKS by walking children via `size`.
int jsmn_skip_token(const jsmntok_t *toks, int idx);

// Inside an OBJECT token, find the value token for `key`. Returns 0 on success.
int jsmn_object_get_value_idx(const char *json, const jsmntok_t *toks, int tokc,
                              int obj_idx, const char *key, int *out_val_idx);

// Finds the ARRAY token value for a given key name by scanning the stream.
// Returns the index of the ARRAY token, or <0 if not found.
int jsmn_find_array_by_key_anywhere(const char *json, const jsmntok_t *toks, int tokc,
                                    const char *key);

// Given an ARRAY token, returns the index of its Nth element (0-based), or <0 on error.
int jsmn_array_nth_child_idx(const jsmntok_t *toks, int tokc, int arr_idx, int index);

#endif