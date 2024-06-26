/**
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_MultiChannelSingleConversion/Inc/main.h
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
#include "stm32f2xx_ll_bus.h"
#include "stm32f2xx_ll_rcc.h"
#include "stm32f2xx_ll_system.h"
#include "stm32f2xx_ll_utils.h"
#include "stm32f2xx_ll_cortex.h"
#include "stm32f2xx_ll_gpio.h"
#include "stm32f2xx_ll_exti.h"
#include "stm32f2xx_ll_adc.h"
#include "stm32f2xx_ll_dma.h"
#include "stm32f2xx_ll_pwr.h"
 #include "stm32f2xx_hal_uart.h"
 #include "stm32f2xx_hal_usart.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */
#include <esp8266at.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Define used to enable time-out management*/
#define USE_TIMEOUT       0

/**
  * @brief LED1
  */
#define LED1_PIN                           LL_GPIO_PIN_13
#define LED1_GPIO_PORT                     GPIOB
#define LED2_PIN                           LL_GPIO_PIN_14
#define LED2_GPIO_PORT                     GPIOB
#define LED3_PIN                           LL_GPIO_PIN_15
#define LED3_GPIO_PORT                     GPIOB
#define LED1_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

void LED_Init(void);
void LED_On(char);
void LED_Off(char);
void LED_Blinking(uint32_t Period);

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         LL_GPIO_PIN_12
#define USER_BUTTON_GPIO_PORT                   GPIOB
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_12
#define USER_BUTTON_EXTI_IRQn                   EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                     \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);                  \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE12);  \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI15_10_IRQHandler

#define INTERNAL_TEMPSENSOR_AVGSLOPE   ((int32_t) 2500)        /* Internal temperature sensor, parameter Avg_Slope (unit: uV/DegCelsius). Refer to device datasheet for min/typ/max values. */
#define INTERNAL_TEMPSENSOR_V25        ((int32_t)  760)        /* Internal temperature sensor, parameter V25 (unit: mV). Refer to device datasheet for min/typ/max values. */
#define INTERNAL_TEMPSENSOR_V25_TEMP   ((int32_t)   25)
#define INTERNAL_TEMPSENSOR_V25_VREF   ((int32_t) 3300)

extern char foot_array[48][96];
extern char temp_left_image[48][96];
extern char temp_right_image[48][96];
extern char left_image[48][48];
extern char right_image[48][48];

#if (FOOT_ORIGINAL__USE_USART == 1)
extern uint8_t uart_image[48*48];
extern uint8_t uart_error[1];
#define RXBUFFERSIZE                      1
#define IMAGE_SIZE                   (COUNTOF(uart_image))
#define ERROR_SIZE                   (COUNTOF(uart_error))
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
extern UART_HandleTypeDef UartHandle;

#endif /* (FOOT_ORIGINAL__USE_USART == 1) */

/**defgroup STM32F207ZG Power Enable Pin
  * @}
  */
#define Module_EN									4
#define WIFI_PW_EN_PIN								GPIO_PIN_1
#define WIFI_PW_EN_PORT								GPIOD
#define WIFI_PW_EN_CLK_ENABLE()						__HAL_RCC_GPIOD_CLK_ENABLE()
#define WIFI_PW_EN_CKL_DISALE()						__HAL_RCC_GPIOD_CLK_DISABLE()

#define Logic_PW_EN_PIN								GPIO_PIN_2
#define Logic_PW_EN_PORT							GPIOD
#define Logic_PW_EN_CLK_ENABLE()					__HAL_RCC_GPIOD_CLK_ENABLE()
#define Logic_PW_EN_CKL_DISALE()					__HAL_RCC_GPIOD_CLK_DISABLE()

#define RF_T_EN_PIN									GPIO_PIN_3
#define RF_T_EN_PORT								GPIOD
#define RF_T_EN_CLK_ENABLE()						__HAL_RCC_GPIOD_CLK_ENABLE()
#define RF_T_EN_CKL_DISALE()						__HAL_RCC_GPIOD_CLK_DISABLE()

#define RF_R_EN_PIN									GPIO_PIN_4
#define RF_R_EN_PORT								GPIOD
#define RF_R_EN_CLK_ENABLE()						__HAL_RCC_GPIOD_CLK_ENABLE()
#define RF_R_EN_CKL_DISALE()						__HAL_RCC_GPIOD_CLK_DISABLE()


/**defgroup STM32F207ZG FOOT PAD Register
  * @}
  */
