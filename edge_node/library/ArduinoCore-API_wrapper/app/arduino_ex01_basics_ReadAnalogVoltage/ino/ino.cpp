/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#include <Arduino.h>

#include "ino.h"
#include "ReadAnalogVoltage/ReadAnalogVoltage.ino"

void arduino_func(void * arg)
{
    initVariant();
    setup();
    for (unsigned int i = 0; ; i++)
	{
        loop();
    }
}
