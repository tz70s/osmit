# current using architecture i386
ARCH=i386
CC=gcc
CFLAGS= -g -static -fno-builtin -fno-strict-aliasing -O2 -march=$(ARCH) -m32 -ffreestanding -Wall -Werror
BOOTDIR=./boot/
BOOTIC=./boot/
OBJECTS= boot.o kernel.o vga.o util.o


boot.elf: $(BOOTDIR)boot.o $(BOOTDIR)kernel.o $(BOOTDIR)vga.o $(BOOTDIR)util.o
	ld -melf_i386 -Tkernel.ld -nostdlib --nmagic -o $@ $^

$(BOOTDIR)boot.o: $(BOOTDIR)boot.S
	$(CC) $(CFLAGS) -nostdinc -I$(BOOTIC) -c $< -o $@

$(BOOTDIR)util.o: $(BOOTDIR)util.c $(BOOTDIR)type.h
	$(CC) -c $(CFLAGS) -I$(BOOTIC) $< -o $@

$(BOOTDIR)vga.o: $(BOOTDIR)vga.c $(BOOTDIR)util.h $(BOOTDIR)type.h
	$(CC) -c $(CFLAGS) -I$(BOOTIC) $< -o $@

$(BOOTDIR)kernel.o: $(BOOTDIR)kernel.c $(BOOTDIR)vga.h
	$(CC) -c $(CFLAGS) -I$(BOOTIC) $< -o $@

# run on qemu
qemu:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf
gdb:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf -gdb tcp::1234 -S
clean:
	rm $(BOOTDIR)*.o *.elf
