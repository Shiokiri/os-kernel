#include "types.h"

void sbi_set_timer(uint64_t stime_value);
void sbi_console_putchar(uint8_t ch);
uint8_t sbi_console_getchar(void);
void sbi_shutdown(void);
void judge_error(int error_code);
uint64_t sbi_get_spec_version(void);