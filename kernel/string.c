#include "string.h"

int strlen(const char *str) {
    int len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

void *memset(void *s, char c, size_t n) {
    char *p = (char *)s;
    while (n--) {
        *p++ = c;
    }
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    char *p = (char *)dest;
    while (n--) {
        *p++ = *(char *)src++;
    }
    return dest;
}

void *strcpy(void *dest, const void *src) {
    char *p = (char *)dest;
    while (*p++ = *(char *)src++);
    return dest;
}