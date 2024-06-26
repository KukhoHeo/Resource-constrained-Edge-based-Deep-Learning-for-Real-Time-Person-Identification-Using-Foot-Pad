/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__NRF52_NRF52XXX == 1)
#if (ARDUINOCORE_API__USE_WIRE == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

#include "../../../wiring_i2c.h"

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(ARDUINO_WIRE_TWI_INSTANCE_ID);

ubi_err_t wiring_i2c_init(uint32_t addr, uint32_t clock)
{
    ubi_err_t ubi_err = UBI_ERR_ERROR;
    ret_code_t nrf_err;
    nrf_drv_twi_frequency_t frequency;

    if (clock >= 400000)
    {
        frequency = NRF_DRV_TWI_FREQ_400K;
    }
    else if (clock >= 250000)
    {
        frequency = NRF_DRV_TWI_FREQ_250K;
    }
    else
    {
        frequency = NRF_DRV_TWI_FREQ_100K;
    }

    const nrf_drv_twi_config_t twi_config = {
       .scl                = ARDUINO_WIRE_SCL_PIN,
       .sda                = ARDUINO_WIRE_SDA_PIN,
       .frequency          = frequency,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    do
    {
        nrf_err = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
        if (nrf_err != NRF_SUCCESS) {
            break;
        }

        nrf_drv_twi_enable(&m_twi);

        ubi_err = UBI_ERR_OK;

        break;
    } while (1);

    return ubi_err;
}

ubi_err_t wiring_i2c_deinit(void)
{
    nrf_drv_twi_disable(&m_twi);

    nrf_drv_twi_uninit(&m_twi);

    return UBI_ERR_OK;
}

ubi_err_t wiring_i2c_master_write(uint8_t slave_addr, uint8_t *data, uint16_t size, uint8_t is_first, uint8_t is_last)
{
    ubi_err_t ubi_err = UBI_ERR_ERROR;
    ret_code_t nrf_err;

    nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_TX(slave_addr >> 1, data, size);

    do
    {
        nrf_err = nrfx_twim_xfer(&m_twi.u.twim, &xfer, is_last ? 0 : NRFX_TWIM_FLAG_TX_NO_STOP);
        if (nrf_err != NRF_SUCCESS) {
            break;
        }

        ubi_err = UBI_ERR_OK;

        break;
    } while (1);

    return ubi_err;
}

ubi_err_t wiring_i2c_master_read(uint8_t slave_addr, uint8_t *data, uint16_t size, uint8_t is_first, uint8_t is_last)
{
    ubi_err_t ubi_err = UBI_ERR_ERROR;
    ret_code_t nrf_err;

    nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_RX(slave_addr >> 1, data, size);

    do
    {
        nrf_err = nrfx_twim_xfer(&m_twi.u.twim, &xfer, is_last ? 0 : NRFX_TWIM_FLAG_TX_NO_STOP);
        if (nrf_err != NRF_SUCCESS) {
            break;
        }

        ubi_err = UBI_ERR_OK;

        break;
    } while (1);

    return ubi_err;
}

#endif /* (ARDUINOCORE_API__USE_WIRE == 1) */
#endif /* (UBINOS__BSP__NRF52_NRF52XXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

