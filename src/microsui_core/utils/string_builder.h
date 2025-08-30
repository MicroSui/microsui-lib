#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char*  data;
    size_t len;
    size_t cap;
    size_t cap_max;
} StringBuilder;

int sb_init(StringBuilder* sb, size_t initial_cap, size_t cap_max);
void sb_free(StringBuilder* sb);
int sb_ensure(StringBuilder* sb, size_t addlen);
int sb_append(StringBuilder* sb, const char* s);
int sb_append_char(StringBuilder* sb, char c);
int sb_appendf(StringBuilder* sb, const char* fmt, ...);
char* sb_detach(StringBuilder* sb);

static inline size_t sb_length(const StringBuilder* sb) {
    return sb ? sb->len : 0;
}

static inline const char* sb_data(const StringBuilder* sb) {
    return sb ? sb->data : NULL;
}

static inline int sb_reserve(StringBuilder* sb, size_t min_cap) {
    if (!sb) return 0;
    if (min_cap <= sb->cap) return 1;
    size_t need = (min_cap > sb->len + 1) ? (min_cap - sb->len - 1) : 0;
    return sb_ensure(sb, need);
}

#endif
