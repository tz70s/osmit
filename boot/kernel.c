#include <stddef.h>
#include <stdint.h>
#include "vga.h"

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
void kernel_main(void) {
	
	screen_init();
	screen_putc('H', VGA_COLOR_BLACK, VGA_COLOR_GREEN);
	screen_putc('i', VGA_COLOR_BLACK, VGA_COLOR_BLUE);
	screen_putc('!', VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
	screen_putc('\n', VGA_COLOR_BLACK, VGA_COLOR_BLUE);

	
	screen_puts( \
	"This is a BABY i386 kernel implementation.\n" \
	"Author @tz70s in 2017. Contact: su3g4284zo6y7@gmail.com\n" \
	"Mostly reference to MIT xv6 and OSdev forum\n" \
	"Start kernel...\n");
	
}