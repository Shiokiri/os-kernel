#include "types.h"
#include "trap.h"
#include "riscv.h"
#include "syscall.h"

extern void app_0_start(void);
extern void app_0_end(void);
extern void user_stack_top(void);
extern void boot_stack_top(void);

#define APP_ADDRESS 0x80400000

uint64_t get_user_stack_top(void) {
    return user_stack_top;
}

uint64_t get_kernel_stack_top(void) {
    return boot_stack_top;
}

// load app to 0x80400000
void load_app(void) {
    __asm__ volatile ("fence.i");
    uint64_t* app_start = (uint64_t*)app_0_start;
    uint64_t* app_end = (uint64_t*)app_0_end;
    uint64_t* app_address = (uint64_t*)APP_ADDRESS;
    while (app_start <= app_end) {
        *app_address = *app_start;
        app_start++;
        app_address++;
    }
}

void run_app(void) {
    const uint64_t sstatus = r_sstatus();
    static TrapContext tc = {
        {0},
        0,
        APP_ADDRESS
    };
    tc.sstatus = sstatus & (~SSTATUS_SPP);
    tc.regs[2] = get_user_stack_top(); // 保证sscratch指向user_stack
    // 将tc压入kernel_stack
    uint64_t store_posi = get_kernel_stack_top() - sizeof(TrapContext);
    *(TrapContext *)store_posi = tc;
    extern void __restore(TrapContext *cx);
    __restore(&tc);
}

#define UserEnvCall 8

TrapContext *trap_handler(TrapContext *cx) {
    // printk("[kernel] trap handler\n");
    uint64_t scause = r_scause();
    uint64_t stval = r_stval();
    // printk("[Trap] scause = 0x%x\n", scause);
    // 根据原因处理trap
    uint64_t trap = scause & 0x0fff;
    if (scause & 0x8000 == 1) { // interrupt
        panic("can't identify interrupt trap");
    } else { // Exception
        switch (trap) {
            case UserEnvCall:
                cx->sepc += 4;
                cx->regs[10] = syscall(cx->regs[17], cx->regs[10], cx->regs[11], cx->regs[12]);
                break;
            default:
                panic("trap scause undefined.");
                break;
        }
    }
    return cx;
}