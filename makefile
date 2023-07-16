DIR = ./kernel
RISC = riscv64-unknown-elf
CC = $(RISC)-gcc
AS = $(RISC)-as
LD = $(RISC)-ld

QEMU = qemu-system-riscv64
QEMUOPTS = -machine virt -bios ./rustsbi-qemu.bin -m 128M -nographic -kernel

GDB_PORT = 1234

.PHONY: all build kernel gdb clean libs

all: build

build: kernel $(DIR)/kernel.elf
	$(QEMU) $(QEMUOPTS) $(DIR)/kernel.elf
	make clean

# entry.S start.c kernel.ld
kernel: 
	cd ./kernel && make
	cd ../

libs:
	cd ./include && make
	cd ../

debug: kernel
	$(QEMU) -s -S $(QEMUOPTS) $(DIR)/kernel.elf
	make clean
# riscv64-unknown-elf-gdb -ex "target remote localhost:$(GDB_PORT)" $(DIR)/kernel.elf

clean:	
	rm -f $(DIR)/*.o $(DIR)/kernel.elf

# gdb: target remote :1234
# b start