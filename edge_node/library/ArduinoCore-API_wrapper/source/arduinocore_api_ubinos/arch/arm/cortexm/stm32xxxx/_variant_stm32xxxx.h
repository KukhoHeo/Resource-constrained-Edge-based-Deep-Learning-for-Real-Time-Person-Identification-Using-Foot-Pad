/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _VARIANT_STM32XXXX_H_
#define _VARIANT_STM32XXXX_H_

#include <ubinos.h>

#include <ubinos/bsp/arch.h>

#include <variant.h>

typedef struct _arduino_d_pin_t
{
    GPIO_TypeDef * port;
    uint32_t no;
    uint8_t support_pwm;
    uint32_t alternate;
    TIM_TypeDef * tim_instance;
    uint32_t tim_channel;
    uint32_t tim_clock;
    uint8_t enable;
} arduino_d_pin_t;

typedef struct _arduino_a_pin_t
{
    GPIO_TypeDef * port;
    uint32_t no;
    ADC_TypeDef * adc_instance;
    uint32_t adc_channel;
    uint8_t enable;
} arduino_a_pin_t;

typedef struct _arduino_tone_t
{
    TIM_HandleTypeDef timer_handle;
    uint8_t timer_initiated;

    uint8_t pin;
    PinStatus pin_status;
    uint8_t pin_initiated;

    uint32_t frequency_steps;
    uint32_t duration_count;
    uint8_t zero_duration;
    uint8_t enable;
} arduino_tone_t;

typedef struct _arduino_d_pin_tim_t
{
    TIM_HandleTypeDef tim_handle;
    TIM_OC_InitTypeDef tim_config;
    uint32_t plus_width_min;
    uint32_t plus_width_max;
    uint32_t angle;
} arduino_d_pin_tim_t;

extern arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS];
extern arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS];
extern arduino_d_pin_tim_t _g_d_pin_tims[NUM_DIGITAL_PINS];
extern arduino_tone_t _arduino_tone;
extern I2C_HandleTypeDef _arduino_wire;

#endif /* _VARIANT_STM32XXXX_H_ */

