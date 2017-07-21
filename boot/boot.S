# Bootloader to load os kernel
# BIOS load this from the first sector of the hard disk into physical memory address 0x7c00
# %cs=0 %ip=7c00

# real mode
.code16
.globl _start
_start:

    # first, say hi to all
    movb $'H', %al
	movb $0x0e, %ah
	int $0x10
	
	movb $'i', %al
	movb $0x0e, %ah
	int $0x10

	movb $'!', %al
	movb $0x0e, %ah
	int $0x10

    cli     # disable interrupts

    # clean data segment registers DS, ES, and SS.
    xorw %ax, %ax   # Set %ax to zero
    movw %ax, %ds   # -> Data Segment
    movw %ax, %es   # -> Extra Segment
    movw %ax, %ss   # -> Stack Segment

    # start transferring to protection mode
    # first set A20 gate for set up 21bit addressable

    # first checkout the keyboard I/O buffer is getting zero
    # otherwise, wait for the buffer clean 
seta20.1:
    inb $0x64, %al      # read I/O buffer data
    testb $0x2, %al     # checkout clean up
    jnz seta20.1        # if not, keep waiting

    movb $0xd1, %al      
    outb %al, $0x64     # 0xd1 -> port 0x64

seta20.2:
    inb $0x64, %al      # read I/O buffer data
    testb $0x2, %al     # checkout clean up
    jnz seta20.2        # if not, keep waiting

    movb $0xdf, %al     
    outb %al, $0x60     # 0xdf -> port 0x60
