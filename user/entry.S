
    .section .text.entry
    .global _entry
_entry:
    la sp, user_stack_top
    j lib_main

# bss段内存
    .section .bss.stack
    .globl user_stack
user_stack:
    .space 4096 * 16
    .globl user_stack_top
user_stack_top:

loop:
    j loop
