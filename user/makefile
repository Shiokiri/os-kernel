RISC = riscv64-unknown-elf
CC = $(RISC)-gcc
AS = $(RISC)-as
LD = $(RISC)-ld
OBJCOPY = rust-objcopy
OBJDUMP = rust-objdump
SRC_LINKER = ./linker.ld
TARGET_ELF = ./00t.elf
TARGET_BIN = ./00t.bin
INC_DIR = ../include

APP_NUM = 00

SRC_FILES = ./entry.S \
			./main.c \
			./syscall.c \
			./${APP_NUM}t.c

build:
	${CC} ${SRC_FILES} -o ${TARGET_ELF} \
		-T${SRC_LINKER} \
		-I${INC_DIR} \
		-nostdinc -nostdlib -mcmodel=medany -fno-builtin
	${OBJCOPY} -S ${TARGET_ELF} -O binary ${TARGET_BIN}