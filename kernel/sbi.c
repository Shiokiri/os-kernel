#include "types.h"
#include "stdio.h"
#include "error.h"

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8

#define SRST_EXTENSION 0x53525354
#define TIMER_EXTENSION 0x54494D45

#define SBI_SUCCESS 0
#define SBI_ERR_FAILED -1
#define SBI_ERR_NOT_SUPPORTED -2
#define SBI_ERR_INVALID_PARAM -3
#define SBI_ERR_DENIED -4
#define SBI_ERR_INVALID_ADDRESS -5
#define SBI_ERR_ALREADY_AVAILABLE -6
#define SBI_ERR_ALREADY_STARTED -7
#define SBI_ERR_ALREADY_STOPPED -8

void judge_error(int error_code) {
    if (error_code < 0) {
        switch (error_code) {
            case SBI_ERR_FAILED:
                cprintk(WARN, "SBI_ERR_FAILED\n");
                break;
            case SBI_ERR_NOT_SUPPORTED:
                cprintk(WARN, "SBI_ERR_NOT_SUPPORTED\n");
                break;
            case SBI_ERR_INVALID_PARAM:
                cprintk(WARN, "SBI_ERR_INVALID_PARAM\n");
                break;
            case SBI_ERR_DENIED:
                cprintk(WARN, "SBI_ERR_DENIED\n");
                break;
            case SBI_ERR_INVALID_ADDRESS:
                cprintk(WARN, "SBI_ERR_INVALID_ADDRESS\n");
                break;
            case SBI_ERR_ALREADY_AVAILABLE:
                cprintk(WARN, "SBI_ERR_ALREADY_AVAILABLE\n");
                break;
            case SBI_ERR_ALREADY_STARTED:
                cprintk(WARN, "SBI_ERR_ALREADY_STARTED\n");
                break;
            case SBI_ERR_ALREADY_STOPPED:
                cprintk(WARN, "SBI_ERR_ALREADY_STOPPED\n");
                break;
            default:
                cprintk(WARN, "unknown error\n");
                break;
        }
        panic("sbi error\n");
    }
}

/*
 * a7: sbi_type
 * a6: function number if applicable
 * a0 - a5: arguments
 * after ecall, a0: error, a1: return value
*/
uint64_t machine_call(uint64_t sbi_EID, uint64_t sbi_FID, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    uint64_t result;
    register uint64_t a7 asm("a7") = sbi_EID;
    register uint64_t a6 asm("a6") = sbi_FID;
    register uint64_t a0 asm("a0") = arg0;
    register uint64_t a1 asm("a1") = arg1;
    register uint64_t a2 asm("a2") = arg2;
    asm volatile ("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a0), "r"(a1), "r"(a7), "r"(a6), "r"(a2)
        : "memory");
    // judge_error(a0);
    result = a0;
    return result;
}


uint64_t sbi_get_spec_version(void) {
    return (uint64_t)machine_call(0x10, 0, 0, 0, 0);
}

void sbi_console_putchar(unsigned char ch) {
    machine_call(SBI_CONSOLE_PUTCHAR, 0, ch, 0, 0);
}

uint8_t sbi_console_getchar(void) {
    return (uint8_t)machine_call(SBI_CONSOLE_GETCHAR, 0, 0, 0, 0);
}

void sbi_shutdown(void) {
    machine_call(SRST_EXTENSION, 0, 0, 0, 0);
}

void sbi_set_timer(uint64_t stime_value) {
    machine_call(TIMER_EXTENSION, 0, stime_value, 0, 0);
}