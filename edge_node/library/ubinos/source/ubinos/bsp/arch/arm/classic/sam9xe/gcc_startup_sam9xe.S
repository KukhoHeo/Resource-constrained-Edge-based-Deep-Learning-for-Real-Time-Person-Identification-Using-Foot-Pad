/*
 
Copyright (c) 2009-2018 ARM Limited. All rights reserved.

    SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the License); you may
not use this file except in compliance with the License.
You may obtain a copy of the License at

    www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

NOTICE: This file has been modified by Nordic Semiconductor ASA.

*/

#include <ubinos_config.h>

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)

#include <ubinos/bsp/arch.h>

    .syntax unified
    .arch armv5tej

#if (INCLUDE__UBINOS__UBIK == 1)
    #define IRQ_Handler                                     ubik_irq_handler
    #define SWI_Handler                                     ubik_swi_handler
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

#ifdef __STARTUP_CONFIG
#include "startup_config.h"
#ifndef __STARTUP_CONFIG_STACK_ALIGNEMENT
#define __STARTUP_CONFIG_STACK_ALIGNEMENT 3
#endif
#endif

    .section .stack
#if defined(__STARTUP_CONFIG)
    .align __STARTUP_CONFIG_STACK_ALIGNEMENT
    .equ    Stack_Size, __STARTUP_CONFIG_STACK_SIZE
#elif defined(__STACK_SIZE)
    .align 3
    .equ    Stack_Size, __STACK_SIZE
#else
    .align 3
    .equ    Stack_Size, 8192
#endif
    .globl __StackTop
    .globl __StackLimit
__StackLimit:
    .space Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .isr_vector
    .arm
    .align 2
    .globl __isr_vector
__isr_vector:
    ldr     pc, =Reset_Handler          /* 0x00 Reset */
    ldr     pc, =Undef_handler          /* 0x04 Undefined Instruction */
    ldr     pc, =SWI_Handler            /* 0x08 Software Interrupt */
    ldr     pc, =PABT_handler           /* 0x0C Prefetch Abort */
    ldr     pc, =DABT_handler           /* 0x10 Data Abort */
    nop                                 /* 0x14 reserved */
    ldr     pc, =IRQ_Handler            /* 0x18 IRQ */
    ldr     pc, =FIQ_handler            /* 0x1C FIQ */

    .size __isr_vector, . - __isr_vector

/* Reset Handler */


    .text
    .arm
    .align 2
    .globl Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:

#if 0
error___
/* Disable interrupt */
.L_loop0:
    msr     cpsr_c, #(ARM_MODE_SVC | I_BIT  | F_BIT)
    nop
    mrs     r0, cpsr
    ands    r0, r0, #0x80
    beq     .L_loop0
    mrs     r0, cpsr
    ands    r0, r0, #0x40
    beq     .L_loop0

    mov     r1, #0xFFFFFFFF
    ldr     r0, =AT91C_AIC_IDCR
    str     r1, [r0]
    ldr     r0, =AT91C_AIC_ICCR
    str     r1, [r0]
    ldr     r0, =AT91C_PMC_PCDR
    str     r1, [r0]

#if (UBINOS__BSP__USE_MMU == 1) || (UBINOS__BSP__USE_ICACHE == 1) || (UBINOS__BSP__USE_DCACHE == 1)
/* Disable mmu, d cache, i cache */
    mrc p15, 0, r0, c1, c0, 0
#if (UBINOS__BSP__USE_MMU == 1)
    bic r0, r0, #0x0001
#endif
#if (UBINOS__BSP__USE_ICACHE == 1)
    bic r0, r0, #0x1000     /* instruction cache */
#endif
#if (UBINOS__BSP__USE_DCACHE == 1)
    bic r0, r0, #0x0004     /* data cache */
#endif
    mcr p15, 0, r0, c1, c0, 0
#endif
#endif

/* Setup temporary stack */
    ldr     sp, =__StackTopTemp
Reset_Breakpoint:
/* Execute lowlevel_init function. */
    bl      lowlevel_init

