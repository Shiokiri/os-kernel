#include "types.h"

typedef uint64_t PhysPageNum;
typedef uint64_t VirtPageNum;
typedef uint64_t PhysAddr;
typedef uint64_t VirtAddr;

#define ADDR_FLOOR  0  // 向下取整
#define ADDR_CEIL   1  // 向上取整

// va 与 pa 均含有保留位
PhysPageNum pa_to_ppn(PhysAddr addr, uint8_t flag);
PhysAddr ppn_to_pa(PhysPageNum page_num);
VirtPageNum va_to_vpn(VirtAddr addr, uint8_t flag);
VirtAddr vpn_to_va(VirtPageNum page_num);
void vpn_to_index(VirtPageNum vpn, uint64_t *index);