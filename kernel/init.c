#include "trap.h"
#include "intr.h"
#include "string.h"
#include "stdio.h"
#include "clock.h"
#include "init.h"
#include "alloc.h"
#include "mem.h"

/*
 * set the trap handler
 * enable the timer interrupt(set sie)
 * enable s mode interrupt(set sstatus)
 * init the kernel heap
 * init the memory
*/
int kern_init(void) {
    // extern char edata[], end[];
    // memset(edata, 0, end - edata);
    trap_init();
    clock_init();
    intr_enable();
    kernel_heap_init();
    memory_init();
    cprintk(INFO, "Kernel initialized.\n");
    return 0;
}