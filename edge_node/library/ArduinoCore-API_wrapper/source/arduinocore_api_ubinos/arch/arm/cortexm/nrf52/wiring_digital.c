/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__NRF52_NRF52XXX == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

void pinMode(pin_size_t pinNumber, PinMode pinMode)
{
    arduino_d_pin_t const * d_pin;

    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }

        d_pin = &_g_d_pin_map[pinNumber];

        if (d_pin->enable == 0)
        {
            logme("Not enabled");
            break;
        }

        switch (pinMode) {
        case INPUT:
            nrf_gpio_cfg_input(d_pin->no, NRF_GPIO_PIN_NOPULL);
            break;
        case OUTPUT:
            nrf_gpio_cfg_output(d_pin->no);
            break;
        case INPUT_PULLUP:
            nrf_gpio_cfg_input(d_pin->no, NRF_GPIO_PIN_PULLUP);
            break;
        case INPUT_PULLDOWN:
        default:
            nrf_gpio_cfg_input(d_pin->no, NRF_GPIO_PIN_PULLDOWN);
            break;
        }

        break;
    } while(1);
}

void digitalWrite(pin_size_t pinNumber, PinStatus status)
{
    arduino_d_pin_t const * d_pin;

    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }
        d_pin = &_g_d_pin_map[pinNumber];

        if (status == LOW)
        {
            nrf_gpio_pin_write(d_pin->no, 0);
        }
        else if (status == HIGH)
        {
            nrf_gpio_pin_write(d_pin->no, 1);
        }
        else
        {
            logme("status is unknown");
            break;
        }

        break;
    } while(1);
}

PinStatus digitalRead(pin_size_t pinNumber)
{
    PinStatus status = LOW;
    arduino_d_pin_t const * d_pin;

    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }
        d_pin = &_g_d_pin_map[pinNumber];

        status = nrf_gpio_pin_read(d_pin->no) ? HIGH : LOW;

        break;
    } while(1);

    return status;
}

#endif /* (UBINOS__BSP__NRF52_NRF52XXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

