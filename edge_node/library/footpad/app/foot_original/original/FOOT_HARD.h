/*
 * foot_hard.h
 *
 *  Created on: 2020. 6. 3.
 *      Author: Heo
 */

#ifndef APP_FOOT_HARD_H_
#define APP_FOOT_HARD_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stm32f2xx_hal.h>

/* To be defined only if the board is provided with the related shield */
/* https://www.adafruit.com/products/802 */
#ifndef ADAFRUIT_TFT_JOY_SD_ID802
#define ADAFRUIT_TFT_JOY_SD_ID802
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F2XX_NUCLEO_144
  * @{
  */

/** @addtogroup STM32F2XX_NUCLEO_144_LOW_LEVEL
  * @{
  */

/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_Exported_Types STM32F2XX NUCLEO 144 LOW LEVEL Exported Types
  * @{
  */
typedef enum
{
  LED1 = 0,
  LED_GREEN = LED1,
  LED2 = 1,
  LED_BLUE = LED2,
  LED3 = 2,
  LED_RED = LED3
}Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY = BUTTON_USER
}Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

typedef enum
{
  JOY_NONE  = 0,
  JOY_SEL   = 1,
  JOY_DOWN  = 2,
  JOY_LEFT  = 3,
  JOY_RIGHT = 4,
  JOY_UP    = 5
}JOYState_TypeDef;

/**
  * @}
  */

/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_Exported_Constants STM32F2XX NUCLEO 144 LOW LEVEL Exported Constants
  * @{
  */

/**
  * @brief Define for STM32F2XX_NUCLEO_144 board
  */
#if !defined (USE_STM32F2XX_NUCLEO_144)
 #define USE_STM32F2XX_NUCLEO_144
#endif
// foot array
short Foot_Array[48][96];
/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_LED STM32F2XX NUCLEO 144 LOW LEVEL LED
  * @{
  */
#define LEDn                                    3

#define LED1_PIN                                GPIO_PIN_13
#define LED1_GPIO_PORT                          GPIOB
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED2_PIN                                GPIO_PIN_14
#define LED2_GPIO_PORT                          GPIOB
#define LED2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED3_PIN                                GPIO_PIN_15
#define LED3_GPIO_PORT                          GPIOB
#define LED3_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) {__HAL_RCC_GPIOB_CLK_ENABLE();} else\
                                                                    {__HAL_RCC_GPIOB_CLK_ENABLE();   }} while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  do { if((__INDEX__) == 0) {__HAL_RCC_GPIOB_CLK_DISABLE();} else\
                                                                    {__HAL_RCC_GPIOB_CLK_DISABLE();   }} while(0)
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

//DMA
#define DMAx_CLOCK_ENABLE()               			__HAL_RCC_DMA2_CLK_ENABLE()
/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                  DMA_CHANNEL_0
#define ADCx_DMA_STREAM                   DMA2_Stream0

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                     DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler               DMA2_Stream0_IRQHandler


//Left ADC
#define ADCx_Left_INSTANCE                     ADC1
#define ADCx_Left_CLOCK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE();
#define ADCx_Left_CLOCK_DISABLE()               __HAL_RCC_ADC1_CLK_DISABLE();
#define ADCx_Left_CHANNEL_GPIO_CLOCK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define ADCx_Left_FORCE_RESET()                __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_Left_RELEASE_RESET()              __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_Left_CHANNEL_PIN                  GPIO_PIN_1
#define ADCx_Left_CHANNEL_GPIO_PORT            GPIOA

/* Definition for ADCx's Channel */
#define ADCx_Left_CHANNEL                      ADC_CHANNEL_1

//Right ADC
#define ADCx_Right_INSTANCE                     ADC1
#define ADCx_Right_CLOCK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE();
#define ADCx_Right_CHANNEL_GPIO_CLOCK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define ADCx_Right_FORCE_RESET()                __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_Right_RELEASE_RESET()              __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_Right_CHANNEL_PIN                  GPIO_PIN_2
#define ADCx_Right_CHANNEL_GPIO_PORT            GPIOA

/* Definition for ADCx's Channel */
#define ADCx_Right_CHANNEL                      ADC_CHANNEL_2

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



/* Definition for WIFI Module*/
//wifi enable pin
#define WIFI_ENABLE_PIN								GPIO_PIN_12
#define WIFI_ENABLE_PORT							GPIOC
#define WIFI_ENABLE_ENABLE()						__HAL_RCC_GPIOC_CLK_ENABLE()
#define WIFI_ENABLE_DISABLE()						__HAL_RCC_GPIOC_CLK_DISABLE()
//wifi reset pin
#define WIFI_RESET_PIN								GPIO_PIN_0
#define WIFI_RESET_PORT								GPIOD
#define WIFI_RESET_ENABLE()							__HAL_RCC_GPIOD_CLK_ENABLE()
#define WIFI_RESET_DISABLE()						__HAL_RCC_GPIOD_CLK_DISABLE()
/////uart
#define WIFI_USARTx                           USART3
#define WIFI_USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define WIFI_USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define WIFI_USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define WIFI_USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define WIFI_USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define WIFI_USARTx_TX_PIN                    GPIO_PIN_11
#define WIFI_USARTx_TX_GPIO_PORT              GPIOC
#define WIFI_USARTx_TX_AF                     GPIO_AF7_USART3
#define WIFI_USARTx_RX_PIN                    GPIO_PIN_10
#define WIFI_USARTx_RX_GPIO_PORT              GPIOC
#define WIFI_USARTx_RX_AF                     GPIO_AF7_USART3






/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_BUTTON STM32F2XX NUCLEO 144 LOW LEVEL BUTTON
  * @{
  */
#define BUTTONn                                 1

/**
 * @brief Key push-button
 */
#define USER_BUTTON_PIN                          GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                    GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()
#define USER_BUTTON_EXTI_LINE                    GPIO_PIN_13
#define USER_BUTTON_EXTI_IRQn                    EXTI15_10_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)      USER_BUTTON_GPIO_CLK_ENABLE()
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)     USER_BUTTON_GPIO_CLK_DISABLE()

/* Aliases */
#define KEY_BUTTON_PIN                       USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT                 USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()         USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()        USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_LINE                 USER_BUTTON_EXTI_LINE
#define KEY_BUTTON_EXTI_IRQn                 USER_BUTTON_EXTI_IRQn


/**
  * @brief OTG_FS1 OVER_CURRENT and POWER_SWITCH Pins definition
  */


#define OTG_FS1_OVER_CURRENT_PIN                  GPIO_PIN_7
#define OTG_FS1_OVER_CURRENT_PORT                 GPIOG
#define OTG_FS1_OVER_CURRENT_PORT_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()

#define OTG_FS1_POWER_SWITCH_PIN                  GPIO_PIN_6
#define OTG_FS1_POWER_SWITCH_PORT                 GPIOG
#define OTG_FS1_POWER_SWITCH_PORT_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()

/**
  * @}
  */

/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_BUS STM32F2XX NUCLEO 144 LOW LEVEL BUS
  * @{
  */
/*############################### SPI_A #######################################*/
#ifdef HAL_SPI_MODULE_ENABLED

#define NUCLEO_SPIx                                     SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                        __HAL_RCC_SPI1_CLK_ENABLE()

#define NUCLEO_SPIx_SCK_AF                              GPIO_AF5_SPI1
#define NUCLEO_SPIx_SCK_GPIO_PORT                       GPIOA
#define NUCLEO_SPIx_SCK_PIN                             GPIO_PIN_5
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE()

#define NUCLEO_SPIx_MISO_MOSI_AF                        GPIO_AF5_SPI1
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT                 GPIOA
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                            GPIO_PIN_6
#define NUCLEO_SPIx_MOSI_PIN                            GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeout are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000

#define NUCLEO_SPIx_CS_GPIO_PORT                        GPIOD
#define NUCLEO_SPIx_CS_PIN                              GPIO_PIN_14
#define NUCLEO_SPIx_CS_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOD_CLK_ENABLE()
#define NUCLEO_SPIx_CS_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOD_CLK_DISABLE()

#define SPIx__CS_LOW()       HAL_GPIO_WritePin(NUCLEO_SPIx_CS_GPIO_PORT, NUCLEO_SPIx_CS_PIN, GPIO_PIN_RESET)
#define SPIx__CS_HIGH()      HAL_GPIO_WritePin(NUCLEO_SPIx_CS_GPIO_PORT, NUCLEO_SPIx_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Control Lines management
  */
#define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()      HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()     HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Interface pins (shield D4)
  */
#define SD_CS_PIN                                 GPIO_PIN_14
#define SD_CS_GPIO_PORT                           GPIOF
#define SD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOF_CLK_DISABLE()

/**
  * @brief  LCD Control Interface pins (shield D10)
  */
#define LCD_CS_PIN                                 GPIO_PIN_14
#define LCD_CS_GPIO_PORT                           GPIOD
#define LCD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOD_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOD_CLK_DISABLE()

/**
  * @brief  LCD Data/Command Interface pins (shield D8)
  */
#define LCD_DC_PIN                                 GPIO_PIN_12
#define LCD_DC_GPIO_PORT                           GPIOF
#define LCD_DC_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOF_CLK_DISABLE()

#endif /* HAL_SPI_MODULE_ENABLED */

/*################################ ADCx for Nucleo 144 board ######################################*/
/**
  * @brief  ADCx Interface pins
  *         used to detect motion of Joystick available on Adafruit 1.8" TFT shield
  */

#ifdef HAL_ADC_MODULE_ENABLED

#define NUCLEO_ADCx                        ADC3
#define NUCLEO_ADCx_CLK_ENABLE()         __HAL_RCC_ADC3_CLK_ENABLE()
#define NUCLEO_ADCx_CLK_DISABLE()        __HAL_RCC_ADC3_CLK_DISABLE()

#define NUCLEO_ADCx_CHANNEL                ADC_CHANNEL_9

#define NUCLEO_ADCx_GPIO_PORT              GPIOF
#define NUCLEO_ADCx_GPIO_PIN               GPIO_PIN_3
#define NUCLEO_ADCx_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define NUCLEO_ADCx_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()

#endif /* HAL_ADC_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_Exported_Macros STM32F2XX NUCLEO 144 LOW LEVEL Exported Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F2XX_NUCLEO_144_LOW_LEVEL_Exported_Functions STM32F2XX NUCLEO 144 LOW LEVEL Exported Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void             BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);
#ifdef HAL_ADC_MODULE_ENABLED
uint8_t          BSP_JOY_Init(void);
JOYState_TypeDef BSP_JOY_GetState(void);
void             BSP_JOY_DeInit(void);
#endif /* HAL_ADC_MODULE_ENABLED */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif




#endif /* APP_FOOT_HARD_H_ */
