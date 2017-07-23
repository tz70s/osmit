#ifndef INCLUDE_KERNEL_H_
#define INCLUDE_KERNEL_H_

/* Type defintions for conveience. */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
// no 64 bits in this os

/* I/O operation definition.*/

// put a byte to an output port
inline void outb(uint16_t port, uint8_t value) {
	__asm__ __volatile__(
			"outb %1, %0"
			: 
			: "dN" (port), "a" (value));
}

// get a byte from an output port
inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__(
			"inb %1, %0"
			: "=a" (ret)
		    : "dN" (port) 	
			);
	return ret;
}

// get a word from an output port
inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	__asm__ __volatile__ (
			"inw %1, %0"
			: "=a" (ret)
			: "dN" (port)
			);
	return ret;
}


/* VGA settings. */

/* Hardware text mode color constants. */
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

/* Output a string */
void screen_puts(char *s, vga_color_t back, vga_color_t front);

#endif
