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

void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
    TIM_OC_InitTypeDef sConfig;
    uint32_t steps;

    do
    {
        if (pin >= NUM_DIGITAL_PINS)
        {
            logme("pin is out of range");
            break;
        }

        if (duration == 0 || frequency == 0)
        {
            if (_arduino_tone.timer_initiated)
            {
                noTone(pin);
            }
            break;
        }

        if (!_arduino_tone.pin_initiated || _arduino_tone.pin != pin)
        {
            pinMode(pin, OUTPUT);
            _arduino_tone.pin = pin;
            _arduino_tone.pin_initiated = 1;
        }
        _arduino_tone.pin_status = LOW;

        if (duration == 0)
        {
            _arduino_tone.zero_duration = 1;
        }
        else
        {
            _arduino_tone.zero_duration = 0;
        }

        steps = TIMER_TONE_BASE_CLOCK / (frequency * 2);
        steps = max((uint32_t) 1, steps);
        steps = min((uint32_t) TIMER_TONE_COUNT_MAX, steps);
        _arduino_tone.frequency_steps = steps;

        steps = TIMER_TONE_BASE_CLOCK / steps * duration / 1000;
        steps = max((uint32_t) 2, steps);
        _arduino_tone.duration_count = steps;

        if (!_arduino_tone.timer_initiated)
        {
            _arduino_tone.timer_handle.Instance = TIMER_TONE;
            _arduino_tone.timer_handle.Init.Period = TIMER_TONE_COUNT_MAX;
            _arduino_tone.timer_handle.Init.Prescaler = (TIMER_TONE_CLOCK / TIMER_TONE_BASE_CLOCK) - 1;
            _arduino_tone.timer_handle.Init.ClockDivision = 0;
            _arduino_tone.timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
            _arduino_tone.timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
            if (HAL_TIM_OC_Init(&_arduino_tone.timer_handle) != HAL_OK)
            {
                logme("fail at HAL_TIM_OC_Init");
                break;
            }

            sConfig.OCMode = TIM_OCMODE_TIMING;
            sConfig.Pulse = _arduino_tone.frequency_steps;
            sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
            if (HAL_TIM_OC_ConfigChannel(&_arduino_tone.timer_handle, &sConfig, TIMER_TONE_CHANNEL) != HAL_OK)
            {
                logme("fail at HAL_TIM_OC_ConfigChannel");
                break;
            }

            NVIC_SetPriority(TIMER_TONE_IRQn, NVIC_PRIO_LOWEST);
            NVIC_EnableIRQ(TIMER_TONE_IRQn);

            if (HAL_TIM_OC_Start_IT(&_arduino_tone.timer_handle, TIMER_TONE_CHANNEL) != HAL_OK)
            {
                logme("fail at HAL_TIM_OC_Start_IT");
                break;
            }

            _arduino_tone.timer_initiated = 1;
        }

        break;
    } while(1);
}

void noTone(uint8_t pin)
{
    do
    {
        if (pin >= NUM_DIGITAL_PINS)
        {
            logme("pin is out of range");
            break;
        }

        if (_arduino_tone.timer_initiated)
        {
            HAL_TIM_OC_Stop_IT(&_arduino_tone.timer_handle, TIMER_TONE_CHANNEL);
            HAL_TIM_OC_DeInit(&_arduino_tone.timer_handle);

            NVIC_DisableIRQ(TIMER_TONE_IRQn);
        }

        digitalWrite(_arduino_tone.pin, LOW);

        _arduino_tone.timer_initiated = 0;
        _arduino_tone.pin_initiated = 0;

        break;
    } while(1);
}

#ifdef __cplusplus
extern "C"{
#endif

void TIMER_TONE_IRQHandler(void)
{
    uint32_t uhCount = 0;
    uint32_t uhNextCh1 = 0;

    __HAL_TIM_CLEAR_IT(&_arduino_tone.timer_handle, TIMER_TONE_CHANNEL_IT);

    uhCount = __HAL_TIM_GET_COUNTER(&_arduino_tone.timer_handle);
    uhNextCh1 = (uhCount + _arduino_tone.frequency_steps) % TIMER_TONE_COUNT_MAX;
    __HAL_TIM_SET_COMPARE(&_arduino_tone.timer_handle, TIMER_TONE_CHANNEL, (uhNextCh1));

    if(_arduino_tone.zero_duration || _arduino_tone.duration_count > 0)
    {
        _arduino_tone.duration_count--;

        if (_arduino_tone.pin_status == HIGH)
        {
            _arduino_tone.pin_status = LOW;
        }
        else
        {
            _arduino_tone.pin_status = HIGH;
        }
        digitalWrite(_arduino_tone.pin, _arduino_tone.pin_status);
    }
    else
    {
        if (_arduino_tone.pin_status == HIGH)
        {
            _arduino_tone.pin_status = LOW;
            digitalWrite(_arduino_tone.pin, _arduino_tone.pin_status);
        }
    }
}

#ifdef	__cplusplus
}
#endif

#endif /* (UBINOS__BSP__STM32_STM32XXXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

