# current using architecture i386
ARCH=i386
CC=gcc
CFLAGS= -g -static -fno-builtin -fno-strict-aliasing -O2 -march=$(ARCH) -m32 -ffreestanding -Wall -Werror
LFLAGS= -melf_i386 -Tkernel.ld -nostdlib --nmagic 
KERDIR=./kernel/
KERINC=./kernel/

OBJS=\
kernel.o \
vga.o\
boot.o\
gdt.o
OBJS:=$(addprefix $(KERDIR), $(OBJS))

HEADERS=\
kernel.h

HEADERS:=$(addprefix $(KERDIR), $(HEADERS))

boot.elf: $(OBJS)
	ld $(LFLAGS) -o $@ $^

$(KERDIR)boot.o: $(KERDIR)boot.S $(HEADERS)
	$(CC) $(CFLAGS) -nostdinc -I$(KERINC) -c $< -o $@

$(KERDIR)%.o: $(KERDIR)%.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(KERINC) -c $< -o $@

test:
	echo $(OBJS)
# run on qemu
qemu:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf
gdb:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf -gdb tcp::1234 -S
clean:
	rm $(KERDIR)*.o *.elf
