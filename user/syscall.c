#include "types.h"
#include "string.h"

#define SYSCALL_WRITE      64
#define SYSCALL_EXIT       93
#define SYSCALL_YIELD     124
#define SYSCALL_GET_TIME  169

/*
 * a7: sbi_type
 * a6: function number if applicable
 * a0 - a5: arguments
 * after ecall, a0: error, a1: return value
*/
uint64_t system_call(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    uint64_t result;
    register uint64_t a7 asm("a7") = id;
    register uint64_t a0 asm("a0") = arg0;
    register uint64_t a1 asm("a1") = arg1;
    register uint64_t a2 asm("a2") = arg2;
    asm volatile ("ecall"
        : "=r"(a0)
        : "r"(a1), "r"(a2), "r"(a7)
        : "memory");
    result = a0;
    return result;
}

uint64_t sys_write(uint64_t fd, char* buf) {
    return system_call(SYSCALL_WRITE, fd, buf, strlen(buf));
}

uint64_t sys_exit(uint64_t code) {
    return system_call(SYSCALL_EXIT, code, 0, 0);
}

uint64_t write(uint64_t fd, char* buf) {
    return sys_write(fd, buf);
}

uint64_t exit(uint64_t code) {
    return sys_exit(code);
}

#define STDOUT 1

void cputs(char* str) {
    write(STDOUT, str);
}