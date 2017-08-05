#ifndef __LIBC_H_
#define __LIBC_H_

/* Type defintions for conveience. */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
/* notice: no 64 bits in this os */

/* Implementation of some common c standard library */

/* memcpy of char */
void memcpy(uint8_t *dest, const uint8_t * src, uint32_t len);

/* memery set */
void memset(void *dest, uint8_t val, uint32_t len);

void bzero(void *dest, uint32_t len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

int strlen(const char *src);

void printk();

#endif
