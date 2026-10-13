/* Bare-metal startup for the differential harness (xgcc_sail oracle).
 * Copied from ~/Documents/code/ship-your-lua/c/src/crt0.S (itself from
 * ship-your-interpreter). Change: main gets argc=1, argv={"prog",NULL}
 * instead of argc=0, argv=NULL, as a hosted run (gcc_native, ccomp -interp,
 * tcc -run) would give it, so corpus programs that read argv[0] or test
 * argc behave the same everywhere. */
    .file "crt0.S"
    .section .text.init
    .globl _start
_start:
.option push
.option norelax
    la      gp, __global_pointer$
.option pop
    la      sp, __stack_top
    /* clear .bss (linker script guarantees 8-byte alignment) */
    la      t0, __bss_start
    la      t1, __bss_end
1:  bgeu    t0, t1, 2f
    sd      zero, 0(t0)
    addi    t0, t0, 8
    j       1b
2:
    li      a0, 1
    la      a1, __harness_argv
    li      a2, 0
    call    main
    tail    exit

    .section .rodata
__harness_arg0:
    .asciz  "prog"
    .section .data
    .balign 8
__harness_argv:
    .dword  __harness_arg0
    .dword  0
