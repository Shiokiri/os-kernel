#include "types.h"
#include "riscv.h"
#include "stdio.h"
#include "error.h"
#include "init.h"
#include "alloc.h"
#include "vector.h"
#include "string.h"

void main();

extern char ekernel[];

// os should be in supervisor mode
void main_start(void) {
	kern_init();
	char *str = (char *)malloc(30);
	strcpy(str, "hello world\n");
	printk("%s", str);
	cprintk(INFO, "str address is 0x%x\n", str);
	free(str);
    // Vector *Vec = (Vector *)malloc(sizeof(Vector));
    // vector_init(Vec);
	// Vector vec = *Vec;
	// vec.add(&vec, "hello");
	// vec.add(&vec, "world");
	// vec.add(&vec, "!");
	// vec.add(&vec, "!");
	// printk("%s\n", vec.get(&vec, 0));
	// printk("size of vec is %d\n", vec.size(&vec));
	// vec.delete(&vec, 0);
	// printk("%s\n", vec.get(&vec, 0));
	// printk("size of vec is %d\n", vec.size(&vec));
	// vec.set(&vec, 0, "hello");
	// printk("%s\n", vec.get(&vec, 0));
	// vec.free(&vec);
	// printk("vec is 0x%x\n", &vec);
	// cprintk(INFO, "end address is 0x%x\n", ekernel);
	while(1);
}