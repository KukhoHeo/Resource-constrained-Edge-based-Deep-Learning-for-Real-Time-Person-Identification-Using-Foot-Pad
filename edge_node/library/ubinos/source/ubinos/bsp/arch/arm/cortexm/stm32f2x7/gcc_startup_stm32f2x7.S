/**
  ******************************************************************************
  * @file      startup_stm32f217xx.s
  * @author    MCD Application Team
  * @brief     STM32F217xx Devices vector table for Atollic TrueSTUDIO toolchain. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M3 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include <ubinos_config.h>

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__STM32_STM32F2X7 == 1)
#if !(UBINOS__BSP__EXCLUDE_ARCH_INIT == 1)

  .syntax unified
  .cpu cortex-m3
  .thumb

    .section .stack
#if defined(__STACK_SIZE)
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
#if (INCLUDE__UBINOS__UBIK == 1)
    .long   bsp_ubik_irq_handler        /* NMI_Handler */
    .long   bsp_ubik_irq_handler        /* HardFault_Handler */
    .long   bsp_ubik_irq_handler        /* MemoryManagement_Handler */
    .long   bsp_ubik_irq_handler        /* BusFault_Handler */
    .long   bsp_ubik_irq_handler        /* UsageFault_Handler */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   bsp_ubik_swi_handler        /* SVC_Handler */
    .long   bsp_ubik_irq_handler        /* DebugMon_Handler */
    .long   0                           /* Reserved */
    .long   bsp_ubik_psv_handler        /* PendSV_Handler */
#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
	.long   SysTick_Handler             /* SysTick_Handler */
#else
	.long   bsp_ubik_tick_handler       /* SysTick_Handler */
#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */
#else
    .long   NMI_Handler
    .long   HardFault_Handler
    .long   MemManage_Handler
    .long   BusFault_Handler
    .long   UsageFault_Handler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   SVC_Handler
    .long   DebugMon_Handler
    .long   0                           /* Reserved */
    .long   PendSV_Handler
    .long   SysTick_Handler
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

  /* External Interrupts */
    .long   WWDG_IRQHandler                   /* Window WatchDog              */
    .long   PVD_IRQHandler                    /* PVD through EXTI Line detection */
    .long   TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */
    .long   RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */
    .long   FLASH_IRQHandler                  /* FLASH                        */
    .long   RCC_IRQHandler                    /* RCC                          */
    .long   EXTI0_IRQHandler                  /* EXTI Line0                   */
    .long   EXTI1_IRQHandler                  /* EXTI Line1                   */
    .long   EXTI2_IRQHandler                  /* EXTI Line2                   */
    .long   EXTI3_IRQHandler                  /* EXTI Line3                   */
    .long   EXTI4_IRQHandler                  /* EXTI Line4                   */
    .long   DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */
    .long   DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */
    .long   DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */
    .long   DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */
    .long   DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */
    .long   DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */
    .long   DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */
    .long   ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */
    .long   CAN1_TX_IRQHandler                /* CAN1 TX                      */
    .long   CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
    .long   CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
    .long   CAN1_SCE_IRQHandler               /* CAN1 SCE                     */
    .long   EXTI9_5_IRQHandler                /* External Line[9:5]s          */
    .long   TIM1_BRK_TIM9_IRQHandler          /* TIM1 Break and TIM9          */
    .long   TIM1_UP_TIM10_IRQHandler          /* TIM1 Update and TIM10        */
    .long   TIM1_TRG_COM_TIM11_IRQHandler     /* TIM1 Trigger and Commutation and TIM11 */
    .long   TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
    .long   TIM2_IRQHandler                   /* TIM2                         */
    .long   TIM3_IRQHandler                   /* TIM3                         */
    .long   TIM4_IRQHandler                   /* TIM4                         */
    .long   I2C1_EV_IRQHandler                /* I2C1 Event                   */
    .long   I2C1_ER_IRQHandler                /* I2C1 Error                   */
    .long   I2C2_EV_IRQHandler                /* I2C2 Event                   */
    .long   I2C2_ER_IRQHandler                /* I2C2 Error                   */
    .long   SPI1_IRQHandler                   /* SPI1                         */
    .long   SPI2_IRQHandler                   /* SPI2                         */
    .long   USART1_IRQHandler                 /* USART1                       */
    .long   USART2_IRQHandler                 /* USART2                       */
    .long   USART3_IRQHandler                 /* USART3                       */
    .long   EXTI15_10_IRQHandler              /* External Line[15:10]s        */
    .long   RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
    .long   OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI line */
    .long   TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */
    .long   TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */
    .long   TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
    .long   TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */
    .long   DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */
    .long   FSMC_IRQHandler                   /* FSMC                         */
    .long   SDIO_IRQHandler                   /* SDIO                         */
    .long   TIM5_IRQHandler                   /* TIM5                         */
    .long   SPI3_IRQHandler                   /* SPI3                         */
    .long   UART4_IRQHandler                  /* UART4                        */
    .long   UART5_IRQHandler                  /* UART5                        */
    .long   TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */
    .long   TIM7_IRQHandler                   /* TIM7                         */
    .long   DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */
    .long   DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */
    .long   DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */
    .long   DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */
    .long   DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */
    .long   ETH_IRQHandler                    /* Ethernet                     */
    .long   ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line */
    .long   CAN2_TX_IRQHandler                /* CAN2 TX                      */
    .long   CAN2_RX0_IRQHandler               /* CAN2 RX0                     */
    .long   CAN2_RX1_IRQHandler               /* CAN2 RX1                     */
    .long   CAN2_SCE_IRQHandler               /* CAN2 SCE                     */
    .long   OTG_FS_IRQHandler                 /* USB OTG FS                   */
    .long   DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */
    .long   DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */
    .long   DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */
    .long   USART6_IRQHandler                 /* USART6                       */
    .long   I2C3_EV_IRQHandler                /* I2C3 event                   */
    .long   I2C3_ER_IRQHandler                /* I2C3 error                   */
    .long   OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */
    .long   OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */
    .long   OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */
    .long   OTG_HS_IRQHandler                 /* USB OTG HS                   */
    .long   DCMI_IRQHandler                   /* DCMI                         */
    .long   CRYP_IRQHandler                   /* CRYP crypto                  */
    .long   HASH_RNG_IRQHandler               /* Hash and Rng                 */
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

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
*
*******************************************************************************/
    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
