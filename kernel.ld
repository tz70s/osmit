/* 
 * Linker script, modified from osdev bare bones tutorial.
 * The memory placement for the kernel, start from 1MB = 0x100000 byte address.
 * At the starting point, the stack is placed with bss section.
 */ 


ENTRY(_start)

SECTIONS
{
	/* Conventional kernel place load from bootloader */
	. = 1M;

	/* put multiboot header in the front */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot);
		*(.text);				
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata);			
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data);				
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON);
		*(.bss);			
	}
}
