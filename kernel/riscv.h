#include "types.h"

#define HEAP_SIZE 0x100000
#define MEM_FREE 0
#define MEM_USED 1

#define PAGE_SIZE 4096
#define MEMORY_END 0x80800000

#define NCPU 1
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define STATUS_MACHINE_MODE 0b11
#define STATUS_SUPERVISOR_MODE 0b01
#define STATUS_USER_MODE 0b00
#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define MIP_STIP (1L << 5)     // Supervisor Timer Interrupt Pending

#define IRQ_U_SOFT   0
#define IRQ_S_SOFT   1
#define IRQ_H_SOFT   2
#define IRQ_M_SOFT   3
#define IRQ_U_TIMER  4
#define IRQ_S_TIMER  5
#define IRQ_H_TIMER  6
#define IRQ_M_TIMER  7
#define IRQ_U_EXT    8
#define IRQ_S_EXT    9
#define IRQ_H_EXT    10
#define IRQ_M_EXT    11
#define IRQ_COP      12
#define IRQ_HOST     13

uint64_t set_bit(uint64_t target, int index, int len, uint64_t insert);
void w_stvec(uint64_t x);
uint64_t r_sstatus(void);
void w_sstatus(uint64_t x);
uint64_t r_scause(void);
uint64_t r_stval(void);
uint64_t r_sie(void);
void w_sie(uint64_t x);
void w_mepc(uint64_t addr);
void w_satp(uint64_t x);