/* Loop to copy data from read only memory to RAM.
 * The ranges of copy from/to are specified by following symbols:
 *      __etext: LMA of start of the section to copy from. Usually end of text
 *      __data_start__: VMA of start of the section to copy to.
 *      __bss_start__: VMA of end of the section to copy to. Normally __data_end__ is used, but by using __bss_start__
 *                    the user can add their own initialized data section before BSS section with the INTERT AFTER command.
 *
 * All addresses must be aligned to 4 bytes boundary.
 */
    ldr r1, =__etext
    ldr r2, =__data_start__
    ldr r3, =__bss_start__

    cmp r1, r2
    beq .L_loop1_done

    subs r3, r3, r2
    ble .L_loop1_done

.L_loop1:
    subs r3, r3, #4
    ldr r0, [r1,r3]
    str r0, [r2,r3]
    bgt .L_loop1

.L_loop1_done:

#if (UBINOS__BSP__USE_RELOCATED_ISR_VECTOR == 1)
/* Loop to copy isr vector to relocated address */

    ldr r1, =__isr_vector_start__
    ldr r2, =__relocated_isr_vector_start__
    ldr r3, =__relocated_isr_vector_end__

    cmp r1, r2
    beq .L_loop2_done

    subs r3, r3, r2
    ble .L_loop2_done

.L_loop2:
    subs r3, r3, #4
    ldr r0, [r1,r3]
    str r0, [r2,r3]
    bgt .L_loop2

.L_loop2_done:
#endif /* (UBINOS__BSP__USE_RELOCATED_ISR_VECTOR == 1) */

/* This part of work usually is done in C library startup code. Otherwise,
 * define __STARTUP_CLEAR_BSS to enable it in this startup. This section
 * clears the RAM where BSS data is located.
 *
 * The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 * All addresses must be aligned to 4 bytes boundary.
 */
#ifdef __STARTUP_CLEAR_BSS
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    movs r0, 0

    subs r2, r2, r1
    ble .L_loop3_done

.L_loop3:
    subs r2, r2, #4
    str r0, [r1, r2]
    bgt .L_loop3

.L_loop3_done:
#endif /* __STARTUP_CLEAR_BSS */

/* Execute SystemInit function. */
    bl SystemInit

#if 0
/* Setup the stack for each mode */
error___
    ldr     r0, =__stack

    msr     cpsr_c, #ARM_MODE_ABT | I_BIT | F_BIT
    mov     sp, r0
    sub     r0, r0, #UBINOS__BSP__STACK_SIZE_ABT

    msr     cpsr_c, #ARM_MODE_UND | I_BIT | F_BIT
    mov     sp, r0
    sub     r0, r0, #UBINOS__BSP__STACK_SIZE_UND

    msr     cpsr_c, #ARM_MODE_FIQ | I_BIT | F_BIT
    mov     sp, r0
    sub     r0, r0, #UBINOS__BSP__STACK_SIZE_FIQ

    msr     cpsr_c, #ARM_MODE_IRQ | I_BIT | F_BIT
    mov     sp, r0
    sub     r0, r0, #UBINOS__BSP__STACK_SIZE_IRQ

    msr     cpsr_c, #ARM_MODE_SVC | I_BIT | F_BIT
    mov     sp, r0
    sub     r0, r0, #UBINOS__BSP__STACK_SIZE_SVC

    msr     cpsr_c, #ARM_MODE_SYS | I_BIT | F_BIT
    mov     sp, r0
    sub     r0, r0, #UBINOS__BSP__STACK_SIZE_SYS

    msr     cpsr_c, #ARM_MODE_SVC | I_BIT  | F_BIT
#endif

/* Call _start function provided by libraries.
 * If those libraries are not accessible, define __START as your entry point.
 */
#ifndef __START
#define __START _start
#endif
    bl __START

    .pool
    .size   Reset_Handler,.-Reset_Handler

    .section ".text"


#if !(INCLUDE__UBINOS__UBIK == 1)
#endif /* !(INCLUDE__UBINOS__UBIK == 1) */


/* Exception Handlers (infinite loops which can be modified) */

    .weak   Undef_handler
    .type   Undef_handler, %function
Undef_handler:                 /* Undefined Instruction */
    stmfd   sp!, {r0, lr}
    mrs     r0, spsr
    stmfd   sp!, {r0}
    mrs     r0, cpsr
    msr     cpsr_c, #ARM_MODE_SVC | I_BIT
    stmfd   sp!, {r0-r12, lr}
    mov     r0, #EXCEPTION_TYPE_UND
    bl      bsp_exception_handler
    ldmfd   sp!, {r0-r12, lr}
    msr     cpsr, r0
    ldmfd   sp!, {r0}
    msr     spsr, r0
    ldmfd   sp!, {r0, pc}^
    .size   Undef_handler, . - Undef_handler


    .weak   PABT_handler
    .type   PABT_handler, %function
