/* RV64GC acceptance probe: enable FP state, then M, C, F, D, fence.i and HTIF. */
.section .text.init
.globl _start
_start:
    la sp, __stack_top
    li t0, 0x112d
    csrs misa, t0
    li t0, 0x6000
    csrs mstatus, t0
    li a0, 6
    li a1, 7
.globl probe_mul
probe_mul:
    mul a0, a0, a1
.globl probe_compressed
probe_compressed:
    c.nop
.globl probe_float
probe_float:
    fcvt.s.w fa0, a0
.globl probe_double
probe_double:
    fcvt.d.s fa1, fa0
    fadd.d fa1, fa1, fa1
    fcvt.w.d a0, fa1, rtz
    li t0, 84
    bne a0, t0, fail
.globl probe_fence
probe_fence:
    fence.i
    li a0, 1
    j finish
fail:
    li a0, 3
finish:
    la t0, tohost
.globl probe_exit
probe_exit:
    sd a0, 0(t0)
1:  j 1b
.section .tohost,"aw",@progbits
.balign 64
.globl tohost
 tohost: .dword 0
.globl fromhost
 fromhost: .dword 0
