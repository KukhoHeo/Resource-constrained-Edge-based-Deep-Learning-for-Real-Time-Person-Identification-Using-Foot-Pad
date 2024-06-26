/**
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

#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"

#if !(UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL)
    #error "Unsupported UBINOS__BSP__BOARD_MODEL"
#endif

/* ADC handler declaration */
ADC_HandleTypeDef AdcHandle;

/* Variable used to get converted value */
__IO uint16_t uhADCxConvertedValue = 0;

static void Error_Handler(void);

static void root_func(void *arg);
static void task1_func(void *arg);
static void task2_func(void *arg);
static void task3_func(void *arg);

int appmain(int argc, char *argv[]) {
    int r;
    (void) r;

    srand(time(NULL));

    r = task_create(NULL, root_func, NULL, task_getmiddlepriority(), 0, "root");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}

static void root_func(void *arg) {
    int r;
    (void) r;

    ADC_ChannelConfTypeDef sConfig;

    printf("\n\n\r\n");
    printf("================================================================================\r\n");
    printf("adc_regularconversion_polling (build time: %s %s)\r\n", __TIME__, __DATE__);
    printf("================================================================================\r\n");
    printf("\r\n");

    /* Configure LED3 */
    BSP_LED_Init(LED3);

    /*##-1- Configure the ADC peripheral #######################################*/
    AdcHandle.Instance = ADCx_INSTANCE;

    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
    AdcHandle.Init.ScanConvMode = DISABLE;
    AdcHandle.Init.ContinuousConvMode = DISABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.NbrOfDiscConversion = 0;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion = 1;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.EOCSelection = DISABLE;

    if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
        /* Initialization Error */
        logme("fail at HAL_ADC_Init\r\n");
        Error_Handler();
    }

    /*##-2- Configure ADC regular channel ######################################*/
    sConfig.Channel = ADCx_CHANNEL;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        /* Channel Configuration Error */
        logme("fail at HAL_ADC_ConfigChannel\r\n");
        Error_Handler();
    }

    r = task_create(NULL, task1_func, NULL, task_getmiddlepriority(), 0, "task1");
    ubi_assert(r == 0);

    r = task_create(NULL, task2_func, NULL, task_getmiddlepriority(), 0, "task2");
    ubi_assert(r == 0);

    r = task_create(NULL, task3_func, NULL, task_getmiddlepriority(), 0, "task3");
    ubi_assert(r == 0);
}

static void task1_func(void *arg) {
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        delayms = (rand() % 10 + 1) * 200;
        printf("1: hello world ! (%u) (delay = %4d ms)\r\n", i, delayms);
        task_sleepms(delayms);
    }
}

static void task2_func(void *arg) {
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        delayms = (rand() % 10 + 1) * 200;
        printf("2: hello world ! (%u) (delay = %4d ms)\r\n", i, delayms);
        task_sleepms(delayms);
    }
}

static void task3_func(void *arg) {
    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        /*##-3- Start the conversion process #######################################*/
        if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
            /* Start Conversation Error */
            logme("fail at HAL_ADC_Start\r\n");
            Error_Handler();
        }

        /*##-4- Wait for the end of conversion #####################################*/
        /*  Before starting a new conversion, you need to check the current state of
         the peripheral; if it is busy you need to wait for the end of current
         conversion before starting a new one.
         For simplicity reasons, this example is just waiting till the end of the
         conversion, but application may perform other tasks while conversion
         operation is ongoing. */
        HAL_ADC_PollForConversion(&AdcHandle, 10);

        /* Check if the continuous conversion of regular channel is finished */
        if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG) {
            /*##-5- Get the converted value of regular channel #######################*/
            uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
        }

        printf("3: ADC value (%u) = %d\r\n", i, uhADCxConvertedValue);
        task_sleepms(1000);
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while (1) {
    }
}

/**
 * @brief ADC MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    GPIO_InitTypeDef GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* ADC3 Periph clock enable */
    ADCx_CLOCK_ENABLE();
    /* Enable GPIO clock ****************************************/
    ADCx_CHANNEL_GPIO_CLOCK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* ADC3 Channel8 GPIO pin configuration */
    GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief ADC MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO to their default state
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc) {

    /*##-1- Reset peripherals ##################################################*/
    ADCx_FORCE_RESET();
    ADCx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* De-initialize the ADC3 Channel8 GPIO pin */
    HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

