/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__STM32_STM32XXXX == 1)
#if (ARDUINOCORE_API__USE_SERVO == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

#include "../../../wiring_servo.h"

#define ANGLE_TO_PLUS_WIDTH(angle, plus_width_min, plus_width_max) \
            ((uint32_t) angle * (plus_width_max - plus_width_min) / 180 + plus_width_min)

#define PLUS_WIDTH_TO_ANGLE(plus_width, plus_width_min, plus_width_max) \
            ((uint32_t) plus_width * 180 / (plus_width_max - plus_width_min) - plus_width_min)

ubi_err_t wiring_servo_init(int pinNumber, int min, int max)
{
    ubi_err_t err;
    arduino_d_pin_t const * d_pin;
    arduino_d_pin_tim_t * d_pin_tim;
    GPIO_TypeDef * GPIO_Port;
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_HandleTypeDef *pTimHandle;
    TIM_OC_InitTypeDef *pTimConfig;
    uint32_t channel;
    uint32_t uhPrescalerValue;

    err = UBI_ERR_ERROR;
    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }

        d_pin = &_g_d_pin_map[pinNumber];
        d_pin_tim = &_g_d_pin_tims[pinNumber];

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

        uhPrescalerValue = ((uint32_t) d_pin->tim_clock / (SERVO_PWM_FREQUENCY * SERVO_PWM_PERIOD)) - 1;

        GPIO_Port = d_pin->port;
        GPIO_InitStruct.Pin = d_pin->no;
        GPIO_InitStruct.Alternate = d_pin->alternate;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

        HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);

        pTimHandle = &d_pin_tim->tim_handle;

        pTimHandle->Instance = d_pin->tim_instance;
        pTimHandle->Init.Prescaler = uhPrescalerValue;
        pTimHandle->Init.Period = SERVO_PWM_PERIOD - 1;
        pTimHandle->Init.ClockDivision = 0;
        pTimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
        pTimHandle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if(HAL_TIM_PWM_Init(pTimHandle) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_Init");
            break;
        }

        pTimConfig = &d_pin_tim->tim_config;

        pTimConfig->OCMode = TIM_OCMODE_PWM1;
        pTimConfig->OCPolarity = TIM_OCPOLARITY_HIGH;
        pTimConfig->OCFastMode = TIM_OCFAST_DISABLE;
        pTimConfig->Pulse = SERVO_PWM_PLUS_WIDTH_DEFAULT;

        channel = d_pin->tim_channel;

        if(HAL_TIM_PWM_ConfigChannel(pTimHandle, pTimConfig, channel) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_ConfigChannel");
            break;
        }

        if(HAL_TIM_PWM_Start(pTimHandle, channel) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_Start");
            break;
        }

        d_pin_tim->plus_width_min = min;
        d_pin_tim->plus_width_max = max;
        d_pin_tim->angle = PLUS_WIDTH_TO_ANGLE(pTimConfig->Pulse, min, max);

        err = UBI_ERR_OK;
        break;
    } while(1);

    return err;
}

ubi_err_t wiring_servo_deinit(int pinNumber)
{
    ubi_err_t err;
    arduino_d_pin_t const * d_pin;
    arduino_d_pin_tim_t * d_pin_tim;
    TIM_HandleTypeDef *pTimHandle;
    uint32_t channel;

    err = UBI_ERR_ERROR;
    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }
        d_pin = &_g_d_pin_map[pinNumber];
        d_pin_tim = &_g_d_pin_tims[pinNumber];

        pTimHandle = &d_pin_tim->tim_handle;

        if (pTimHandle->Instance == NULL)
        {
            logmw("not initiated");
            break;
        }

        channel = d_pin->tim_channel;

        if(HAL_TIM_PWM_Stop(pTimHandle, channel) != HAL_OK)
        {
            logme("fail at HAL_TIM_PWM_Stop");
            break;
        }

        pTimHandle->Instance = NULL;

        err = UBI_ERR_OK;
        break;
    } while(1);

    return err;
}

