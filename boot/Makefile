# current using architecture i386
ARCH=i386
CC=gcc
CFLAGS= -g -Os -march=$(ARCH) -m32 -ffreestanding -Wall -Werror

boot.bin: boot.o kernel.o
	ld -melf_i386 -Tkernel.ld -nostdlib --nmagic -o $@ $^
boot.o: boot.s
	$(CC) -c $(CFLAGS) $< -o $@
kernel.o: kernel.c
	$(CC) -c $(CFLAGS) $< -o $@

# run on qemu
qemu:
	@echo "start booting kernel from qemu..."
	@qemu-system-i386 -kernel boot.bin
clean:
	rm *.o *.bin
