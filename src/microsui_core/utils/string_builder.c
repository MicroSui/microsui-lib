#include "string_builder.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

int sb_init(StringBuilder* sb, size_t initial_cap, size_t cap_max) {
    if (!sb) return 0;
    if (initial_cap == 0) initial_cap = 64;
    sb->data = (char*)malloc(initial_cap);
    if (!sb->data) return 0;
    sb->len = 0;
    sb->cap = initial_cap;
    sb->cap_max = cap_max;
    sb->data[0] = '\0';
    return 1;
}

void sb_free(StringBuilder* sb) {
    if (sb && sb->data) {
        free(sb->data);
        sb->data = NULL;
    }
    if (sb) {
        sb->len = 0;
        sb->cap = 0;
        sb->cap_max = 0;
    }
}

int sb_ensure(StringBuilder* sb, size_t addlen) {
    if (!sb || !sb->data) return 0;
    size_t need = sb->len + addlen + 1; // +1 for null terminator
    if (need <= sb->cap) return 1;

    size_t new_cap = sb->cap;
    while (new_cap < need) {
        if (new_cap > (SIZE_MAX / 2)) return 0; // overflow guard
        new_cap *= 2;
        if (sb->cap_max && new_cap > sb->cap_max) {
            new_cap = sb->cap_max;
            if (new_cap < need) return 0; // exceeds max capacity
            break;
        }
    }
    char* p = (char*)realloc(sb->data, new_cap);
    if (!p) return 0;
    sb->data = p;
    sb->cap = new_cap;
    return 1;
}

int sb_append(StringBuilder* sb, const char* s) {
    if (!s) return 1;
    size_t sl = strlen(s);
    if (!sb_ensure(sb, sl)) return 0;
    memcpy(sb->data + sb->len, s, sl);
    sb->len += sl;
    sb->data[sb->len] = '\0';
    return 1;
}

int sb_append_char(StringBuilder* sb, char c) {
    if (!sb_ensure(sb, 1)) return 0;
    sb->data[sb->len++] = c;
    sb->data[sb->len] = '\0';
    return 1;
}

int sb_appendf(StringBuilder* sb, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    size_t avail = (sb->cap > sb->len) ? (sb->cap - sb->len) : 0;
    int n = vsnprintf(sb->data + sb->len, avail, fmt, ap);
    va_end(ap);

    if (n < 0) return 0;

    if ((size_t)n < avail) {
        sb->len += (size_t)n;
        return 1;
    }

    if (!sb_ensure(sb, (size_t)n)) return 0;
    va_start(ap, fmt);
    int n2 = vsnprintf(sb->data + sb->len, sb->cap - sb->len, fmt, ap);
    va_end(ap);
    if (n2 < 0) return 0;
    sb->len += (size_t)n2;
    return 1;
}

char* sb_detach(StringBuilder* sb) {
    char* p = sb->data;
    sb->data = NULL;
    sb->len = sb->cap = 0;
    return p;
}