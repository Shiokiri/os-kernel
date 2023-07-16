#include <stdarg.h>

#define ERROR 0
#define WARN 1
#define DEBUG 2
#define INFO 3

int printk(const char *fmt, ...);
int cprintk(const int level, const char *fmt, ...);