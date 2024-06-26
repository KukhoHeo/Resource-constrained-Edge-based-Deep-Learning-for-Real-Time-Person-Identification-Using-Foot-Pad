/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _VARIANT_NRF52_H_
#define _VARIANT_NRF52_H_

#include <ubinos.h>

#include <ubinos/bsp/arch.h>

#include <variant.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_twi.h"
#include "nrf_drv_saadc.h"

typedef struct _arduino_d_pin_t
{
    uint32_t no;
    uint8_t enable;
} arduino_d_pin_t;

typedef struct _arduino_a_pin_t
{
    nrf_saadc_input_t adc_input;
    uint8_t channel;
    uint8_t enable;
} arduino_a_pin_t;

extern arduino_d_pin_t const _g_d_pin_map[NUM_DIGITAL_PINS];
extern arduino_a_pin_t const _g_a_pin_map[NUM_ANALOG_INPUTS];

#endif /* _VARIANT_NRF52_H_ */

