#include "types.h"
#include "pte.h"
#include "vector.h"

// 恒等映射
#define MAP_TYPE_IDENTICAL 0
#define MAP_TYPE_RESERVED 1

#define MAP_PERMISSION_R 1 << 1
#define MAP_PERMISSION_W 1 << 2
#define MAP_PERMISSION_X 1 << 3
#define MAP_PERMISSION_U 1 << 4

typedef uint64_t MapType;
typedef uint64_t MapPermission;

typedef struct {
    VirtPageNum head;
    uint64_t size;
} VpnRange;

/*
 * 每个MapArea存储一个逻辑段，具有相同的权限
 * vpn_range: 逻辑段的虚拟地址范围
 * map_type: 映射类型(MAP_TYPE_IDENTICAL, MAP_TYPE_RESERVED)
 * map_permission: 映射权限(MAP_PERMISSION_R, MAP_PERMISSION_W, MAP_PERMISSION_X, MAP_PERMISSION_U)
 */
typedef struct MapArea {
    VpnRange vpn_range;
    MapType map_type;
    MapPermission map_permission;
} MapArea;

void map_area_init(MapArea *ma, VirtAddr head, VirtAddr tail, MapType type, MapPermission permission);
void map_area_map(MapArea *ma, PageTable *table);

/*
 * 逻辑段的集合和页表
 */
typedef struct MemorySet {
    PageTable *page_table;
    Vector areas; // of MapArea
} MemorySet;

void memory_init(void);