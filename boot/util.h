#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include "type.h"

// write a byte to a port
void outb(uint16_t port, uint8_t value);

// read a byte from a port
uint8_t inb(uint16_t port);

// read a word from a port
uint16_t inw(uint16_t port);

#endif
