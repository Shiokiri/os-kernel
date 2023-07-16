# os-kernel

## Notes

### 32个通用寄存器

|  x0  | zero | Hard-wired zero  | 硬件零 |
|  ----  | ----  |   ----  | ----  |
|  x1  | ra | Return address	 | 常用于保存函数的返回地址 |
|  x2  | sp | Stack pointer  | 栈顶指针  |
|  x3  | gp | Global pointer  | — |
|  x4  | tp | Thread pointer  | — |
|  x5-7  | t0-2 | Temporary  | 临时寄存器 |
|  x8  | s0/fp |  Saved Register/ Frame pointer | 函数调用时保存的寄存器和栈顶指针 |
|  x9  | s1 |  Saved register	 | 函数调用时保存的寄存器 |
|  x10-11  | a0-1 | Function argument/ return value  | 函数调用时的参数/函数的返回值 |
|  x12-17  | a2-7 | Function argument  | 函数调用时的参数 |
|  x18-27  | s2-11 |  Saved register   | 函数调用时保存的寄存器 |
|  x28-31  | t3-6 |  Temporary | 临时寄存器 |

### RISC-V 中修改机器的特权级

通过修改mstatus寄存器的MPP / MPIE位来修改机器的特权级

### System Call

x17: a7

x10: a0

x11: a1

x12: a2

### trap流程

user mode中调用syscall，其中调用ecall，产生异常id为8,然后根据stevc中的地址跳转至alltraps

其中alltraps中保存寄存器，跳转至trap_handler
