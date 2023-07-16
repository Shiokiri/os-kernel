RISC = riscv64-unknown-elf
CC = $(RISC)-gcc
AS = $(RISC)-as
LD = $(RISC)-ld
OBJCOPY = rust-objcopy
OBJDUMP = rust-objdump

kernel:
	$(CC) -g -Wall -Wextra -c -mcmodel=medany -ffreestanding *.c
	$(AS) -g -c entry.S -o entry.o
	$(AS) -g -c trapentry.S -o trapentry.o
	$(LD) -T kernel.ld -nostdlib *.o -o kernel.elf