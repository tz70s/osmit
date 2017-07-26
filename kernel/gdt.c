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
extern void gdt_update(uint32_t);
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

    /* update/flush the gdt table */
    gdt_update((uint32_t)&gdt_ptr);
}

/* Set to the corresponding bits */
static void gdt_set_entry(uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[index].base_low = base & 0xFFFF;
    gdt_entries[index].base_middle = (base >> 16) & 0xFF;
    gdt_entries[index].base_high = (base >> 24) & 0xFF;

    gdt_entries[index].limit = limit & 0xFFFF;
    /* limit 16 ~ 19, the remain 4 bits */
    gdt_entries[index].granularity = (limit >> 16) & 0x0F;
    /* granularity, AVL, L, D/B, G. Bits 20 ~ 23*/
    gdt_entries[index].granularity |= gran & 0xF0;
    /* access, Type, S, DPL, P. Bits 8 ~ 15 in first 32 bits */
    gdt_entries[index].access = access;
}