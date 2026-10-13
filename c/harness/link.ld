/* Linker script for the differential harness (xgcc_sail oracle).
 * Copied from ~/Documents/code/ship-your-lua/c/src/link.ld (itself from
 * ship-your-interpreter's c/src/link.ld) with the .lua_chunk region
 * removed. RAM starts at 0x80000000, 128 MiB, 8 MiB stack at the top;
 * the heap runs from _end to __heap_end. */
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

__stack_size = 8M;

SECTIONS
{
    .text : {
        *(.text.init)
        *(.text .text.*)
    } > RAM

    .rodata : ALIGN(8) {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    } > RAM

    /* HTIF mailbox (tohost/fromhost); found by section name. */
    .tohost : ALIGN(64) {
        *(.tohost)
    } > RAM

    .data : ALIGN(8) {
        __global_pointer$ = . + 0x800;
        *(.data .data.*)
        *(.sdata .sdata.*)
    } > RAM

    .bss : ALIGN(8) {
        __bss_start = .;
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM

    . = ALIGN(16);
    _end = .;
    PROVIDE(end = .);

    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
    __heap_end = __stack_top - __stack_size;
}