#if (INCLUDE__UBINOS__UBIK == 1)
	b       bsp_ubik_irq_handler
#else
    b       .
#endif /* (INCLUDE__UBINOS__UBIK == 1) */
    .size   Default_Handler, . - Default_Handler

   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler

   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler

   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler

   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler

   .weak      WWDG_IRQHandler
   .thumb_set WWDG_IRQHandler,Default_Handler

   .weak      PVD_IRQHandler
   .thumb_set PVD_IRQHandler,Default_Handler

   .weak      TAMP_STAMP_IRQHandler
   .thumb_set TAMP_STAMP_IRQHandler,Default_Handler

   .weak      RTC_WKUP_IRQHandler
   .thumb_set RTC_WKUP_IRQHandler,Default_Handler

   .weak      FLASH_IRQHandler
   .thumb_set FLASH_IRQHandler,Default_Handler

   .weak      RCC_IRQHandler
   .thumb_set RCC_IRQHandler,Default_Handler

   .weak      EXTI0_IRQHandler
   .thumb_set EXTI0_IRQHandler,Default_Handler

   .weak      EXTI1_IRQHandler
   .thumb_set EXTI1_IRQHandler,Default_Handler

   .weak      EXTI2_IRQHandler
   .thumb_set EXTI2_IRQHandler,Default_Handler 

   .weak      EXTI3_IRQHandler
   .thumb_set EXTI3_IRQHandler,Default_Handler

   .weak      EXTI4_IRQHandler
   .thumb_set EXTI4_IRQHandler,Default_Handler

   .weak      DMA1_Stream0_IRQHandler
   .thumb_set DMA1_Stream0_IRQHandler,Default_Handler

   .weak      DMA1_Stream1_IRQHandler
   .thumb_set DMA1_Stream1_IRQHandler,Default_Handler

   .weak      DMA1_Stream2_IRQHandler
   .thumb_set DMA1_Stream2_IRQHandler,Default_Handler

   .weak      DMA1_Stream3_IRQHandler
   .thumb_set DMA1_Stream3_IRQHandler,Default_Handler 

   .weak      DMA1_Stream4_IRQHandler
   .thumb_set DMA1_Stream4_IRQHandler,Default_Handler

   .weak      DMA1_Stream5_IRQHandler
   .thumb_set DMA1_Stream5_IRQHandler,Default_Handler

   .weak      DMA1_Stream6_IRQHandler
   .thumb_set DMA1_Stream6_IRQHandler,Default_Handler

   .weak      ADC_IRQHandler
   .thumb_set ADC_IRQHandler,Default_Handler

   .weak      CAN1_TX_IRQHandler
   .thumb_set CAN1_TX_IRQHandler,Default_Handler

   .weak      CAN1_RX0_IRQHandler
   .thumb_set CAN1_RX0_IRQHandler,Default_Handler

   .weak      CAN1_RX1_IRQHandler
   .thumb_set CAN1_RX1_IRQHandler,Default_Handler

   .weak      CAN1_SCE_IRQHandler
   .thumb_set CAN1_SCE_IRQHandler,Default_Handler

   .weak      EXTI9_5_IRQHandler
   .thumb_set EXTI9_5_IRQHandler,Default_Handler

   .weak      TIM1_BRK_TIM9_IRQHandler
   .thumb_set TIM1_BRK_TIM9_IRQHandler,Default_Handler

   .weak      TIM1_UP_TIM10_IRQHandler
   .thumb_set TIM1_UP_TIM10_IRQHandler,Default_Handler

   .weak      TIM1_TRG_COM_TIM11_IRQHandler
   .thumb_set TIM1_TRG_COM_TIM11_IRQHandler,Default_Handler

   .weak      TIM1_CC_IRQHandler
   .thumb_set TIM1_CC_IRQHandler,Default_Handler

   .weak      TIM2_IRQHandler
   .thumb_set TIM2_IRQHandler,Default_Handler

   .weak      TIM3_IRQHandler
   .thumb_set TIM3_IRQHandler,Default_Handler

   .weak      TIM4_IRQHandler
   .thumb_set TIM4_IRQHandler,Default_Handler

   .weak      I2C1_EV_IRQHandler
   .thumb_set I2C1_EV_IRQHandler,Default_Handler

   .weak      I2C1_ER_IRQHandler
   .thumb_set I2C1_ER_IRQHandler,Default_Handler

   .weak      I2C2_EV_IRQHandler
   .thumb_set I2C2_EV_IRQHandler,Default_Handler

   .weak      I2C2_ER_IRQHandler
   .thumb_set I2C2_ER_IRQHandler,Default_Handler

   .weak      SPI1_IRQHandler
   .thumb_set SPI1_IRQHandler,Default_Handler

   .weak      SPI2_IRQHandler
   .thumb_set SPI2_IRQHandler,Default_Handler

   .weak      USART1_IRQHandler
   .thumb_set USART1_IRQHandler,Default_Handler

   .weak      USART2_IRQHandler
   .thumb_set USART2_IRQHandler,Default_Handler

   .weak      USART3_IRQHandler
   .thumb_set USART3_IRQHandler,Default_Handler

   .weak      EXTI15_10_IRQHandler
   .thumb_set EXTI15_10_IRQHandler,Default_Handler

   .weak      RTC_Alarm_IRQHandler
   .thumb_set RTC_Alarm_IRQHandler,Default_Handler

   .weak      OTG_FS_WKUP_IRQHandler
   .thumb_set OTG_FS_WKUP_IRQHandler,Default_Handler

   .weak      TIM8_BRK_TIM12_IRQHandler
   .thumb_set TIM8_BRK_TIM12_IRQHandler,Default_Handler

   .weak      TIM8_UP_TIM13_IRQHandler
   .thumb_set TIM8_UP_TIM13_IRQHandler,Default_Handler

   .weak      TIM8_TRG_COM_TIM14_IRQHandler
   .thumb_set TIM8_TRG_COM_TIM14_IRQHandler,Default_Handler

   .weak      TIM8_CC_IRQHandler
   .thumb_set TIM8_CC_IRQHandler,Default_Handler

   .weak      DMA1_Stream7_IRQHandler
   .thumb_set DMA1_Stream7_IRQHandler,Default_Handler

   .weak      FSMC_IRQHandler
   .thumb_set FSMC_IRQHandler,Default_Handler

   .weak      SDIO_IRQHandler
   .thumb_set SDIO_IRQHandler,Default_Handler

   .weak      TIM5_IRQHandler
   .thumb_set TIM5_IRQHandler,Default_Handler

   .weak      SPI3_IRQHandler
   .thumb_set SPI3_IRQHandler,Default_Handler

   .weak      UART4_IRQHandler
   .thumb_set UART4_IRQHandler,Default_Handler

   .weak      UART5_IRQHandler
   .thumb_set UART5_IRQHandler,Default_Handler

   .weak      TIM6_DAC_IRQHandler
   .thumb_set TIM6_DAC_IRQHandler,Default_Handler

   .weak      TIM7_IRQHandler
   .thumb_set TIM7_IRQHandler,Default_Handler

   .weak      DMA2_Stream0_IRQHandler
   .thumb_set DMA2_Stream0_IRQHandler,Default_Handler

   .weak      DMA2_Stream1_IRQHandler
   .thumb_set DMA2_Stream1_IRQHandler,Default_Handler

   .weak      DMA2_Stream2_IRQHandler
   .thumb_set DMA2_Stream2_IRQHandler,Default_Handler

   .weak      DMA2_Stream3_IRQHandler
   .thumb_set DMA2_Stream3_IRQHandler,Default_Handler

   .weak      DMA2_Stream4_IRQHandler
   .thumb_set DMA2_Stream4_IRQHandler,Default_Handler

   .weak      ETH_IRQHandler
   .thumb_set ETH_IRQHandler,Default_Handler

   .weak      ETH_WKUP_IRQHandler
   .thumb_set ETH_WKUP_IRQHandler,Default_Handler

   .weak      CAN2_TX_IRQHandler
   .thumb_set CAN2_TX_IRQHandler,Default_Handler

   .weak      CAN2_RX0_IRQHandler
   .thumb_set CAN2_RX0_IRQHandler,Default_Handler

   .weak      CAN2_RX1_IRQHandler
   .thumb_set CAN2_RX1_IRQHandler,Default_Handler

   .weak      CAN2_SCE_IRQHandler
   .thumb_set CAN2_SCE_IRQHandler,Default_Handler

   .weak      OTG_FS_IRQHandler
   .thumb_set OTG_FS_IRQHandler,Default_Handler

   .weak      DMA2_Stream5_IRQHandler
   .thumb_set DMA2_Stream5_IRQHandler,Default_Handler

   .weak      DMA2_Stream6_IRQHandler
   .thumb_set DMA2_Stream6_IRQHandler,Default_Handler

   .weak      DMA2_Stream7_IRQHandler
   .thumb_set DMA2_Stream7_IRQHandler,Default_Handler

   .weak      USART6_IRQHandler
   .thumb_set USART6_IRQHandler,Default_Handler

   .weak      I2C3_EV_IRQHandler
   .thumb_set I2C3_EV_IRQHandler,Default_Handler

   .weak      I2C3_ER_IRQHandler
   .thumb_set I2C3_ER_IRQHandler,Default_Handler

   .weak      OTG_HS_EP1_OUT_IRQHandler
   .thumb_set OTG_HS_EP1_OUT_IRQHandler,Default_Handler

   .weak      OTG_HS_EP1_IN_IRQHandler
   .thumb_set OTG_HS_EP1_IN_IRQHandler,Default_Handler

   .weak      OTG_HS_WKUP_IRQHandler
   .thumb_set OTG_HS_WKUP_IRQHandler,Default_Handler

   .weak      OTG_HS_IRQHandler
   .thumb_set OTG_HS_IRQHandler,Default_Handler

   .weak      DCMI_IRQHandler
   .thumb_set DCMI_IRQHandler,Default_Handler

   .weak      CRYP_IRQHandler
   .thumb_set CRYP_IRQHandler,Default_Handler

   .weak      HASH_RNG_IRQHandler
   .thumb_set HASH_RNG_IRQHandler,Default_Handler

#endif /* !(UBINOS__BSP__EXCLUDE_ARCH_INIT == 1) */
#endif /* (UBINOS__BSP__STM32_STM32F2X7 == 1) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
