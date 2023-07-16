#include "addr.h"

#define PAGE_SHIFT 12

PhysPageNum pa_to_ppn(PhysAddr addr, uint8_t flag) {
    return (flag == ADDR_FLOOR) ? addr >> PAGE_SHIFT : (addr >> PAGE_SHIFT) + 1;
}

PhysAddr ppn_to_pa(PhysPageNum page_num) {
    return page_num << PAGE_SHIFT;
}

VirtPageNum va_to_vpn(VirtAddr addr, uint8_t flag) {
    return (flag == ADDR_FLOOR) ? addr >> PAGE_SHIFT : (addr >> PAGE_SHIFT) + 1;
}

VirtAddr vpn_to_va(VirtPageNum page_num) {
    return page_num << PAGE_SHIFT;
}

void vpn_to_index(VirtPageNum vpn, uint64_t *index) {
    for(int i = 2; i >= 0; i --){
        index[i] = vpn & 0x1ff;
        vpn >>= 9;
    }
}
