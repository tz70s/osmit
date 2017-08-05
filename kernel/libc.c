/* Implementation of common c utils */

#include "libc.h"

/* memcpy of char */
void memcpy(uint8_t *dest, const uint8_t * src, uint32_t len) {
    for (; len > 0; len--) {
        *dest++ = *src++;
    }
}

/* memery set */
void memset(void *dest, uint8_t val, uint32_t len) {
    uint8_t *tmp_dst = (uint8_t*) dest;
    for (; len > 0; len--) {
        *tmp_dst++ = val;
    }
}

/* Set all to zero */
void bzero(void *dest, uint32_t len) {
    memset(dest, '\0', len);
}

/* Compare two string */
int strcmp(const char *str1, const char *str2) {
    for (;*str1; str1++, str2++) {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
    }
    return 0;
}

/* Copy the second string to the first */
char *strcpy(char *dest, const char *src) {
    for(; *src; dest++, src++) {
        *dest = *src;
    }
    return dest;
}

/* Concact the second string after the first */
char *strcat(char *dest, const char *src) {
    char *tmp = dest;
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }
    /* return the first-byte address */
    return tmp;
}

/* count the string length */
int strlen(const char *src) {
    int len = 0;
    while (*src) {
        len++;
    }
    return len;
}