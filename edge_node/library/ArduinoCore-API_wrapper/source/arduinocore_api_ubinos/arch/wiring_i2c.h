/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _WIRING_I2C_H_
#define _WIRING_I2C_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <ubinos.h>

ubi_err_t wiring_i2c_init(uint32_t addr, uint32_t clock);
ubi_err_t wiring_i2c_deinit(void);
ubi_err_t wiring_i2c_master_write(uint8_t slave_addr, uint8_t *data, uint16_t size, uint8_t is_first, uint8_t is_last);
ubi_err_t wiring_i2c_master_read(uint8_t slave_addr, uint8_t *data, uint16_t size, uint8_t is_first, uint8_t is_last);

#ifdef	__cplusplus
}
#endif

#endif /* _WIRING_I2C_H_ */

