    .section .text.entry
    .globl _entry
_entry:
    /* setup stack */
    la    sp, boot_stack_top          # setup stack pointer
    call  main_start

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 * 16
    .globl boot_stack_top
boot_stack_top:

loop:
    j loop
