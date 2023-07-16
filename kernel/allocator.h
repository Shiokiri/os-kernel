#include "types.h"
#include "addr.h"
#include "vector.h"

void frame_allocator_init(void);
PhysPageNum frame_allocator_alloc(void);
void frame_allocator_free(PhysPageNum *ppn);