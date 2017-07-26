#include "kernel.h"

/* 
For GDT settings.
This is for practicing, the GDT already set up via GRUB is equivalent to the following we sets.
*/

/* GDT table length */
#define GDT_LENGTH 5

/* Declaration of GDT */
gdt_entry_t gdt_entries[GDT_LENGTH];

/* GDTR */
gdt_ptr_t gdt_ptr;

/* 
Insert entry to GDT, the index represented as the number of the entries.
Actually we have only 5 entries, that is, use only uint8_t size.
*/
static void gdt_set_entry(uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

/* 
Initialized GDT.
Using the Flat Mode, only one segment at base address 0x000000.
Partitioned to 5 entries:
    1. NULL entry (enforced)
    2. Code segment
    3. Data segment
    4. User mode code segment
    5. User mode data segment
After setting up each entries, update/flush the GDT to a new one. 
*/
void init_gdt() {
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    /* Set up flat mode */
    gdt_set_entry(0, 0, 0, 0, 0); /* The NULL entry on the above */
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
}