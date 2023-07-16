#include "pte.h"
#include "allocator.h"
#include "alloc.h"

#define PTE_V (1 << 0)
#define PTE_R (1 << 1)
#define PTE_W (1 << 2)
#define PTE_X (1 << 3)
#define PTE_U (1 << 4)
#define PTE_A (1 << 6)
#define PTE_D (1 << 7)

#define BIT_COUNT 10

// 0000 0000 0011 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000
#define PTE_PPN_MASK 0x3FFFFFFFFFFC00
#define PTE_FLAGS_MASK 0x3FF

int pte_isValid(PageTableEntry pte) {
    return (pte & PTE_V) == PTE_V ? PTE_PASS : PTE_FAIL;
}

int pte_isReadable(PageTableEntry pte) {
    return (pte & PTE_R) == PTE_R ? PTE_PASS : PTE_FAIL;
}

int pte_isWritable(PageTableEntry pte) {
    return (pte & PTE_W) == PTE_W ? PTE_PASS : PTE_FAIL;
}

int pte_isExecutable(PageTableEntry pte) {
    return (pte & PTE_X) == PTE_X ? PTE_PASS : PTE_FAIL;
}

int pte_isUserValid(PageTableEntry pte) {
    return (pte & PTE_U) == PTE_U ? PTE_PASS : PTE_FAIL;
}

int pte_isAccessed(PageTableEntry pte) {
    return (pte & PTE_A) == PTE_A ? PTE_PASS : PTE_FAIL;    
}

int pte_isDirty(PageTableEntry pte) {
    return (pte & PTE_D) == PTE_D ? PTE_PASS : PTE_FAIL;
}

PhysPageNum pte_getPPN(PageTableEntry pte) {
    return (PhysPageNum)((pte & PTE_PPN_MASK) >> BIT_COUNT);
}

PTEFlags pte_getFlags(PageTableEntry pte) {
    return (PTEFlags)(pte & PTE_FLAGS_MASK);
}

PageTableEntry pte_new(PhysPageNum ppn, PTEFlags flags) {
    return (PageTableEntry)((ppn << BIT_COUNT) | flags);
}

void page_table_init(PageTable *table) {
    PhysPageNum ppn = frame_allocator_alloc();
    table->root = ppn;
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    vector_init(vec);
    table->ppns = *vec;
    table->ppns.add(&table->ppns, &ppn);
}

PageTableEntry *page_table_find_pte(PageTable *table, VirtPageNum vpn) {
    uint64_t index[3];
    vpn_to_index(vpn, index);
    PageTableEntry *root = (PageTableEntry *)ppn_to_pa(table->root);
    PageTableEntry *pte = root + index[0];
    for(int i = 0; i < 2; i++) {
        if(!pte_isValid(*pte)) {
            PhysPageNum ppn = frame_allocator_alloc();
            *pte = pte_new(ppn, (PTEFlags)PTE_V);
            table->ppns.add(&table->ppns, &ppn); // TODO
        }
        pte = (PageTableEntry *)ppn_to_pa(pte_getPPN(*pte)) + index[i + 1];
    }
    return pte;
}

void page_table_map(PageTable *table, VirtPageNum vpn, PhysPageNum ppn, PTEFlags flags) {
    PageTableEntry *pte = page_table_find_pte(table, vpn);
    *pte = pte_new(ppn, flags | PTE_V);
}

void page_table_unmap(PageTable *table, VirtPageNum vpn) {
    PageTableEntry *pte = page_table_find_pte(table, vpn);
    *pte = pte_new(0, 0);
}
