/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE)
#if (UBINOS__BSP__BOARD_VARIATION__NRF52840DONGLE == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS] =
{
    {NRF_GPIO_PIN_MAP(1,  1), 1}, // D0
    {NRF_GPIO_PIN_MAP(1,  2), 1}, // D1
    {NRF_GPIO_PIN_MAP(1,  3), 1}, // D2
    {NRF_GPIO_PIN_MAP(1,  4), 1}, // D3
    {NRF_GPIO_PIN_MAP(1,  5), 1}, // D4
    {NRF_GPIO_PIN_MAP(1,  6), 1}, // D5
    {NRF_GPIO_PIN_MAP(1,  7), 1}, // D6
    {NRF_GPIO_PIN_MAP(1,  8), 1}, // D7
    {NRF_GPIO_PIN_MAP(1, 10), 1}, // D8
    {NRF_GPIO_PIN_MAP(1, 11), 1}, // D9
    {NRF_GPIO_PIN_MAP(1, 12), 1}, // D10
    {NRF_GPIO_PIN_MAP(1, 13), 1}, // D11
    {NRF_GPIO_PIN_MAP(1, 14), 1}, // D12
    {NRF_GPIO_PIN_MAP(1, 15), 1}, // D13
};

arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS] =
{
    {NRF_SAADC_INPUT_AIN5, 3, 1}, // A0 P0.29
    {NRF_SAADC_INPUT_AIN7, 5, 1}, // A1 P0.31
};

void initVariant(void)
{
}

#endif /* (UBINOS__BSP__BOARD_VARIATION__NRF52840DONGLE == 1) */
#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