#define Shift_Registern								2
//Left shift register
#define Shift_Register_LEFT_DATA_PIN				GPIO_PIN_13
#define Shift_Register_LEFT_DATA_PORT				GPIOE
#define Shift_Register_LEFT_DATA_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define Shift_Register_LEFT_DATA_CKL_DISALE()		__HAL_RCC_GPIOE_CLK_DISABLE()

#define Shift_Register_LEFT_CLK_PIN					GPIO_PIN_14
#define Shift_Register_LEFT_CLK_PORT				GPIOE
#define Shift_Register_LEFT_CLK_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define Shift_Register_LEFT_CLK_CKL_DISALE()		__HAL_RCC_GPIOE_CLK_DISABLE()

#define Shift_Register_LEFT_CE_PIN					GPIO_PIN_15
#define Shift_Register_LEFT_CE_PORT					GPIOE
#define Shift_Register_LEFT_CE_CLK_ENABLE()			__HAL_RCC_GPIOE_CLK_ENABLE()
#define Shift_Register_LEFT_CE_CKL_DISALE()			__HAL_RCC_GPIOE_CLK_DISABLE()

//right shift register
#define Shift_Register_RIGHT_DATA_PIN				GPIO_PIN_9
#define Shift_Register_RIGHT_DATA_PORT				GPIOG
#define Shift_Register_RIGHT_DATA_CLK_ENABLE()		__HAL_RCC_GPIOG_CLK_ENABLE()
#define Shift_Register_RIGHT_DATA_CKL_DISALE()		__HAL_RCC_GPIOG_CLK_DISABLE()

#define Shift_Register_RIGHT_CLK_PIN				GPIO_PIN_10
#define Shift_Register_RIGHT_CLK_PORT				GPIOG
#define Shift_Register_RIGHT_CLK_CLK_ENABLE()		__HAL_RCC_GPIOG_CLK_ENABLE()
#define Shift_Register_RIGHT_CLK_CKL_DISALE()		__HAL_RCC_GPIOG_CLK_DISABLE()

#define Shift_Register_RIGHT_CE_PIN					GPIO_PIN_11
#define Shift_Register_RIGHT_CE_PORT				GPIOG
#define Shift_Register_RIGHT_CE_CLK_ENABLE()		__HAL_RCC_GPIOG_CLK_ENABLE()
#define Shift_Register_RIGHT_CE_CKL_DISALE()		__HAL_RCC_GPIOG_CLK_DISABLE()


/**defgroup STM32F207ZG ANALOGUE MUX
  * @}
  */
#define Muxn										2
//Left Mux
#define Mux_Left_S0_PIN								GPIO_PIN_6
#define Mux_Left_S0_PORT							GPIOE
#define Mux_Left_S0_ENABLE()						__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_S0_DISABLE()						__HAL_RCC_GPIOE_CLK_DISABLE()

#define Mux_Left_S1_PIN								GPIO_PIN_7
#define Mux_Left_S1_PORT							GPIOE
#define Mux_Left_S1_ENABLE()						__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_S1_DISABLE()						__HAL_RCC_GPIOE_CLK_DISABLE()

#define Mux_Left_S2_PIN								GPIO_PIN_8
#define Mux_Left_S2_PORT							GPIOE
#define Mux_Left_S2_ENABLE()						__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_S2_DISABLE()						__HAL_RCC_GPIOE_CLK_DISABLE()

#define Mux_Left_S3_PIN								GPIO_PIN_9
#define Mux_Left_S3_PORT							GPIOE
#define Mux_Left_S3_ENABLE()						__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_S3_DISABLE()						__HAL_RCC_GPIOE_CLK_DISABLE()

#define Mux_Left_Mux_1_EN_PIN						GPIO_PIN_10
#define Mux_Left_Mux_1_EN_PORT						GPIOE
#define Mux_Left_Mux_1_EN_ENABLE()					__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_Mux_1_EN_DISABLE()					__HAL_RCC_GPIOE_CLK_DISABLE()

#define Mux_Left_Mux_2_EN_PIN						GPIO_PIN_11
#define Mux_Left_Mux_2_EN_PORT						GPIOE
#define Mux_Left_Mux_2_EN_ENABLE()					__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_Mux_2_EN_DISABLE()					__HAL_RCC_GPIOE_CLK_DISABLE()

#define Mux_Left_Mux_3_EN_PIN						GPIO_PIN_12
#define Mux_Left_Mux_3_EN_PORT						GPIOE
#define Mux_Left_Mux_3_EN_ENABLE()					__HAL_RCC_GPIOE_CLK_ENABLE()
#define Mux_Left_Mux_3_EN_DISABLE()					__HAL_RCC_GPIOE_CLK_DISABLE()

