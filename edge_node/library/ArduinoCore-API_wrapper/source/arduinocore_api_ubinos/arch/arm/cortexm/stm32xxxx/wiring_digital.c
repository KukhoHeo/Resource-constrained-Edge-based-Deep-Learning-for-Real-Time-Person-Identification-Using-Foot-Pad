/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__STM32_STM32XXXX == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

void pinMode(pin_size_t pinNumber, PinMode pinMode)
{
    GPIO_TypeDef * GPIO_Port;
    GPIO_InitTypeDef GPIO_InitStruct;
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

        GPIO_Port = d_pin->port;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Pin = d_pin->no;
        switch (pinMode) {
        case INPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
        case OUTPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
        case INPUT_PULLUP:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
        case INPUT_PULLDOWN:
        default:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
        }

        HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);

        break;
    } while(1);
}

void digitalWrite(pin_size_t pinNumber, PinStatus status)
{
    uint32_t GPIO_Pin;
    GPIO_TypeDef * GPIO_Port;
    GPIO_PinState PIN_State;
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
            PIN_State = GPIO_PIN_RESET;
        }
        else if (status == HIGH)
        {
            PIN_State = GPIO_PIN_SET;
        }
        else
        {
            logme("status is unknown");
            break;
        }

        GPIO_Pin = d_pin->no;
        GPIO_Port = d_pin->port;

        HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, PIN_State);

        break;
    } while(1);
}

PinStatus digitalRead(pin_size_t pinNumber)
{
    uint32_t GPIO_Pin;
    GPIO_TypeDef * GPIO_Port;
    GPIO_PinState PIN_State;
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

        GPIO_Pin = d_pin->no;
        GPIO_Port = d_pin->port;

        PIN_State = HAL_GPIO_ReadPin(GPIO_Port, GPIO_Pin);
        if (PIN_State == GPIO_PIN_RESET)
        {
            status = LOW;
        }
        else if (PIN_State == GPIO_PIN_SET)
        {
            status = HIGH;
        }
        else
        {
            logme("PIN_State is unknown");
            break;
        }

        break;
    } while(1);

    return status;
}

#endif /* (UBINOS__BSP__STM32_STM32XXXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

