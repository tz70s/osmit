/* assembly for TRAP_ENTRY and IRQ settings */
.macro TRAP_ENTRY TRAP_ENTRY_NUM, ERRCODE
.global trap\TRAP_ENTRY_NUM
.type trap\TRAP_ENTRY_NUM, @function

trap\TRAP_ENTRY_NUM:
    cli
.if \ERRCODE
.else
    push $0
.endif
    push \TRAP_ENTRY_NUM
    jmp isr_common_stub
.endm

TRAP_ENTRY 0,  0 /* #DE */
TRAP_ENTRY 1,  0 /* #DB */
TRAP_ENTRY 2,  0 /* #NMI */
TRAP_ENTRY 3,  0 /* #BP */
TRAP_ENTRY 4,  0 /* #OF */
TRAP_ENTRY 5,  0 /* #BR */
TRAP_ENTRY 6,  0 /* #UD */
TRAP_ENTRY 7,  0 /* #NM */
TRAP_ENTRY 8,  1 /* #DF */
TRAP_ENTRY 9,  0 /* Cross operation*/
TRAP_ENTRY 10, 1 /* #TS */
TRAP_ENTRY 11, 1 /* #NP */
TRAP_ENTRY 12, 1 /* #SS */
TRAP_ENTRY 13, 1 /* #GP */
TRAP_ENTRY 14, 1 /* #PF */
TRAP_ENTRY 15, 0 /* CPU preserved */
TRAP_ENTRY 16, 0 /* #MF */
TRAP_ENTRY 17, 1 /* #AC */
TRAP_ENTRY 18, 0 /* #MC */
TRAP_ENTRY 19, 0 /* #XM */
TRAP_ENTRY 20, 0 /* #VE */

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