PABT_handler:                  /* Prefetch Abort */
    sub     lr, lr, #4
    stmfd   sp!, {r0, lr}
    mrs     r0, spsr
    stmfd   sp!, {r0}
    mrs     r0, cpsr
    msr     cpsr_c, #ARM_MODE_SVC | I_BIT
    stmfd   sp!, {r0-r12, lr}
    mov     r0, #EXCEPTION_TYPE_PABT
    bl      bsp_exception_handler
    ldmfd   sp!, {r0-r12, lr}
    msr     cpsr, r0
    ldmfd   sp!, {r0}
    msr     spsr, r0
    ldmfd   sp!, {r0, pc}^
    .size   PABT_handler, . - PABT_handler


    .weak   DABT_handler
    .type   DABT_handler, %function
DABT_handler:                  /* Data Abort */
    sub     lr, lr, #8
    stmfd   sp!, {r0, lr}
    mrs     r0, spsr
    stmfd   sp!, {r0}
    mrs     r0, cpsr
    msr     cpsr_c, #ARM_MODE_SVC | I_BIT
    stmfd   sp!, {r0-r12, lr}
    mov     r0, #EXCEPTION_TYPE_DABT
    bl      bsp_exception_handler
    ldmfd   sp!, {r0-r12, lr}
    msr     cpsr, r0
    ldmfd   sp!, {r0}
    msr     spsr, r0
    ldmfd   sp!, {r0, pc}^
    .size   DABT_handler, . - DABT_handler

#if !(INCLUDE__UBINOS__UBIK == 1)

    .weak   SWI_Handler
    .type   SWI_Handler, %function
SWI_Handler:                   /* Software Interrupt */
    stmfd   sp!, {r0-r12, lr}
    mrs     r1, spsr
    stmfd   sp!, {r1}
    ands    r1, r1, #0x20
    ldrh    r1, [lr, #-2]
    bicne   r1, r1, #0xffffff00
    ldreq   r1, [lr, #-4]
    biceq   r1, r1, #0xff000000
    mov     r0, #EXCEPTION_TYPE_SWI
    bl      bsp_exception_handler
    ldmfd   sp!, {r0}
    msr     spsr, r0
    ldmfd   sp!, {r0-r12, pc}^
    .size   SWI_Handler, . - SWI_Handler


    .weak   IRQ_Handler
    .type   IRQ_Handler, %function
IRQ_Handler:                   /* IRQ */
    sub     lr, lr, #4
    stmfd   sp!, {r0, lr}
    mrs     r0, spsr
    stmfd   sp!, {r0}
    mrs     r0, cpsr
    msr     cpsr_c, #ARM_MODE_SVC | I_BIT
    stmfd   sp!, {r0-r12, lr}
    mov     r0, #EXCEPTION_TYPE_IRQ
    bl      bsp_exception_handler
    ldmfd   sp!, {r0-r12, lr}
    msr     cpsr, r0
    ldmfd   sp!, {r0}
    msr     spsr, r0
    ldmfd   sp!, {r0, pc}^
    .size   IRQ_Handler, . - IRQ_Handler

#endif /* !(INCLUDE__UBINOS__UBIK == 1) */

    .weak   FIQ_handler
    .type   FIQ_handler, %function
FIQ_handler:                   /* FIQ */
    sub     lr, lr, #4
    stmfd   sp!, {r0, lr}
    mrs     r0, spsr
    stmfd   sp!, {r0}
    mrs     r0, cpsr
    msr     cpsr_c, #ARM_MODE_SVC | I_BIT | F_BIT
    stmfd   sp!, {r0-r12, lr}
    mov     r0, #EXCEPTION_TYPE_FIQ
    bl      bsp_exception_handler
    ldmfd   sp!, {r0-r12, lr}
    msr     cpsr, r0
    ldmfd   sp!, {r0}
    msr     spsr, r0
    ldmfd   sp!, {r0, pc}^
    .size   FIQ_handler, . - FIQ_handler

  .end

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

