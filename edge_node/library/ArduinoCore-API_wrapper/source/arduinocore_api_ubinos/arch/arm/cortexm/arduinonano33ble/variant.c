/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE)
#if (UBINOS__BSP__BOARD_VARIATION__ARDUINONANO33BLE == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS] =
{
    {NRF_GPIO_PIN_MAP(1,  3), 1}, // D0
    {NRF_GPIO_PIN_MAP(1, 10), 1}, // D1
    {NRF_GPIO_PIN_MAP(1, 11), 1}, // D2
    {NRF_GPIO_PIN_MAP(1, 12), 1}, // D3
    {NRF_GPIO_PIN_MAP(1, 15), 1}, // D4
    {NRF_GPIO_PIN_MAP(1, 13), 1}, // D5
    {NRF_GPIO_PIN_MAP(1, 14), 1}, // D6
    {NRF_GPIO_PIN_MAP(0, 23), 1}, // D7
    {NRF_GPIO_PIN_MAP(0, 21), 1}, // D8
    {NRF_GPIO_PIN_MAP(0, 27), 1}, // D9
    {NRF_GPIO_PIN_MAP(1,  2), 1}, // D10
    {NRF_GPIO_PIN_MAP(1,  1), 1}, // D11
    {NRF_GPIO_PIN_MAP(1,  8), 1}, // D12
    {NRF_GPIO_PIN_MAP(0, 13), 1}, // D13
    {NRF_GPIO_PIN_MAP(0, 13), 1}, // LED_BUILTIN
    {NRF_GPIO_PIN_MAP(1,  9), 1}, // LED_POWER
    {NRF_GPIO_PIN_MAP(0, 24), 1}, // LED_RED
    {NRF_GPIO_PIN_MAP(0, 16), 1}, // LED_GREEN
    {NRF_GPIO_PIN_MAP(0,  6), 1}, // LED_BLUE
};

arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS] =
{
    {NRF_SAADC_INPUT_AIN2, 0, 1}, // A0 P0.04
    {NRF_SAADC_INPUT_AIN3, 1, 1}, // A1 P0.05
    {NRF_SAADC_INPUT_AIN6, 2, 1}, // A2 P0.30
    {NRF_SAADC_INPUT_AIN5, 3, 1}, // A3 P0.29
    {NRF_SAADC_INPUT_AIN7, 4, 1}, // A4 P0.31
    {NRF_SAADC_INPUT_AIN0, 5, 1}, // A5 P0.02
    {NRF_SAADC_INPUT_AIN4, 6, 1}, // A6 P0.28
    {NRF_SAADC_INPUT_AIN1, 7, 1}, // A7 P0.03
};

void initVariant(void)
{
    uint32_t on_led_pin = NRF_GPIO_PIN_MAP(1, 9);
    nrf_gpio_cfg_output(on_led_pin);
    nrf_gpio_pin_write(on_led_pin, 1);

    uint32_t i2c_pull_pin = NRF_GPIO_PIN_MAP(1, 0);
    nrf_gpio_cfg_output(i2c_pull_pin);
    nrf_gpio_pin_write(i2c_pull_pin, 1);

    uint32_t vdd_env_enable_pin = NRF_GPIO_PIN_MAP(0, 22);
    nrf_gpio_cfg_output(vdd_env_enable_pin);
    nrf_gpio_pin_write(vdd_env_enable_pin, 1);

    delay(100);
}

#endif /* (UBINOS__BSP__BOARD_VARIATION__ARDUINONANO33BLE == 1) */
#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

