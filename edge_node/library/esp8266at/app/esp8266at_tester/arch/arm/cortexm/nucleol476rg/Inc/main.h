/**
  ******************************************************************************
  * @file    UART/UART_HyperTerminal_IT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#include <esp8266at.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#if (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__EXTERNAL)
#if (STM32CUBEL4__DTTY_STM32_UART_ENABLE == 1)

/* Definition for DTTY_STM32_UART */

#if (UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER == 3)

#define DTTY_STM32_UART                         USART3
#define DTTY_STM32_UART_HANDLE                  huart3

#define DTTY_STM32_UART_CLK_ENABLE()            __HAL_RCC_USART3_CLK_ENABLE()
#define DTTY_STM32_UART_RX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define DTTY_STM32_UART_TX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

#define DTTY_STM32_UART_FORCE_RESET()           __HAL_RCC_USART3_FORCE_RESET()
#define DTTY_STM32_UART_RELEASE_RESET()         __HAL_RCC_USART3_RELEASE_RESET()

#define DTTY_STM32_UART_TX_Pin                  GPIO_PIN_4
#define DTTY_STM32_UART_TX_GPIO_Port            GPIOC
#define DTTY_STM32_UART_TX_AF                   GPIO_AF7_USART3
#define DTTY_STM32_UART_RX_Pin                  GPIO_PIN_5
#define DTTY_STM32_UART_RX_GPIO_Port            GPIOC
#define DTTY_STM32_UART_RX_AF                   GPIO_AF7_USART3

#define DTTY_STM32_UART_IRQn                    USART3_IRQn
#define DTTY_STM32_UART_IRQHandler              USART3_IRQHandler

#else

#error "Unsupported UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER"

#endif

extern UART_HandleTypeDef DTTY_STM32_UART_HANDLE;

void dtty_stm32_uart_rx_callback(void);
void dtty_stm32_uart_tx_callback(void);
void dtty_stm32_uart_err_callback(void);

#endif /* (STM32CUBEL4__DTTY_STM32_UART_ENABLE == 1) */
#endif /* (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__EXTERNAL) */

/* Definition for ESP8266_UART (mikroBUS 1) */

#define ESP8266_UART                            USART2
#define ESP8266_UART_HANDLE                     huart2

#define ESP8266_UART_CLK_ENABLE()               __HAL_RCC_USART2_CLK_ENABLE()
#define ESP8266_UART_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ESP8266_UART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define ESP8266_UART_FORCE_RESET()              __HAL_RCC_USART2_FORCE_RESET()
#define ESP8266_UART_RELEASE_RESET()            __HAL_RCC_USART2_RELEASE_RESET()

#define ESP8266_UART_TX_Pin                     GPIO_PIN_2
#define ESP8266_UART_TX_GPIO_Port               GPIOA
#define ESP8266_UART_TX_AF                      GPIO_AF7_USART2
#define ESP8266_UART_RX_Pin                     GPIO_PIN_3
#define ESP8266_UART_RX_GPIO_Port               GPIOA
#define ESP8266_UART_RX_AF                      GPIO_AF7_USART2

#define ESP8266_UART_IRQn                       USART2_IRQn
#define ESP8266_UART_IRQHandler                 USART2_IRQHandler

extern UART_HandleTypeDef ESP8266_UART_HANDLE;

void esp8266_uart_rx_callback(void);
void esp8266_uart_tx_callback(void);
void esp8266_uart_err_callback(void);

#if (ESP8266AT__USE_RESET_PIN == 1)
/* Definition for ESP8266_NRST */
    #define ESP8266_NRST_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
    #define ESP8266_NRST_Pin                        GPIO_PIN_0
    #define ESP8266_NRST_GPIO_Port                  GPIOB
#endif /* (ESP8266AT__USE_RESET_PIN == 1) */

#if (ESP8266AT__USE_CHIPSELECT_PIN == 1)
/* Definition for ESP8266_CS */
    #define ESP8266_CS_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
    #define ESP8266_CS_Pin                          GPIO_PIN_6
    #define ESP8266_CS_GPIO_Port                    GPIOB
#endif /* (ESP8266AT__USE_CHIPSELECT_PIN == 1) */

/* Definition for esp8266at */

extern esp8266at_t _g_esp8266at;

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
