/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (ARDUINOCORE_API__USE_SERVO == 1)

#include <Servo.h>

#include "../../arch/wiring_servo.h"

using namespace arduino;

Servo::Servo()
    : pin(0), min(WIRING_SERVO_PULSE_WIDTH_MIN), max(WIRING_SERVO_PULSE_WIDTH_MAX)

{
}

uint8_t Servo::attach(int pin)
{
    return this->attach(pin, WIRING_SERVO_PULSE_WIDTH_MIN, WIRING_SERVO_PULSE_WIDTH_MAX);
}

uint8_t Servo::attach(int pin, int min, int max)
{
    ubi_err_t err;
    uint8_t r;

    err = wiring_servo_init(pin, min, max);
    if (err == UBI_ERR_OK)
    {
        this->pin = pin;
        this->min = min;
        this->max = max;
        r = 1;
    }
    else
    {
        r = 0;
    }

    return r;
}

void Servo::detach()
{
    wiring_servo_deinit(this->pin);
}

void Servo::write(int value)
{
    wiring_servo_write(this->pin, value);
}

void Servo::writeMicroseconds(int value)
{
    wiring_servo_write_ms(this->pin, value);
}

int Servo::read() const
{
    return wiring_servo_read(this->pin);
}

bool Servo::attached() const
{
    bool r;
    int value;

    value = wiring_servo_attached(this->pin);
    if (value == 1)
    {
        r = true;
    }
    else
    {
        r = false;
    }

    return r;
}

#endif /* (ARDUINOCORE_API__USE_SERVO == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

