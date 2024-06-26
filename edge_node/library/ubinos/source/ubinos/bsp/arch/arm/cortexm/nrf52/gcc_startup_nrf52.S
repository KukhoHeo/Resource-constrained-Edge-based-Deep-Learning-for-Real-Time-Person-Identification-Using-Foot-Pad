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
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA)

    .syntax unified
    .arch armv7e-m

#ifdef __STARTUP_CONFIG
#include "startup_config.h"
#ifndef __STARTUP_CONFIG_STACK_ALIGNEMENT
#define __STARTUP_CONFIG_STACK_ALIGNEMENT 3
#endif
#endif


#if (INCLUDE__UBINOS__UBIK == 1)

#define _NMI_Handler                 bsp_ubik_irq_handler
#define _HardFault_Handler           bsp_ubik_irq_handler
#define _MemoryManagement_Handler    bsp_ubik_irq_handler
#define _BusFault_Handler            bsp_ubik_irq_handler
#define _UsageFault_Handler          bsp_ubik_irq_handler

#define _SVC_Handler                 bsp_ubik_swi_handler
#define _DebugMon_Handler            bsp_ubik_irq_handler

#define _PendSV_Handler              bsp_ubik_psv_handler

	#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#define _SysTick_Handler             0
		#if (UBINOS__UBIK__TICK_RTC_NO == 0)
#define _RTC0_IRQHandler             bsp_ubik_tick_handler
#define _RTC1_IRQHandler             RTC1_IRQHandler
#define _RTC2_IRQHandler             RTC2_IRQHandler
		#elif (UBINOS__UBIK__TICK_RTC_NO == 1)
#define _RTC0_IRQHandler             RTC0_IRQHandler
#define _RTC1_IRQHandler             bsp_ubik_tick_handler
#define _RTC2_IRQHandler             RTC2_IRQHandler
		#elif (UBINOS__UBIK__TICK_RTC_NO == 2)
#define _RTC0_IRQHandler             RTC0_IRQHandler
#define _RTC1_IRQHandler             RTC1_IRQHandler
#define _RTC2_IRQHandler             bsp_ubik_tick_handler
		#else
			#error "Unsupported UBINOS__UBIK__TICK_RTC_NO"
		#endif
	#else
#define _SysTick_Handler             bsp_ubik_tick_handler
#define _RTC0_IRQHandler             RTC0_IRQHandler
#define _RTC1_IRQHandler             RTC1_IRQHandler
#define _RTC2_IRQHandler             RTC2_IRQHandler
	#endif

#else

#define _NMI_Handler                 NMI_Handler
#define _HardFault_Handler           HardFault_Handler
#define _MemoryManagement_Handler    MemoryManagement_Handler
#define _BusFault_Handler            BusFault_Handler
#define _UsageFault_Handler          UsageFault_Handler

#define _SVC_Handler                 SVC_Handler
#define _DebugMon_Handler            DebugMon_Handler

#define _PendSV_Handler              PendSV_Handler

#define _SysTick_Handler             SysTick_Handler
#define _RTC0_IRQHandler             RTC0_IRQHandler
#define _RTC1_IRQHandler             RTC1_IRQHandler
#define _RTC2_IRQHandler             RTC2_IRQHandler

#endif /* (INCLUDE__UBINOS__UBIK == 1) */


#if   (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA)

#define _USBD_IRQHandler             0
#define _UARTE1_IRQHandler           0
#define _QSPI_IRQHandler             0
#define _CRYPTOCELL_IRQHandler       0

#define _PWM3_IRQHandler             0

#define _SPIM3_IRQHandler            0

#elif (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA)

#define _USBD_IRQHandler             USBD_IRQHandler
#define _UARTE1_IRQHandler           UARTE1_IRQHandler
#define _QSPI_IRQHandler             UARTE1_IRQHandler
#define _CRYPTOCELL_IRQHandler       CRYPTOCELL_IRQHandler

#define _PWM3_IRQHandler             PWM3_IRQHandler

#define _SPIM3_IRQHandler            SPIM3_IRQHandler

