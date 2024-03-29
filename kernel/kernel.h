#ifndef INCLUDE_KERNEL_H_
#define INCLUDE_KERNEL_H_

#include "libc.h"
/* I/O operation definition.*/

/* put a byte to an output port */
static inline void outb(uint16_t port, uint8_t value) {
	__asm__ __volatile__(
			"outb %1, %0"
			: 
			: "dN" (port), "a" (value));
}

/* put a word to an output port */
static inline void outw(uint16_t port, uint16_t value) {
	__asm__ __volatile__(
			"outb %1, %0"
			: 
			: "dN" (port), "a" (value));
}

/* get a byte from an output port */
static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__(
			"inb %1, %0"
			: "=a" (ret)
		    : "dN" (port) 	
			);
	return ret;
}

/* get a word from an output port */
static inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	__asm__ __volatile__ (
			"inw %1, %0"
			: "=a" (ret)
			: "dN" (port)
			);
	return ret;
}

/* GDT settings */

/*
Segment descriptor, 64bits totally.
From the below to the top of memory placement.

The first 32 bits
Segment limit - 0 ~ 15
Base Address - 0 ~ 15

The second 32 bits
Base Address - 0 ~ 7
Type - 8 ~ 11
S (descriptor type) - 12
DPL (descriptor privilege level) - 13 ~ 14
P (segment present) - 15
Segment limit - 16 ~ 19 (Totally 20 bits)
AVL (Avaiable for use by system software) - 20
L (64-bit code segment, IA-32 mode only) - 21
D/B (Default operation size, 0=16bit segment, 1=32bit segment) - 22
G (Granularity) - 23 
Base Address - 24 ~ 31 (Totally 32 bits)

*/
typedef struct gdt_entry_t {
	uint16_t limit; 
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed, aligned(1))) gdt_entry_t;

/* GDTR 48 bits - higher 32 bits for base address and lower 16 bits for limit */
typedef struct gdt_ptr_t {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed, aligned(1))) gdt_ptr_t;

void init_gdt();

/* Interrupt settings */

/*
Interrupt Gate/Descriptor, two 32-bits as an entry.
Similar to segment descriptor.

Offset - 0 ~ 15
Segment Selector - 16 ~ 31

Always Zero - 0 ~ 7
Flags - 8 ~ 12
DPL(Ring) - 13 ~ 14
P (Segment present) - 15
Offset - 16 ~ 31
*/
typedef struct idt_entry_t {
	uint16_t off_low;
	uint16_t sel;
	uint8_t alwayszero;
	uint8_t flags;
	uint16_t off_high;
} __attribute__((packed)) idt_entry_t;

/* IDTR */
typedef struct idt_ptr_t {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

/* init idt */
void init_idt();

void init_timer(uint32_t);
/* VGA settings. */

/* Hardware text mode color constants. Formal declaration. */
typedef enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_t;

/* Clean the vga screen */
void screen_init();

/* Output a character */
void screen_putc(char c, vga_color_t back, vga_color_t front);

/* Output a character */
void screen_putc_stay(char c, vga_color_t back, vga_color_t front);


/* Output a string */
void screen_puts(char *s, vga_color_t back, vga_color_t front);

#endif
