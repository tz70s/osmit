ARCH=i386
CC=gcc
CFLAGS= -g -Os -march=$(ARCH) -m32 -ffreestanding -Wall -Werror

boot.o: boot.S
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: as
as:
	ld -Ttext 0x7c00 -melf_i386 -Tsetup.ld --oformat=binary boot.o -o boot.bin
target:
	ld -static -melf_i386 -Tsetup.ld -nostdlib --nmagic -o boot.elf boot.o
	objcopy -O binary boot.elf boot.bin

clean:
	rm *.o *.elf *.bin
