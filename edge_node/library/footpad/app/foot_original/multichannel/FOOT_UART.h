/*
 * FOOT_UART.h
 *
 *  Created on: 2022. 3. 18
 *      Author: Heo
 */


#ifndef APP_FOOT_ORIGINAL_ARCH_ARM_ST_STM32F207ZG_MULTI_SINGGLE_FOOT_UART_H_
#define APP_FOOT_ORIGINAL_ARCH_ARM_ST_STM32F207ZG_MULTI_SINGGLE_FOOT_UART_H_
#include <ubinos.h>

/* Includes ------------------------------------------------------------------*/
#include "FOOT_HARD_ll.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void ESP8266_UART_IRQHandler(void);
void Foot_Uart_Init(void);
void UART_Error_Handler(void);

#endif /* APP_FOOT_ORIGINAL_ARCH_ARM_ST_STM32F207ZG_MULTI_SINGGLE_FOOT_UART_H_ */