#else
    #error "Unsupported UBINOS__BSP__CPU_MODEL"
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
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                  /* Top of Stack */
    .long   Reset_Handler
    .long   _NMI_Handler                /* NMI_Handler */
    .long   _HardFault_Handler          /* HardFault_Handler */
    .long   _MemoryManagement_Handler   /* MemoryManagement_Handler */
    .long   _BusFault_Handler           /* BusFault_Handler */
    .long   _UsageFault_Handler         /* UsageFault_Handler */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   _SVC_Handler                /* SVC_Handler */
    .long   _DebugMon_Handler           /* DebugMon_Handler */
    .long   0                           /* Reserved */
    .long   _PendSV_Handler             /* PendSV_Handler */
    .long   _SysTick_Handler            /* SysTick_Handler */

  /* External Interrupts */
    .long   POWER_CLOCK_IRQHandler
    .long   RADIO_IRQHandler
    .long   UARTE0_UART0_IRQHandler
    .long   SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
    .long   SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
    .long   NFCT_IRQHandler
    .long   GPIOTE_IRQHandler
    .long   SAADC_IRQHandler
    .long   TIMER0_IRQHandler
    .long   TIMER1_IRQHandler
    .long   TIMER2_IRQHandler
    .long   _RTC0_IRQHandler
    .long   TEMP_IRQHandler
    .long   RNG_IRQHandler
    .long   ECB_IRQHandler
    .long   CCM_AAR_IRQHandler
    .long   WDT_IRQHandler
    .long   _RTC1_IRQHandler
    .long   QDEC_IRQHandler
    .long   COMP_LPCOMP_IRQHandler
    .long   SWI0_EGU0_IRQHandler
    .long   SWI1_EGU1_IRQHandler
    .long   SWI2_EGU2_IRQHandler
    .long   SWI3_EGU3_IRQHandler
    .long   SWI4_EGU4_IRQHandler
    .long   SWI5_EGU5_IRQHandler
    .long   TIMER3_IRQHandler
    .long   TIMER4_IRQHandler
    .long   PWM0_IRQHandler
    .long   PDM_IRQHandler
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   MWU_IRQHandler
    .long   PWM1_IRQHandler
    .long   PWM2_IRQHandler
    .long   SPIM2_SPIS2_SPI2_IRQHandler
    .long   _RTC2_IRQHandler
    .long   I2S_IRQHandler
    .long   FPU_IRQHandler
    .long   _USBD_IRQHandler
    .long   _UARTE1_IRQHandler
    .long   _QSPI_IRQHandler
    .long   _CRYPTOCELL_IRQHandler
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   _PWM3_IRQHandler
    .long   0                           /*Reserved */
    .long   _SPIM3_IRQHandler
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */

    .size __isr_vector, . - __isr_vector


/* Reset Handler */

    .text
    .thumb
    .thumb_func
    .align 1
    .globl Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
/* set temporary stack pointer */
    ldr sp, =__StackTopTemp
Reset_Breakpoint:

#if (UBINOS__BSP__USE_RELOCATED_ISR_VECTOR == 1)
/* Loop to copy isr vector to relocated address */

    ldr r1, =__isr_vector_start__
    ldr r2, =__relocated_isr_vector_start__
    ldr r3, =__relocated_isr_vector_end__

    cmp r1, r2
    beq .L_loop0_done

    subs r3, r3, r2
    ble .L_loop0_done

.L_loop0:
    subs r3, r3, #4
    ldr r0, [r1,r3]
    str r0, [r2,r3]
    bgt .L_loop0

.L_loop0_done:

#if (UBINOS__BSP__NRF52_SOFTDEVICE_PRESENT == 1) || (UBINOS__BSP__NRF52_MBR_PRESENT == 1)
/* Copy handlers of softdevice to relocated address */
    movs r1, #0
    ldr r2, =__relocated_isr_vector_start__
    ldr r3, =__relocated_isr_vector_end__

#if (INCLUDE__UBINOS__UBIK == 1)
/* Skip ~ SysTick_Handler of softdevice */
    adds r1, r1, #64
    adds r2, r2, #64
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

    cmp r1, r2
    beq .L_loop0_2_done

    subs r3, r3, r2
    ble .L_loop0_2_done

.L_loop0_2:
    subs r3, r3, #4
    ldr r0, [r1,r3]
    str r0, [r2,r3]
    bgt .L_loop0_2

.L_loop0_2_done:

#if (INCLUDE__UBINOS__UBIK == 1)
/* Copy SVC_Handler of softdevice to relocated address */
    movs r1, #0
    ldr r2, =__relocated_isr_vector_start__
    mov r3, #44
    ldr r0, [r1,r3]
    str r0, [r2,r3]
