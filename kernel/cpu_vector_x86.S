/* assembly for TRAP and IRQ settings */
.macro TRAP TRAP_NUM, ERRCODE
.global trap\TRAP_NUM
.type trap\TRAP_NUM, @function

trap\TRAP_NUM:
    cli
.if \ERRCODE
.else
    push $0
.endif
    push $\TRAP_NUM
    jmp isr_common_stub
.endm

TRAP 0,  0 /* #DE */
TRAP 1,  0 /* #DB */
TRAP 2,  0 /* #NMI */
TRAP 3,  0 /* #BP */
TRAP 4,  0 /* #OF */
TRAP 5,  0 /* #BR */
TRAP 6,  0 /* #UD */
TRAP 7,  0 /* #NM */
TRAP 8,  1 /* #DF */
TRAP 9,  0 /* Cross operation*/
TRAP 10, 1 /* #TS */
TRAP 11, 1 /* #NP */
TRAP 12, 1 /* #SS */
TRAP 13, 1 /* #GP */
TRAP 14, 1 /* #PF */
TRAP 15, 0 /* CPU preserved */
TRAP 16, 0 /* #MF */
TRAP 17, 1 /* #AC */
TRAP 18, 0 /* #MC */
TRAP 19, 0 /* #XM */
TRAP 20, 0 /* #VE */

/* Interrupt common stub */
isr_common_stub:
    /* Push edi, esi, ebp, esp, ebx, edx, ecx, eax */
    pusha
    movw %ds, %ax
    push %eax

    /* jump to kernel data segment */
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    
    push %esp
    call isr_handler
    add $4, %esp

    pop %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs
    movw %bx, %ss

    popa
    add $8, %esp
    iret
.end:

/* IRQ settings */

.macro IRQ IRQ_NUM, INT_NUM
.global irq\IRQ_NUM
.type irq\IRQ_NUM, @function

irq\IRQ_NUM:
    cli
    push $0
    push $\INT_NUM
    jmp isr_common_stub
.endm

IRQ 0,  32 /* Clock */
IRQ 1,  33 /* Keyboard */
IRQ 2,  34 /* Connect with TRAP9 */
IRQ 3,  35 /* COM2 */
IRQ 4,  36 /* COM1 */
IRQ 5,  37 /* Audio */
IRQ 6,  38 /* Floppy Disk */
IRQ 7,  39 /* Printer */
IRQ 8,  40 /* Real-time Clock */
IRQ 9,  41 /* Connect with TRAP2*/
IRQ 10, 42 /* Network */
IRQ 11, 43 /* AGP */
IRQ 12, 44 /* PS/2 mouse */
IRQ 13, 45 /* FPU, Coprocessor, Inter-processor */
IRQ 14, 46 /* Primary ATA Hard Disk */
IRQ 15, 47 /* Secondary ATA Hard Disk */