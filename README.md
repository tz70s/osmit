## OSmit

Implement a simple operating system.

### TODO
- [ ] Initialized Kernel
	- [ ] Set up GDT
	- [ ] Set up IDT
- [ ] Memory Management
	- [ ] Paging
- [ ] Process Management
- [ ] Interrupt, I/O Management 

### Execution
```bash
$ cd boot
$ make
# run on qemu-system-i386
$ make qemu
```

### Compilation
Not so serious to build own cross-compilation toolchain(binutils, gcc), use flags in gcc, ld instead.

### License
MIT

### Author
[@tz70s](https://twitter.com/tz70s)
