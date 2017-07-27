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
volatile gdt_ptr_t gdt_ptr;

/* 
Insert entry to GDT, the index represented as the number of the entries.
Actually we have only 5 entries, that is, use only uint8_t size.
*/
static void gdt_set_entry(uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
extern void gdt_update();
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

    __asm__ __volatile__("lgdt (%0)" :: "r"(&gdt_ptr));

    /* update/flush the gdt table */
    gdt_update();
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
/* 
When an interrupt occured, at a specific time, CPU save current state and jump to handle interrupt(ISR).
This is the memory layout of saving state.
*/
typedef struct pt_regs_t {
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_num;
    uint32_t err_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} pt_regs;

/* 
Trap, defined in cpu_vector_x86.S
The interrupt number 0 ~ 31 preserved by Intel CPU.That is, the number 32 ~ 255 can be defined by users. 
The trap numbers are from 0 to 19.
*/

extern void trap0();
extern void trap1();
extern void trap2();
extern void trap3();
extern void trap4();
extern void trap5();
extern void trap6();
extern void trap7();
extern void trap8();
extern void trap9();
extern void trap10();
extern void trap11();
extern void trap12();
extern void trap13();
extern void trap14();
extern void trap15();
extern void trap16();
extern void trap17();
extern void trap18();
extern void trap19();
extern void trap20();

/* Function pointer to ISR. */
typedef void (*interrupt_handler_t)(pt_regs *);

/* Interrupt handler registration */
void interrupt_handler_register(uint8_t n, interrupt_handler_t h);

/* Interrupt handlers */
interrupt_handler_t interrupt_handlers[256];

/* Interrupt handler */
void isr_handler(pt_regs *regs){
    if (interrupt_handlers[regs->int_num]) {
        interrupt_handlers[regs->int_num](regs);
    } else {
        screen_puts("\nUnhandled interrupt", VGA_COLOR_BLACK, VGA_COLOR_GREEN);
        screen_putc('0' + regs->int_num, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
        screen_putc('\n', VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    }   
}

/* IDT */
idt_entry_t idt_entries[256];

/* IDTR */
idt_ptr_t idt_ptr;

/* Set each entry */
static void idt_set_entry(uint8_t num, uint32_t off, uint16_t sel, uint8_t flags) {
    idt_entries[num].off_low = off & 0xFFFF;
    idt_entries[num].off_high = (off >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].alwayszero = 0;
    idt_entries[num].flags = flags;
}

/* set a sample interrupt handler */
static void sample_interrupt_divide_by_zero(pt_regs * regs) {
    screen_puts("\nDivided by ZERO, interrupt(trap) no.", VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    screen_putc('0' + regs->int_num, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
    screen_putc('\n', VGA_COLOR_BLACK, VGA_COLOR_GREEN);
}

/* IDT settings */
void init_idt() {

    /* TODO: Initialized Interrupt handlers[] and Idt_entries[] */
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    interrupt_handlers[0] = sample_interrupt_divide_by_zero;
    idt_set_entry(0, (uint32_t)trap0, 0x08, 0x8E);
    idt_set_entry(1, (uint32_t)trap1, 0x08, 0x8E);
    idt_set_entry(2, (uint32_t)trap2, 0x08, 0x8E);
    idt_set_entry(3, (uint32_t)trap3, 0x08, 0x8E);
    idt_set_entry(4, (uint32_t)trap4, 0x08, 0x8E);
    idt_set_entry(5, (uint32_t)trap5, 0x08, 0x8E);
    idt_set_entry(6, (uint32_t)trap6, 0x08, 0x8E);
    idt_set_entry(7, (uint32_t)trap7, 0x08, 0x8E);
    idt_set_entry(8, (uint32_t)trap8, 0x08, 0x8E);
    idt_set_entry(9, (uint32_t)trap9, 0x08, 0x8E);
    idt_set_entry(10, (uint32_t)trap10, 0x08, 0x8E);
    idt_set_entry(11, (uint32_t)trap11, 0x08, 0x8E);
    idt_set_entry(12, (uint32_t)trap12, 0x08, 0x8E);
    idt_set_entry(13, (uint32_t)trap13, 0x08, 0x8E);
    idt_set_entry(14, (uint32_t)trap14, 0x08, 0x8E);
    idt_set_entry(15, (uint32_t)trap15, 0x08, 0x8E);
    idt_set_entry(16, (uint32_t)trap16, 0x08, 0x8E);
    idt_set_entry(17, (uint32_t)trap17, 0x08, 0x8E);
    idt_set_entry(18, (uint32_t)trap18, 0x08, 0x8E);
    idt_set_entry(19, (uint32_t)trap19, 0x08, 0x8E);
    idt_set_entry(20, (uint32_t)trap20, 0x08, 0x8E);
    
    __asm__ __volatile__("lidt (%0)" :: "r"(&idt_ptr));
    
}