#include "mem.h"
#include "addr.h"
#include "riscv.h"
#include "stdio.h"
#include "allocator.h"
#include "alloc.h"
#include "error.h"

extern void text_start();
extern void text_end();
extern void rodata_start();
extern void rodata_end();
extern void data_start();
extern void data_end();
extern void bss_start();
extern void bss_end();
extern void ekernel();
extern void strampoline();

static MemorySet kernel_ms = {0};

/*
 * 初始化MapArea，根据参数设置虚拟地址范围、映射类型和权限 
 */
void map_area_init(MapArea *ma, VirtAddr head, VirtAddr tail, MapType type, MapPermission permission) {
    cprintk(DEBUG, "init map area(0x%x) from 0x%x to 0x%x\n", ma, head, tail);
    ma->vpn_range.head = va_to_vpn(head, ADDR_FLOOR);
    ma->vpn_range.size = va_to_vpn(tail, ADDR_CEIL) - ma->vpn_range.head;
    ma->map_type = type;
    ma->map_permission = permission;
}

/*
 * 根据mapArea的属性建立地址映射
 */
void map_area_map(MapArea *ma, PageTable *table) {
    PhysPageNum ppn;
    for(int64_t i = 0; i < ma->vpn_range.size; i++) {
        if(ma->map_type == MAP_TYPE_IDENTICAL) {
            ppn = (PhysPageNum)(ma->vpn_range.head + i);
        } else {
            // TODO
        }
        // cprintk(DEBUG, "map vpn 0x%x to ppn 0x%x\n", ma->vpn_range.head + i, ppn);
        page_table_map(table, ma->vpn_range.head + i, ppn, ma->map_permission);
    }
}

void memory_set_push(MemorySet *ms, MapArea *data) {
    ms->areas.add(&ms->areas, data);
    map_area_map(data, ms->page_table);
}

// TODO
void memory_set_map_trampoline(MemorySet *ms) {
    MapArea ma_tmp;
    map_area_init(&ma_tmp, (VirtAddr)strampoline, (VirtAddr)strampoline + PAGE_SIZE, MAP_TYPE_IDENTICAL, MAP_PERMISSION_R | MAP_PERMISSION_X);
    memory_set_push(ms, &ma_tmp);
}

void memory_set_init(MemorySet *ms) {
    PageTable *table = (PageTable *)malloc(sizeof(PageTable));
    page_table_init(table);
    ms->page_table = table;
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    vector_init(vec);
    ms->areas = *vec;
}

void memory_set_kernel_init(MemorySet *ms) {
    MapArea *ma_tmp;
    // MapArea ma_tmp;
    memory_set_map_trampoline(ms); // 映射跳板
    cprintk(DEBUG, "text: 0x%x - 0x%x\n", (uint64_t)text_start, (uint64_t)text_end);
    cprintk(DEBUG, "rodata: 0x%x - 0x%x\n", (uint64_t)rodata_start, (uint64_t)rodata_end);
    cprintk(DEBUG, "data: 0x%x - 0x%x\n", (uint64_t)data_start, (uint64_t)data_end);
    cprintk(DEBUG, "bss: 0x%x - 0x%x\n", (uint64_t)bss_start, (uint64_t)bss_end);
    // memory_set_map_kernel_stack_kernel(ms); // 映射内核栈（用于trap时直接访问栈，而不用改变sp）
    ma_tmp = (MapArea *)malloc(sizeof(MapArea));
    map_area_init(ma_tmp, (VirtAddr)text_start,   (VirtAddr)text_end,   MAP_TYPE_IDENTICAL, MAP_PERMISSION_R | MAP_PERMISSION_X);
    memory_set_push(ms, ma_tmp);
    ma_tmp = (MapArea *)malloc(sizeof(MapArea));
    map_area_init(ma_tmp, (VirtAddr)rodata_start, (VirtAddr)rodata_end, MAP_TYPE_IDENTICAL, MAP_PERMISSION_R                   );
    memory_set_push(ms, ma_tmp);
    ma_tmp = (MapArea *)malloc(sizeof(MapArea));
    map_area_init(ma_tmp, (VirtAddr)data_start,   (VirtAddr)data_end,   MAP_TYPE_IDENTICAL, MAP_PERMISSION_R | MAP_PERMISSION_W);
    memory_set_push(ms, ma_tmp);
    ma_tmp = (MapArea *)malloc(sizeof(MapArea));
    map_area_init(ma_tmp, (VirtAddr)bss_start,    (VirtAddr)bss_end,    MAP_TYPE_IDENTICAL, MAP_PERMISSION_R | MAP_PERMISSION_W);
    memory_set_push(ms, ma_tmp);
    ma_tmp = (MapArea *)malloc(sizeof(MapArea));
    map_area_init(ma_tmp, (VirtAddr)ekernel,      (VirtAddr)MEMORY_END, MAP_TYPE_IDENTICAL, MAP_PERMISSION_R | MAP_PERMISSION_W);
    memory_set_push(ms, ma_tmp);
    cprintk(DEBUG, "memory set kernel init done\n");
}

/*
 * init frame allocator
 * init kernel memory set
 * set satp
 * set mmu 
 */
void memory_init(void) {
    frame_allocator_init();
    // kernel_mm_set_for_mmu_init();
    memory_set_init(&kernel_ms);
    memory_set_kernel_init(&kernel_ms);
    cprintk(DEBUG, "kernel page table root: 0x%x\n", kernel_ms.page_table->root);
    cprintk(DEBUG, "satp: 0x%x\n", ((uint64_t)0x8 << 60) | (uint64_t)kernel_ms.page_table->root);
    w_satp( ((uint64_t)0x8 << 60) | (uint64_t)kernel_ms.page_table->root);
    asm volatile("sfence.vma");
    cprintk(DEBUG, "memory init done\n");
}
