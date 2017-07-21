__asm__ (".code16\n");
__asm__ ("jmpl $0x0000, $main\n");


void puts(char *str) {
	while(*str) {
		__asm__ __volatile__ (
			"int $0x10\n" : : "a"(0x0e00 | *str), "b"(0x0007)
		);
		++str;
	}
}

void main() {
	puts("hello world");
	__asm__ __volatile__ (
		"movb $'H', %al\n"
		"movb $0x0e, %ah\n"
		"int $0x10\n"

		"movb $'i', %al\n"
		"movb $0x0e, %ah\n"
		"int $0x10\n"

		"movb $'!', %al\n"
		"movb $0x0e, %ah\n"
		"int $0x10\n"
	);
}
