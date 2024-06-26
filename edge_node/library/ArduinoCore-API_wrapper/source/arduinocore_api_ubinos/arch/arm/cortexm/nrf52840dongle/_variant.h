/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <ubinos.h>

#include <ubinos/bsp/arch.h>

#include "../nrf52/_variant_nrf52.h"

/* Wire  */
#define ARDUINO_WIRE_SCL_PIN NRF_GPIO_PIN_MAP(0, 27)
#define ARDUINO_WIRE_SDA_PIN NRF_GPIO_PIN_MAP(0, 26)

#define ARDUINO_WIRE_TWI_INSTANCE_ID 0

#endif /* _VARIANT_H_ */

