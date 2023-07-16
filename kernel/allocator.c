#include "allocator.h"
#include "riscv.h"
#include "error.h"
#include "stdio.h"
#include "string.h"
#include "alloc.h"

/*
 * head: the first free page
 * tail: the last free page
 * free_pages: 已经分配的且已回收的页
 */
static struct {
    PhysPageNum head;
    PhysPageNum tail;
    // vector 保存 ppn
    Vector free_pages;
} FrameAllocator;

#define __vec FrameAllocator.free_pages

extern void ekernel();

void frame_allocator_init(void) {
    cprintk(DEBUG, "ekernel is 0x%x\n", ekernel);
    FrameAllocator.head = pa_to_ppn((PhysAddr)ekernel, ADDR_CEIL);
    FrameAllocator.tail = pa_to_ppn((PhysAddr)MEMORY_END, ADDR_FLOOR);
    cprintk(DEBUG, "FrameAllocator init: head = 0x%x, tail = 0x%x\n", FrameAllocator.head, FrameAllocator.tail);
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    vector_init(vec);
    FrameAllocator.free_pages = *vec;
}

PhysPageNum frame_allocator_alloc(void) {
    PhysPageNum result;
    // cprintk(DEBUG, "vector is 0x%x\n", &__vec);
    // cprintk(DEBUG, "vec.size addr is 0x%x\n", &__vec.size);
    // 如果free_pages为空，则分配head指向的页
    if(__vec.size(&__vec) == 0) {
        result = FrameAllocator.head++;
    } else {
        result = (PhysPageNum)__vec.pop(&__vec);
    }
    uint8_t *ptr = (uint8_t *)ppn_to_pa(result);
    // 将分配的页清零
    // for(uint64_t i = 0; i < PAGE_SIZE; i++) {
    //     cprintk(DEBUG, "clear 0x%x\n", ptr + i);
    //     ptr[i] = 0;
    // }
    memset(ptr, 0, PAGE_SIZE);
    return result;
}

void frame_allocator_free(PhysPageNum *ppn) {
    // judge if ppn is valid, if not , panic
    if(*ppn >= FrameAllocator.head || __vec.find(&__vec, ppn) != NULL) {
        panic("free invalid ppn");
    }
    __vec.add(&__vec, ppn);
}