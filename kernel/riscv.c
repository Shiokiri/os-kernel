#include "riscv.h"

uint64_t set_bit(uint64_t target, int index, int len, uint64_t insert) {
    uint64_t mask = 0xffffffffffffffff;
    mask >>= (32 - len);
    mask <<= (index - len + 1);
    mask = ~mask;
    target &= mask;
    insert <<= (index - len + 1);
    target |= insert;
    return target;
}

/*
 * stvec is the address of the trap handler 
*/
void w_stvec(uint64_t x) {
    __asm__ volatile ("csrw stvec, %0" : : "r" (x));
}

uint64_t r_sstatus(void) {
    uint64_t x;
    __asm__ volatile ("csrr %0, sstatus" : "=r" (x));
    return x;
}

void w_sstatus(uint64_t x) {
    __asm__ volatile ("csrw sstatus, %0" : : "r" (x));
}

uint64_t r_scause(void) {
    uint64_t x;
    __asm__ volatile ("csrr %0, scause" : "=r" (x));
    return x;
}

uint64_t r_stval(void) {
    uint64_t x;
    __asm__ volatile ("csrr %0, stval" : "=r" (x));
    return x;
}

/*
 * sie contains the bits to enable/disable time interrupts 
*/
uint64_t r_sie(void) {
    uint64_t x;
    __asm__ volatile ("csrr %0, sie" : "=r" (x));
    return x;
}

void w_sie(uint64_t x) {
    __asm__ volatile ("csrw sie, %0" : : "r" (x));
}

/* write mepc, which is the address of the instruction after mret
 * when exception happens, mepc will be set to the address of the instruction that caused the exception
 * and mepc can be a virtual address, so sometime we should disable paging first to use the physical address
 */
void w_mepc(uint64_t addr) {
    __asm__ volatile ("csrw mepc, %0" : : "r" (addr));
}

/*
 * satp controls supervisor-mode address translation and protection
 */
void w_satp(uint64_t x) {
    __asm__ volatile ("csrw satp, %0" : : "r" (x));
    __asm__ volatile ("sfence.vma");
}