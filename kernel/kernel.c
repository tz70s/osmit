#include "kernel.h"

void kernel_main(void) {
	
	/* initialized gdt */
	init_gdt();

	/* initialized itd */
	init_idt();

	screen_init();
	screen_putc('H', VGA_COLOR_BLACK, VGA_COLOR_GREEN);
	screen_putc('i', VGA_COLOR_BLACK, VGA_COLOR_BLUE);
	screen_putc('!', VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
	screen_putc('\n', VGA_COLOR_BLACK, VGA_COLOR_BLUE);

	
	screen_puts( \
	"\n* This is a BABY i386 kernel implementation.\n" \
	"Mostly reference to MIT xv6 and OSdev forum\n" \
	"Author @tz70s in 2017.\n" \
	"Start kernel...\n", VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);

	__asm__ __volatile__("int $0x00");
	__asm__ __volatile__("int $0x03");
	__asm__ __volatile__("int $0x04");
	
}