#endif /* (INCLUDE__UBINOS__UBIK == 1) */
#endif /* (UBINOS__BSP__NRF52_SOFTDEVICE_PRESENT == 1) || (UBINOS__BSP__NRF52_MBR_PRESENT == 1) */
#endif /* (UBINOS__BSP__USE_RELOCATED_ISR_VECTOR == 1) */

/* Execute SystemInit function. (SystemInit function should not use data section.) */
    bl SystemInit

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
    ble .L_loop2_done

.L_loop2:
    subs r2, r2, #4
    str r0, [r1, r2]
    bgt .L_loop2

.L_loop2_done:
#endif /* __STARTUP_CLEAR_BSS */

/* set stack pointer */
    ldr sp, =__StackTop

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

/* Dummy Exception Handlers (infinite loops which can be modified) */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    b       .
    .size   NMI_Handler, . - NMI_Handler


    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    b       .
    .size   HardFault_Handler, . - HardFault_Handler


    .weak   MemoryManagement_Handler
    .type   MemoryManagement_Handler, %function
MemoryManagement_Handler:
    b       .
    .size   MemoryManagement_Handler, . - MemoryManagement_Handler


    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    b       .
    .size   BusFault_Handler, . - BusFault_Handler


    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    b       .
    .size   UsageFault_Handler, . - UsageFault_Handler


    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    b       .
    .size   SVC_Handler, . - SVC_Handler


    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    b       .
    .size   DebugMon_Handler, . - DebugMon_Handler


    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    b       .
    .size   PendSV_Handler, . - PendSV_Handler


    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    b       .
    .size   SysTick_Handler, . - SysTick_Handler

#endif /* !(INCLUDE__UBINOS__UBIK == 1) */

/* IRQ Handlers */

    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
#if (INCLUDE__UBINOS__UBIK == 1)
	b       bsp_ubik_irq_handler
#else
    b       .
#endif /* (INCLUDE__UBINOS__UBIK == 1) */
    .size   Default_Handler, . - Default_Handler

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm

    IRQ  POWER_CLOCK_IRQHandler
    IRQ  RADIO_IRQHandler
    IRQ  UARTE0_UART0_IRQHandler
    IRQ  SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
    IRQ  SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
    IRQ  NFCT_IRQHandler
    IRQ  GPIOTE_IRQHandler
    IRQ  SAADC_IRQHandler
    IRQ  TIMER0_IRQHandler
    IRQ  TIMER1_IRQHandler
    IRQ  TIMER2_IRQHandler
    IRQ  RTC0_IRQHandler
    IRQ  TEMP_IRQHandler
    IRQ  RNG_IRQHandler
    IRQ  ECB_IRQHandler
    IRQ  CCM_AAR_IRQHandler
    IRQ  WDT_IRQHandler
    IRQ  RTC1_IRQHandler
    IRQ  QDEC_IRQHandler
    IRQ  COMP_LPCOMP_IRQHandler
    IRQ  SWI0_EGU0_IRQHandler
    IRQ  SWI1_EGU1_IRQHandler
    IRQ  SWI2_EGU2_IRQHandler
    IRQ  SWI3_EGU3_IRQHandler
    IRQ  SWI4_EGU4_IRQHandler
    IRQ  SWI5_EGU5_IRQHandler
    IRQ  TIMER3_IRQHandler
    IRQ  TIMER4_IRQHandler
    IRQ  PWM0_IRQHandler
    IRQ  PDM_IRQHandler
    IRQ  MWU_IRQHandler
    IRQ  PWM1_IRQHandler
    IRQ  PWM2_IRQHandler
    IRQ  SPIM2_SPIS2_SPI2_IRQHandler
    IRQ  RTC2_IRQHandler
    IRQ  I2S_IRQHandler
    IRQ  FPU_IRQHandler
#if   (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA)
#elif (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA)
    IRQ  USBD_IRQHandler
    IRQ  UARTE1_IRQHandler
    IRQ  QSPI_IRQHandler
    IRQ  CRYPTOCELL_IRQHandler
    IRQ  PWM3_IRQHandler
    IRQ  SPIM3_IRQHandler
#else
    #error "Unsupported UBINOS__BSP__CPU_MODEL"
#endif

  .end

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

