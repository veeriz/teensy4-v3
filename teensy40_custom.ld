MEMORY
{
    ITCM  (rwx): ORIGIN = 0x00000000, LENGTH = 512K
    DTCM  (rwx): ORIGIN = 0x20000000, LENGTH = 512K
    RAM   (rwx): ORIGIN = 0x20200000, LENGTH = 512K
    FLASH (rwx): ORIGIN = 0x60000000, LENGTH = 1984K
}

SECTIONS
{
    .text : {
        . = ALIGN(4);
        KEEP(*(.vectors))
        *(.startup*)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > DTCM AT > FLASH

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > DTCM
}