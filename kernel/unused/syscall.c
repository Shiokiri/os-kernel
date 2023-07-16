#include "types.h"
#include "stdio.h"

#define SYSCALL_WRITE      64
#define SYSCALL_EXIT       93
#define SYSCALL_YIELD     124
#define SYSCALL_GET_TIME  169

uint64_t syscall(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    uint64_t ret;
    switch (id) {
        case SYSCALL_WRITE:
            ret = syswrite(arg0, (char *)arg1, arg2);
            break;
        case SYSCALL_EXIT:
            sysexit(arg0);
            break;
        default:
            printk("[syscall] error id = %x\n", id);
            panic("kernel: syscall id undefined.");
            break;
    }
    return ret;
}


uint64_t syswrite(uint64_t fd, char* buf, uint64_t len) {
    uint64_t ret;
    switch (fd)
    {
    case 1:
        cputs(buf);
        break;
    
    default:
        panic("kernel: sys_write fd undefined.");
        break;
    }
    return len;
}

void sysexit(uint64_t code) {
    panic("kernel: user app exit");
}