#include "kernel.h"
#include "libc.h"

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

/* IRQ isr */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq16();
extern void irq17();
extern void irq18();
extern void irq19();

/* Function pointer to ISR. */
typedef void (*interrupt_handler_t)(pt_regs *);

/* Interrupt handlers */
interrupt_handler_t interrupt_handlers[256];

/* Interrupt handler registration */
void interrupt_handler_register(uint8_t n, interrupt_handler_t h) {
    interrupt_handlers[n] = h;    
}

static void irq_handler(pt_regs *regs) {
    /* Send reset interrupt signal to PICs */
    if (regs->int_num >= 40) {
        /* Send signal to slave */
        outb(0xA0, 0x20);
    }
    /* Send signal to master */
    outb(0x20, 0x20);
    if (interrupt_handlers[regs->int_num]) {
        interrupt_handlers[regs->int_num](regs);
    } else {
        screen_puts("\nUnhandled interrupt", VGA_COLOR_BLACK, VGA_COLOR_GREEN);
        screen_putc('0' + regs->int_num, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
        screen_putc('\n', VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    }
}
/* Interrupt handler */
void isr_handler(pt_regs *regs){

    /* If IRQ, jump to irq_handler */
    if ((regs->int_num >= 32) && (regs->int_num <= 47)) {
        irq_handler(regs);
    } else {
        if (interrupt_handlers[regs->int_num]) {
            interrupt_handlers[regs->int_num](regs);
        } else {
            screen_puts("\nUnhandled interrupt", VGA_COLOR_BLACK, VGA_COLOR_GREEN);
            screen_putc('0' + regs->int_num, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
            screen_putc('\n', VGA_COLOR_BLACK, VGA_COLOR_GREEN);
        }   
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
static uint32_t tick = 0;
static uint8_t count = 0;
 
/* Timer IRQ handler */
static void timer_callback(pt_regs *regs) {
    tick++;
    if (tick % 100 == 0) {
        count++;
        screen_putc_stay('0'+(uint8_t)count%10, VGA_COLOR_BROWN, VGA_COLOR_LIGHT_CYAN);
    }
}

/* Init timer */
void init_timer(uint32_t frequency) {
    interrupt_handler_register(32, timer_callback);
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

/* Initialized IRQ */
static void init_irq() {
    /*
    Intel 8259A (PIC) have 8 interrupt numbers each. 
    Intel architecture uses two 8259A (master, slave) for irq extension.
    The master IRQ port - 0x20, 0x21
    The slave IRQ port - 0xA0, 0xA1 
    */

    /* For initialization */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    /* Set IRQ Master PIC start at interrupt number 32(0x20) */
    outb(0x21, 0x20);

    /* Set IRQ Slave PIC start at interrupt number 40(0x28) */
    outb(0xA1, 0x28);

    /* Set IRQ Master pin IR2 connect to Slave */
    outb(0x21, 0x04);

    /* Set IRQ Slave connect to master */
    outb(0xA1, 0x02);

    /* Set PIC works in 8086 mode */
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* Allow interrupt */
    outb(0x21, 0x0);
    outb(0xA1, 0x0); 
} 

/* IDT settings */
void init_idt() {

    /* Initialized PIC */
    init_irq();

    /* Initialized Interrupt handlers[] and Idt_entries[] */
    bzero(idt_entries, 256 * sizeof(idt_entry_t));
    bzero(interrupt_handlers, 256 * sizeof(interrupt_handler_t));
    
    /* Set idt pointer (lidt) */
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

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
    
    /* IRQ */
    idt_set_entry(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_entry(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_entry(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_entry(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_entry(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_entry(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_entry(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_entry(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_entry(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_entry(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_entry(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_entry(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_entry(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_entry(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_entry(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_entry(47, (uint32_t)irq15, 0x08, 0x8E);
    
    __asm__ __volatile__("lidt (%0)" :: "r"(&idt_ptr));
    
}
