#include "syscall.c"

int lib_main() {
    char* temp = "hello, user mode world!\n";
    cputs(temp);
    exit(0);
}