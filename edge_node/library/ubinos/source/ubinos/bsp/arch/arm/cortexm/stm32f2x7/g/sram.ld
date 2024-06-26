/* Linker script for Nordic Semiconductor nRF devices
 *
 * Version: Sourcery G++ 4.5-1
 * Support: https://support.codesourcery.com/GNUToolchain/
 *
 * Copyright (c) 2007, 2008, 2009, 2010 CodeSourcery, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

MEMORY
{
    FLASH (rx) : ORIGIN = <FLASH_ORIGIN>, LENGTH = <FLASH_LENGTH>
    FLASH2 (rx) : ORIGIN = <FLASH2_ORIGIN>, LENGTH = <FLASH2_LENGTH>
    RAM (rwx) : ORIGIN = <RAM_ORIGIN>, LENGTH = <RAM_LENGTH>
    RAM2 (rwx) : ORIGIN = <RAM2_ORIGIN>, LENGTH = <RAM2_LENGTH>
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapBase
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __StackTopTemp
 *   __stack
 */

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        __text_start__ = .;

        __isr_vector_start__ = .;
        __relocated_isr_vector_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        __isr_vector_end__ = .;
        __relocated_isr_vector_end__ = .;

        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > RAM
    
    .mem_section_dummy_rom :
    {
        . = ALIGN(4);
    } > RAM

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > RAM

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > RAM
    __exidx_end = .;

    __text_end__ = .;
    __etext = .;
}

SECTIONS
{
    .data :
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;
    } > RAM

    .mem_section_dummy_ram __data_end__ :
    {
        . = ALIGN(4);
        PROVIDE(__stop_mem_section_dummy_ram = .);
    } > RAM

    .bss __stop_mem_section_dummy_ram :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        __end__ = .;
        PROVIDE(end = .);
    } > RAM

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (DSECT) :
    {
        KEEP(*(.stack*))
    }

    .heap __bss_end__ (COPY):
    {
        . = ALIGN(4);
        __HeapBase = .;
        PROVIDE(__heap_base = .);
        . = ORIGIN(RAM) + LENGTH(RAM) - SIZEOF(.stack_dummy);
        . = ALIGN(4);
        __HeapLimit = .;
        PROVIDE(__heap_limit = .);
    } > RAM

    .stack __HeapLimit (COPY):
    {
        . = ALIGN(8);
        __StackLimit = .;
        PROVIDE(__stack_limit = .);
        . = . + SIZEOF(.stack_dummy);
        . = ALIGN(8);
        __StackTop = .;
        PROVIDE(__stack = .);
    } > RAM

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}

SECTIONS
{
    __StackTopTemp = ORIGIN(RAM) + LENGTH(RAM);

    __start_ot_flash_data = ORIGIN(FLASH2);
    __stop_ot_flash_data = ORIGIN(FLASH2) + LENGTH(FLASH2);

    /* Check if data + heap + stack exceeds limit */
    ASSERT(__StackLimit >= __HeapLimit, "region overflowed with stack")
    
    /* Check if text sections + data exceeds FLASH limit */
    DataInitFlashUsed = 0;
    CodeFlashUsed = 0;
    TotalFlashUsed = CodeFlashUsed + DataInitFlashUsed;
    ASSERT(TotalFlashUsed <= LENGTH(FLASH), "region FLASH overflowed with .data and user data")
    
    __flash_start__                 = ORIGIN(FLASH);
    __flash_size__                  = LENGTH(FLASH);
    __flash_usage__                 = TotalFlashUsed;
    __ram_start__                   = ORIGIN(RAM);
    __ram_size__                    = LENGTH(RAM);
    __ram_static_usage__            = __bss_end__ - ORIGIN(RAM);
    __flash2_start__                = ORIGIN(FLASH2);
    __flash2_size__                 = LENGTH(FLASH2);
    __flash2_usage__                = LENGTH(FLASH2);
    __ram2_start__                  = ORIGIN(RAM2);
    __ram2_size__                   = LENGTH(RAM2);
    __ram2_static_usage__           = 0;
    __entry_address__               = Reset_Handler;
    __isr_vector_size__             = __isr_vector_end__ - __isr_vector_start__;
    __text_size__                   = __text_end__ - __text_start__;
    __relocated_isr_vector_size__   = __relocated_isr_vector_end__ - __relocated_isr_vector_start__;
    __data_size__                   = SIZEOF(.data);
    __bss_size__                    = SIZEOF(.bss);
    __stack_top__                   = __StackTop;
    __stack_limit__                 = __StackLimit;
    __stack_size__                  = SIZEOF(.stack_dummy);
    __heap_base__                   = __HeapBase;
    __heap_limit__                  = __HeapLimit;
    __heap_size__                   = SIZEOF(.heap);
}


