#include "riscv.h"
#include "types.h"

typedef struct memblock {
    struct memblock *prev;
    struct memblock *next;
    uint64_t size; // the real free size
    uint8_t isFree;
} memblock;

void kernel_heap_init(void);
void *malloc(uint64_t size);
void free(void *addr);
void *realloc(void *addr, uint64_t size);