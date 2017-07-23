# current using architecture i386
ARCH=i386
CC=gcc
CFLAGS= -g -static -fno-builtin -fno-strict-aliasing -O2 -march=$(ARCH) -m32 -ffreestanding -Wall -Werror
LFLAGS= -melf_i386 -Tkernel.ld -nostdlib --nmagic 
KERDIR=./kernel/
KERINC=./kernel/

OBJS=\
$(KERDIR)kernel.o \
$(KERDIR)vga.o\
$(KERDIR)boot.o

HEADERS=\
$(KERDIR)kernel.h\
$(KERDIR)asm.h


boot.elf: $(OBJS)
	ld $(LFLAGS) -o $@ $^

$(KERDIR)boot.o: $(KERDIR)boot.S $(HEADERS)
	$(CC) $(CFLAGS) -nostdinc -I$(KERINC) -c $< -o $@

$(KERDIR)%.o: $(KERDIR)%.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(KERINC) -c $< -o $@

# run on qemu
qemu:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf
gdb:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf -gdb tcp::1234 -S
clean:
	rm $(KERDIR)*.o *.elf
