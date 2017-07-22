/* linker script, modified from osdev bare bones tutorial. */ 

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
