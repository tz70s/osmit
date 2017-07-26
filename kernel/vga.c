#include "kernel.h"

#define WIDTH 80
#define HEIGHT 25

/* VGA buffer address(Memory map I/O), 0xB8000 */
static uint16_t *vga_memory = (uint16_t*)0xB8000;

/* Cursor position */
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static void cursor();

void screen_init() {
    uint8_t color_byte = (VGA_COLOR_BLACK << 4) | (VGA_COLOR_BLACK & 0x0f);
    int i;
    /* Simply set all memory objects to all black attribute */
    for(i=0; i<80*25; i++) {
        vga_memory[i] = 0x20 | (color_byte << 8);
    }
    cursor_x = 0;
    cursor_y = 0;

    /* TODO : Initialized Cursor */
    
	cursor();
}

/* Cursor */
static void cursor() {
    uint16_t cursorLocation = cursor_y*80 + cursor_x;

    /*
    Two I/O register involved,
    $14 -> the higher 8bits of cursor location
    $15 -> the lower 8bits of cursor location
    0x3D4 represented as telling I/O we want to set cursor
    0x3D5 represented as setting cursor location
    */

    outb(0x3D4, 0x0E);
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 0x0F);
    outb(0x3D5, cursorLocation);
}

/* Put a character on the screen */
void screen_putc(char c, vga_color_t back, vga_color_t front) {
    /*
    The VGA buffer put from the address 0xB8000.
    Then read the memory above, each two bytes represented as a character.
    The color (back, front) puts on the higher 8 bits, and the ASCII code puts on the lower 8 bits.    
    */
    uint8_t color_byte = (back << 4) | (front & 0x0f);

    switch(c) {
        case '\n':
            cursor_x = 0;
            cursor_y++;
            cursor();
            return;
        default:
            if (c >= ' ') {
                vga_memory[cursor_y*80 + cursor_x] = c | (color_byte << 8);
                cursor_x++;
            }
    }

    /* newline */
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    cursor();
}

/* put a string */
void screen_puts(char *s, vga_color_t back, vga_color_t front) {
    while(*s) {
        screen_putc(*s, back, front);
        s++;
    }
}
