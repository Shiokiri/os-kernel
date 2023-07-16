#include "types.h"

extern volatile uint64_t ticks;

uint64_t get_time(void);
void clock_init(void);
void clock_set_next_event(void);