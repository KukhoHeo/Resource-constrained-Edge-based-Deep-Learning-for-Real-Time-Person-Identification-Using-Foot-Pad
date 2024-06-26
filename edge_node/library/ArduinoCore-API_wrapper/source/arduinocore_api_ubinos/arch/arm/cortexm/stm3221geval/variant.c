/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL)
#if (UBINOS__BSP__BOARD_VARIATION__STM3221GEVAL == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS] =
{
    {GPIOG, GPIO_PIN_6 , 0, 0            , 0   , 0            , 0            , 1}, // D0
    {GPIOG, GPIO_PIN_8 , 0, 0            , 0   , 0            , 0            , 1}, // D1
};

arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS] =
{
};

arduino_d_pin_tim_t _g_d_pin_tims[NUM_DIGITAL_PINS];

arduino_tone_t _arduino_tone;

void initVariant(void)
{
    // for digital io
    __HAL_RCC_GPIOG_CLK_ENABLE();

    for (int i = 0; i < NUM_DIGITAL_PINS; i++)
    {
        _g_d_pin_tims[i].tim_handle.Instance = NULL;
    }

    // for analog in

    ////////////////////////////////////////
    // for analog, servo, tone out (pwm)
    __HAL_RCC_TIM3_CLK_ENABLE();

    ////////////////////////////////////////
    // for tone
    _arduino_tone.timer_initiated = 0;
    _arduino_tone.pin_initiated = 0;
}

#endif /* (UBINOS__BSP__BOARD_VARIATION__STM3221GEVAL == 1) */
#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

