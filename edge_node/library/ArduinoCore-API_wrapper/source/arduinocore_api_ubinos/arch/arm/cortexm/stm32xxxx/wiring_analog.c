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

static int _readResolution = 10;

void analogReadResolution(int res)
{
  _readResolution = res;
}

int analogRead(pin_size_t pinNumber)
{
    ADC_HandleTypeDef AdcHandle;
    ADC_ChannelConfTypeDef sConfig;
    arduino_a_pin_t const * a_pin;
    uint16_t uhADCxConvertedValue = 0;

    do
    {
        if (pinNumber >= NUM_ANALOG_INPUTS)
        {
            logme("pinNumber is out of range");
            break;
        }

        memset(&AdcHandle, 0, sizeof(ADC_HandleTypeDef));

        a_pin = &_g_a_pin_map[pinNumber];

        if (a_pin->enable == 0)
        {
            logme("Not enabled");
            break;
        }

        AdcHandle.Instance = a_pin->adc_instance;
        AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
        if (_readResolution <= 6) {
            AdcHandle.Init.Resolution = ADC_RESOLUTION_6B;
        } else if (_readResolution <= 8) {
            AdcHandle.Init.Resolution = ADC_RESOLUTION_8B;
        } else if (_readResolution <= 10) {
            AdcHandle.Init.Resolution = ADC_RESOLUTION_10B;
        } else {
            AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
        }
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

        if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
        {
            logme("fail at HAL_ADC_Init");
            break;
        }

        sConfig.Channel = a_pin->adc_channel;
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        sConfig.Offset = 0;

        if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
        {
            logme("fail at HAL_ADC_ConfigChannel");
            break;
        }

        if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
        {
            logme("fail at HAL_ADC_Start");
            break;
        }

        HAL_ADC_PollForConversion(&AdcHandle, 10);

        if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) != HAL_ADC_STATE_EOC_REG)
        {
            logme("fail at HAL_ADC_GetState");
            break;
        }

        uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);

        break;
    } while(1);

    return (int) uhADCxConvertedValue;
}

void analogWrite(pin_size_t pinNumber, int value)
{
    GPIO_TypeDef * GPIO_Port;
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_HandleTypeDef TimHandle;
    TIM_OC_InitTypeDef sConfig;
    uint32_t channel;
    arduino_d_pin_t const * d_pin;
    uint32_t uhPrescalerValue;

    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }
        d_pin = &_g_d_pin_map[pinNumber];

        if (!d_pin->support_pwm)
        {
            logme("not support pwm");
            break;
        }
        if (d_pin->tim_instance != TIM1 && d_pin->tim_instance != TIM3 && d_pin->tim_instance != TIM4)
        {
            logme("not support TIM");
            break;
        }
        uhPrescalerValue = ((uint32_t) d_pin->tim_clock / (PWM_FREQUENCY * PWM_MAX_DUTY_CYCLE)) - 1;

        GPIO_Port = d_pin->port;
        GPIO_InitStruct.Pin = d_pin->no;
        GPIO_InitStruct.Alternate = d_pin->alternate;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

        HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);

        TimHandle.Instance = d_pin->tim_instance;
        TimHandle.Init.Prescaler = uhPrescalerValue;
        TimHandle.Init.Period = PWM_MAX_DUTY_CYCLE - 1;
        TimHandle.Init.ClockDivision = 0;
        TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
        TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_Init");
            break;
        }

        sConfig.OCMode = TIM_OCMODE_PWM1;
        sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfig.OCFastMode = TIM_OCFAST_DISABLE;
        sConfig.Pulse = value;

        channel = d_pin->tim_channel;

        if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_ConfigChannel");
            break;
        }

        if(HAL_TIM_PWM_Start(&TimHandle, channel) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_Start");
            break;
        }

        break;
    } while(1);
}

#endif /* (UBINOS__BSP__STM32_STM32XXXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

