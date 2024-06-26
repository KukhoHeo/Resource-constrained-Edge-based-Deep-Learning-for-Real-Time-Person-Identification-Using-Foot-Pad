/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK)
#if (UBINOS__BSP__BOARD_VARIATION__ADAFRUITNRF52840E == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS] =
{
    {NRF_GPIO_PIN_MAP(1,  8), 0}, // D0
    {NRF_GPIO_PIN_MAP(1,  8), 0}, // D1
    {NRF_GPIO_PIN_MAP(1,  8), 0}, // D2
    {NRF_GPIO_PIN_MAP(1,  8), 0}, // D3
    {NRF_GPIO_PIN_MAP(1,  8), 0}, // D4
    {NRF_GPIO_PIN_MAP(1,  8), 1}, // D5
    {NRF_GPIO_PIN_MAP(0,  7), 1}, // D6
    {NRF_GPIO_PIN_MAP(0,  7), 0}, // D7
    {NRF_GPIO_PIN_MAP(0,  7), 0}, // D8
    {NRF_GPIO_PIN_MAP(0, 26), 1}, // D9
    {NRF_GPIO_PIN_MAP(0, 27), 1}, // D10
    {NRF_GPIO_PIN_MAP(0,  6), 1}, // D11
    {NRF_GPIO_PIN_MAP(0,  8), 1}, // D12
    {NRF_GPIO_PIN_MAP(1,  9), 1}, // D13
    {NRF_GPIO_PIN_MAP(1, 15), 1}, // LED_BUILTIN
    {NRF_GPIO_PIN_MAP(1, 10), 1}, // LED_BUILTIN_2
};

arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS] =
{
    {NRF_SAADC_INPUT_AIN2, 0, 1}, // A0 P0.04
    {NRF_SAADC_INPUT_AIN3, 1, 1}, // A1 P0.05
    {NRF_SAADC_INPUT_AIN6, 2, 1}, // A2 P0.30
    {NRF_SAADC_INPUT_AIN4, 3, 1}, // A3 P0.28
    {NRF_SAADC_INPUT_AIN0, 4, 1}, // A4 P0.02
    {NRF_SAADC_INPUT_AIN1, 5, 1}, // A5 P0.03
    {NRF_SAADC_INPUT_AIN5, 6, 1}, // A6 P0.29 PIN_VBAT
    {NRF_SAADC_INPUT_AIN7, 7, 1}, // A7 P0.31 PIN_AREF

};

void initVariant(void)
{
}

#endif /* (UBINOS__BSP__BOARD_VARIATION__ADAFRUITNRF52840E == 1) */
#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

