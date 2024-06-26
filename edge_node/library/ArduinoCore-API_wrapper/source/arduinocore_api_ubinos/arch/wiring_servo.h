/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _WIRING_SERVO_H_
#define _WIRING_SERVO_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <ubinos.h>
#include <stddef.h>

#define WIRING_SERVO_PULSE_WIDTH_MIN    544     // the shortest pulse sent to a servo  
#define WIRING_SERVO_PULSE_WIDTH_MAX    2400    // the longest pulse sent to a servo 

ubi_err_t wiring_servo_init(int pinNumber, int min, int max);
ubi_err_t wiring_servo_deinit(int pinNumber);
ubi_err_t wiring_servo_write(int pinNumber, int value);
ubi_err_t wiring_servo_write_ms(int pinNumber, int value);
int wiring_servo_read(int pinNumber);
int wiring_servo_attached(int pinNumber);

#ifdef	__cplusplus
}
#endif

#endif /* _WIRING_SERVO_H_ */

