#include "alloc.h"
#include "stdio.h"
#include "string.h"

static uint8_t kernel_heap[HEAP_SIZE] = {0};

void kernel_heap_init(void) {
    memblock *block = (memblock *)kernel_heap;
    block->isFree = MEM_FREE;
    block->next = NULL;
    block->prev = NULL;
    block->size = HEAP_SIZE - sizeof(memblock);
    cprintk(DEBUG, "kernel heap init, kernel heap is 0x%x\n", kernel_heap);
    cprintk(DEBUG, "kernel heap init, kernel heap end is 0x%x\n", kernel_heap + HEAP_SIZE);
}

// 使用first fit算法分配内存
void *malloc(uint64_t size) {
    memblock *block = (memblock *)kernel_heap;
    memblock *target = NULL;
    void *return_addr = NULL;
    while(block != NULL) {
        if(block->isFree == MEM_FREE && block->size >= size) {
            target = block;
            break;
        }
        block = block->next;
    }
    if(target == NULL) {
        cprintk(ERROR, "malloc failed, no enough memory\n");
        return NULL;
    }
    target->isFree = MEM_USED;
    return_addr = (void *)((uint64_t)target + sizeof(memblock));
    // if the size of the left memory is larger than the size of memblock, split it
    // else malloc all the left memory
    if(target->size - size > sizeof(memblock)) {
        memblock *new_block = (memblock *)((uint64_t)return_addr + size);
        new_block->isFree = MEM_FREE;
        new_block->size = target->size - size - sizeof(memblock);
        new_block->prev = target;
        new_block->next = target->next;
        if(target->next != NULL) {
            target->next->prev = new_block;
        }
        target->next = new_block;
        target->size = size;
    } else {
        ;
    }
    return return_addr;
}

// merge the free block with its prev block recrusily, return the new block address
void *merge_prev(memblock *block) {
    memblock *result_block = block->prev;
    if(result_block == NULL || result_block->isFree == MEM_USED) {
        return block;
    }
    result_block->size += block->size + sizeof(memblock);
    result_block->next = block->next;
    if(block->next != NULL) {
        block->next->prev = result_block;
    }
    return merge_prev(result_block);
}

void free(void* free_addr) {
    if(free_addr == NULL) {
        cprintk(ERROR, "free failed, target address is NULL\n");
        return;
    }
    memblock *block = (memblock *)((uint64_t)free_addr - sizeof(memblock));
    block->isFree = MEM_FREE;
    merge_prev(block);
}

void *realloc(void *addr, uint64_t size) {
    if(addr == NULL) {
        return malloc(size);
    }
    memblock *block = (memblock *)((uint64_t)addr - sizeof(memblock));
    if(block->size >= size) {
        return addr;
    }
    void *new_addr = malloc(size);
    if(new_addr == NULL) {
        cprintk(ERROR, "realloc failed, no enough memory\n");
        return NULL;
    }
    memcpy(new_addr, addr, block->size);
    free(addr);
    return new_addr;
}