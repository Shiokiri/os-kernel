#include "riscv.h"
#include "trap.h"
#include "stdio.h"
#include "types.h"
#include "clock.h"

extern void __alltraps(void);

// every 100 clock ticks (approximately 1 second), print a string
#define TICK_NUM 100

void interrupt_handler(trapframe_t *tf) {
    // clear the pending bit
    uint64_t cause = (tf->cause << 1) >> 1;
    switch (cause) {
    case IRQ_S_TIMER:
        clock_set_next_event();
        if(++ticks % TICK_NUM == 0) {
            printk("100 ticks\n");
        }
        break;
    default:
        printk("unknown interrupt\n");
        break;
    } 
}

void exception_handler(trapframe_t *tf) {
    printk("exception_handler\n");
}

void trap_init(void) {
    cprintk(DEBUG, "set stvec to __alltraps in trapentry.S at 0x%x\n", __alltraps);
    w_stvec((uint64_t)__alltraps);
}

void trap(trapframe_t *tf) {
    if((int64_t)(tf->cause) < 0) {
        interrupt_handler(tf);
    } else {
        printk("trap: exception\n");
        exception_handler(tf);
    }
}