## OSmit

Implement a simple operating system.

### TODO
- [ ] Initialized Kernel
	- [x] Set up GDT
	- [ ] Set up IDT, Interrupt
- [ ] Memory Management
	- [ ] Paging
- [ ] Process Management

### Execution
```bash
$ cd boot
$ make
# run on qemu-system-i386
$ make qemu
```

### Debugging
```bash
# set remote gdb server on localhost:1234
$ make gdb

$ gdb

$(gdb) file boot.elf
$(gdb) target remote :1234

# looks up help
$(gdb) help

# example : set break point
$(gdb) b * _start 

# example : next instruction
$(gdb) si

# example : continue, remote gdb can't use run, use continue instead
$(gdb) continue

```

### Compilation
Not so serious to build own cross-compilation toolchain(binutils, gcc), use flags in gcc, ld instead.

I.e. `-march=i386, -m32`

### License
MIT

### Author
[@tz70s](https://twitter.com/tz70s)
