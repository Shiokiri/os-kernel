#include "types.h"
#include "intr.h"
#include "riscv.h"

void intr_enable(void) {
    w_sstatus(r_sstatus() | SSTATUS_SIE);
}

void intr_disable(void) {
    w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}