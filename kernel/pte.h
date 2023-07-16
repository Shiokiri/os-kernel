#include "types.h"
#include "addr.h"
#include "vector.h"

#define PTE_PASS 1
#define PTE_FAIL 0

typedef uint16_t PTEFlags;

typedef struct PageTable {
    PhysPageNum root;
    // vector of PhysPageNum
    Vector ppns;
} PageTable;

typedef uint64_t PageTableEntry;

int pte_isValid(PageTableEntry pte);
int pte_isReadable(PageTableEntry pte);
int pte_isWritable(PageTableEntry pte);
int pte_isExecutable(PageTableEntry pte);
int pte_isUserValid(PageTableEntry pte);
int pte_isAccessed(PageTableEntry pte);
int pte_isDirty(PageTableEntry pte);
PhysPageNum pte_getPPN(PageTableEntry pte);
PTEFlags pte_getFlags(PageTableEntry pte);

void page_table_init(PageTable *table);
void page_table_map(PageTable *table, VirtPageNum vpn, PhysPageNum ppn, PTEFlags flags);
void page_table_unmap(PageTable *table, VirtPageNum vpn);