ubi_err_t wiring_servo_write(int pinNumber, int value)
{
    ubi_err_t err;
    arduino_d_pin_tim_t * d_pin_tim;
    TIM_HandleTypeDef *pTimHandle;
    uint32_t min, max, plus_width;

    err = UBI_ERR_ERROR;
    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }

        d_pin_tim = &_g_d_pin_tims[pinNumber];

        pTimHandle = &d_pin_tim->tim_handle;

        if (pTimHandle->Instance == NULL)
        {
            logmw("not initiated");
            break;
        }

        min = d_pin_tim->plus_width_min;
        max = d_pin_tim->plus_width_max;
        plus_width = ANGLE_TO_PLUS_WIDTH(value, min, max);

        err = wiring_servo_write_ms(pinNumber, plus_width);
        if (err != UBI_ERR_OK)
        {
            logmw("fail at wiring_servo_write_ms");
            break;
        }

        d_pin_tim->angle = value;

        err = UBI_ERR_OK;
        break;
    } while(1);

    return err;
}

ubi_err_t wiring_servo_write_ms(int pinNumber, int value)
{
    ubi_err_t err;
    arduino_d_pin_t const * d_pin;
    arduino_d_pin_tim_t * d_pin_tim;
    TIM_HandleTypeDef *pTimHandle;
    TIM_OC_InitTypeDef *pTimConfig;
    uint32_t channel;
    uint32_t min, max;

    err = UBI_ERR_ERROR;
    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }
        d_pin = &_g_d_pin_map[pinNumber];
        d_pin_tim = &_g_d_pin_tims[pinNumber];

        pTimHandle = &d_pin_tim->tim_handle;

        if (pTimHandle->Instance == NULL)
        {
            logmw("not initiated");
            break;
        }

        pTimConfig = &d_pin_tim->tim_config;
        channel = d_pin->tim_channel;

        pTimConfig->Pulse = value;
        switch (channel)
        {
            case TIM_CHANNEL_1:
                pTimHandle->Instance->CCR1 = value;
                break;
            case TIM_CHANNEL_2:
                pTimHandle->Instance->CCR2 = value;
                break;
            case TIM_CHANNEL_3:
                pTimHandle->Instance->CCR3 = value;
                break;
            case TIM_CHANNEL_4:
                pTimHandle->Instance->CCR4 = value;
                break;
        }

        min = d_pin_tim->plus_width_min;
        max = d_pin_tim->plus_width_max;
        d_pin_tim->angle = PLUS_WIDTH_TO_ANGLE(pTimConfig->Pulse, min, max);

        err = UBI_ERR_OK;
        break;
    } while(1);

    return err;
}

int wiring_servo_read(int pinNumber)
{
    int r;
    arduino_d_pin_tim_t * d_pin_tim;
    TIM_HandleTypeDef *pTimHandle;

    r = 0;
    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }
        
        d_pin_tim = &_g_d_pin_tims[pinNumber];

        pTimHandle = &d_pin_tim->tim_handle;

        if (pTimHandle->Instance == NULL)
        {
            logmw("not initiated");
            break;
        }

        r = d_pin_tim->angle;
        break;
    } while(1);

    return r;
}

int wiring_servo_attached(int pinNumber)
{
    int r;
    arduino_d_pin_tim_t * d_pin_tim;
    TIM_HandleTypeDef *pTimHandle;

    r = 0;
    do
    {
        if (pinNumber >= NUM_DIGITAL_PINS)
        {
            logme("pinNumber is out of range");
            break;
        }

        d_pin_tim = &_g_d_pin_tims[pinNumber];

        pTimHandle = &d_pin_tim->tim_handle;

        if (pTimHandle->Instance == NULL)
        {
            logmw("not initiated");
            break;
        }

        r = 1;
        break;
    } while(1);

    return r;
}

#endif /* (ARDUINOCORE_API__USE_SERVO == 1) */
#endif /* (UBINOS__BSP__STM32_STM32XXXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