/////Right Mux
#define Mux_Right_S0_PIN							GPIO_PIN_2
#define Mux_Right_S0_PORT							GPIOG
#define Mux_Right_S0_ENABLE()						__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_S0_DISABLE()						__HAL_RCC_GPIOG_CLK_DISABLE()

#define Mux_Right_S1_PIN							GPIO_PIN_3
#define Mux_Right_S1_PORT							GPIOG
#define Mux_Right_S1_ENABLE()						__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_S1_DISABLE()						__HAL_RCC_GPIOG_CLK_DISABLE()

#define Mux_Right_S2_PIN							GPIO_PIN_4
#define Mux_Right_S2_PORT							GPIOG
#define Mux_Right_S2_ENABLE()						__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_S2_DISABLE()						__HAL_RCC_GPIOG_CLK_DISABLE()

#define Mux_Right_S3_PIN							GPIO_PIN_5
#define Mux_Right_S3_PORT							GPIOG
#define Mux_Right_S3_ENABLE()						__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_S3_DISABLE()						__HAL_RCC_GPIOG_CLK_DISABLE()

#define Mux_Right_Mux_1_EN_PIN						GPIO_PIN_6
#define Mux_Right_Mux_1_EN_PORT						GPIOG
#define Mux_Right_Mux_1_EN_ENABLE()					__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_Mux_1_EN_DISABLE()				__HAL_RCC_GPIOG_CLK_DISABLE()

#define Mux_Right_Mux_2_EN_PIN						GPIO_PIN_7
#define Mux_Right_Mux_2_EN_PORT						GPIOG
#define Mux_Right_Mux_2_EN_ENABLE()					__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_Mux_2_EN_DISABLE()				__HAL_RCC_GPIOG_CLK_DISABLE()

#define Mux_Right_Mux_3_EN_PIN						GPIO_PIN_8
#define Mux_Right_Mux_3_EN_PORT						GPIOG
#define Mux_Right_Mux_3_EN_ENABLE()					__HAL_RCC_GPIOG_CLK_ENABLE()
#define Mux_Right_Mux_3_EN_DISABLE()				__HAL_RCC_GPIOG_CLK_DISABLE()

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/* IRQ Handler treatment */
void UserButton_Callback(void);
void AdcDmaTransferComplete_Callback(void);
void AdcDmaTransferError_Callback(void);
void AdcGrpRegularSequenceConvComplete_Callback(void);
void AdcGrpRegularOverrunError_Callback(void);

/* Definition for ESP8266_UART */

#define ESP8266_UART                            USART3
#define ESP8266_UART_HANDLE                     huart3

#define ESP8266_UART_CLK_ENABLE()               __HAL_RCC_USART3_CLK_ENABLE()
#define ESP8266_UART_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define ESP8266_UART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()

#define ESP8266_UART_FORCE_RESET()              __HAL_RCC_USART3_FORCE_RESET()
#define ESP8266_UART_RELEASE_RESET()            __HAL_RCC_USART3_RELEASE_RESET()

#define ESP8266_UART_RX_Pin                     GPIO_PIN_11
#define ESP8266_UART_RX_GPIO_Port               GPIOC
#define ESP8266_UART_RX_AF                      GPIO_AF7_USART3
#define ESP8266_UART_TX_Pin                     GPIO_PIN_10
#define ESP8266_UART_TX_GPIO_Port               GPIOC
#define ESP8266_UART_TX_AF                      GPIO_AF7_USART3

#define ESP8266_UART_IRQn                       USART3_IRQn
#define ESP8266_UART_IRQHandler                 USART3_IRQHandler

extern UART_HandleTypeDef ESP8266_UART_HANDLE;

void esp8266_uart_rx_callback(void);
void esp8266_uart_tx_callback(void);
void esp8266_uart_err_callback(void);

/* Definition for ESP8266_NRST */

#define ESP8266_NRST_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()

#define ESP8266_NRST_Pin                        GPIO_PIN_0
#define ESP8266_NRST_GPIO_Port                  GPIOD

/* Definition for ESP8266_CS */

#define ESP8266_CS_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()

#define ESP8266_CS_Pin                          GPIO_PIN_12
#define ESP8266_CS_GPIO_Port                    GPIOC

/* Definition for esp8266at */

extern esp8266at_t _g_esp8266at;
extern esp8266at_err_t err;





#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
