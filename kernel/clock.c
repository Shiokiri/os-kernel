#include "clock.h"
#include "riscv.h"
#include "sbi.h"
#include "stdio.h"

// 10^6 / 10^7 s = 0.1 s
static uint64_t next_event = 1000000;

// counter for timer interrupt
volatile uint64_t ticks;

uint64_t get_time() {
    uint64_t time;
    __asm__ volatile("rdtime %0" : "=r"(time));
    return time;
}

void clock_set_next_event(void) {
    uint64_t time = get_time();
    uint64_t next = time + next_event;
    // cprintk(DEBUG, "current time is %d\n", time);
    // cprintk(DEBUG, "set next event to %d\n", next);
    sbi_set_timer(next);
}

void clock_init(void) {
    uint64_t sie = r_sie();
    // enable timer interrupt
    w_sie(sie | MIP_STIP);
    ticks = 0;
    clock_set_next_event();
    cprintk(INFO, "set timer interrupt\n");
}