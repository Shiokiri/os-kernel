#include "error.h"

void panic(const char* s) {
    printk(ERROR, "Kernel panic: %s\n", s);
    sbi_shutdown();
}