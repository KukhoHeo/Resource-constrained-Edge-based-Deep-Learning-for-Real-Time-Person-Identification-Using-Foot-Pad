/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef PINS_ARDUINO_DIGITAL_H_
#define PINS_ARDUINO_DIGITAL_H_

#include "variant.h"

enum {
    D0, D1, D2,  D3,  D4,  D5,  D6,  D7,
    D8, D9, D10, D11, D12, D13,
    LED_BUILTIN,
    LED_POWER,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    NUM_DIGITAL_PINS
};

enum {
    LED1 = LED_BUILTIN,
    LED2 = LED_RED,
    LED3 = LED_GREEN,
    LED4 = LED_BLUE,
};

#endif /* PINS_ARDUINO_DIGITAL_H_ */

