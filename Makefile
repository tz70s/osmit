# current using architecture i386
ARCH=i386
CC=gcc
CFLAGS= -g -static -fno-builtin -fno-strict-aliasing -O2 -march=$(ARCH) -m32 -ffreestanding -Wall -Werror
BOOTDIR=./boot/
BOOTIC=./boot/

boot.elf: $(BOOTDIR)boot.o $(BOOTDIR)kernel.o
	ld -melf_i386 -Tkernel.ld -nostdlib --nmagic -o $@ $^
$(BOOTDIR)boot.o: $(BOOTDIR)boot.S
	$(CC) $(CFLAGS) -nostdinc -I$(BOOTIC) -c $< -o $@
$(BOOTDIR)kernel.o: $(BOOTDIR)kernel.c
	$(CC) -c $(CFLAGS) $< -o $@
# run on qemu
qemu:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf
gdb:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.elf -gdb tcp::1234 -S
clean:
	rm $(BOOTDIR)*.o *.elf
