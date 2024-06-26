/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOF207ZG)
#if (UBINOS__BSP__BOARD_VARIATION__NUCLEOF207ZG == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS] =
{
//   port, no, support_pwm, alternate, tim_instance, tim_channel, tim_clock, enable
    {GPIOG, GPIO_PIN_9 , 0, 0            , 0   , 0            , 0            , 1}, // D0
    {GPIOG, GPIO_PIN_14, 0, 0            , 0   , 0            , 0            , 1}, // D1
    {GPIOF, GPIO_PIN_15, 0, 0            , 0   , 0            , 0            , 1}, // D2
    {GPIOE, GPIO_PIN_13, 1, GPIO_AF1_TIM1, TIM1, TIM_CHANNEL_3, 120000000    , 1}, // D3  ,SystemCoreClock    , o
    {GPIOF, GPIO_PIN_14, 0, 0            , 0   , 0            , 0            , 1}, // D4
    {GPIOE, GPIO_PIN_11, 1, GPIO_AF1_TIM1, TIM1, TIM_CHANNEL_2, 120000000    , 1}, // D5  ,SystemCoreClock    , o
    {GPIOE, GPIO_PIN_9 , 1, GPIO_AF1_TIM1, TIM1, TIM_CHANNEL_1, 120000000    , 1}, // D6  ,SystemCoreClock    , o
    {GPIOF, GPIO_PIN_13, 0, 0            , 0   , 0            , 0            , 1}, // D7
    {GPIOF, GPIO_PIN_12, 0, 0            , 0   , 0            , 0            , 1}, // D8
    {GPIOD, GPIO_PIN_15, 1, GPIO_AF2_TIM4, TIM4, TIM_CHANNEL_4, 120000000 / 2, 1}, // D9  ,SystemCoreClock / 2, o
    {GPIOD, GPIO_PIN_14, 1, GPIO_AF2_TIM4, TIM4, TIM_CHANNEL_3, 120000000 / 2, 1}, // D10 ,SystemCoreClock / 2, o
    {GPIOA, GPIO_PIN_7 , 0, GPIO_AF2_TIM3, TIM3, TIM_CHANNEL_2, 120000000 / 2, 1}, // D11 ,SystemCoreClock / 2
    {GPIOA, GPIO_PIN_6 , 0, GPIO_AF2_TIM3, TIM3, TIM_CHANNEL_1, 120000000 / 2, 1}, // D12 ,SystemCoreClock / 2
    {GPIOA, GPIO_PIN_5 , 0, 0            , 0   , 0            , 0            , 1}, // D13
    {GPIOB, GPIO_PIN_9 , 0, GPIO_AF2_TIM4, TIM4, TIM_CHANNEL_4, 120000000 / 2, 1}, // D14 ,SystemCoreClock / 2
    {GPIOB, GPIO_PIN_8 , 0, GPIO_AF2_TIM4, TIM4, TIM_CHANNEL_3, 120000000 / 2, 1}, // D15 ,SystemCoreClock / 2
};

arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS] =
{
//  port, no, adc_instance, adc_channel, enable
    {GPIOA, GPIO_PIN_3 , ADC3, ADC_CHANNEL_3 , 1}, // A0
    {GPIOC, GPIO_PIN_0 , ADC3, ADC_CHANNEL_10, 1}, // A1
    {GPIOC, GPIO_PIN_3 , ADC3, ADC_CHANNEL_13, 1}, // A2
    {GPIOF, GPIO_PIN_3 , ADC3, ADC_CHANNEL_9 , 1}, // A3
    {GPIOF, GPIO_PIN_5 , ADC3, ADC_CHANNEL_15, 1}, // A4
    {GPIOF, GPIO_PIN_10, ADC3, ADC_CHANNEL_8 , 1}, // A5
};

arduino_d_pin_tim_t _g_d_pin_tims[NUM_DIGITAL_PINS];

arduino_tone_t _arduino_tone;

void initVariant(void)
{
    GPIO_TypeDef * GPIO_Port;
	GPIO_InitTypeDef GPIO_InitStruct;
    arduino_a_pin_t const * a_pin;

    ////////////////////////////////////////
    // for digital io
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    for (int i = 0; i < NUM_DIGITAL_PINS; i++)
    {
        _g_d_pin_tims[i].tim_handle.Instance = NULL;
    }

    ////////////////////////////////////////
    // for analog in
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	__HAL_RCC_ADC3_CLK_ENABLE();

    for (int i = 0; i < NUM_ANALOG_INPUTS; i++)
    {
        a_pin = &_g_a_pin_map[i];

        if (a_pin->enable)
        {
            GPIO_Port = a_pin->port;
            GPIO_InitStruct.Pin = a_pin->no;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);
        }
    }

    ////////////////////////////////////////
    // for analog, servo, tone out (pwm)
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    ////////////////////////////////////////
    // for tone
    _arduino_tone.timer_initiated = 0;
    _arduino_tone.pin_initiated = 0;
}

#endif /* (UBINOS__BSP__BOARD_VARIATION__NUCLEOF207ZG == 1) */
#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOF207ZG) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

