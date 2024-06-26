/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef PINS_ARDUINO_DIGITAL_H_
#define PINS_ARDUINO_DIGITAL_H_

#include "variant.h"

enum {
    D5 = 5, D6,
    D9 = 9, D10, D11, D12, D13,
    LED_BUILTIN, 
    LED_BUILTIN_2,
    NUM_DIGITAL_PINS
};

enum {
    LED1 = LED_BUILTIN,
    LED2 = LED_BUILTIN_2,
};

#endif /* PINS_ARDUINO_DIGITAL_H_ */

