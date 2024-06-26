/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__STM32_STM32XXXX == 1)
#if (ARDUINOCORE_API__USE_WIRE == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

#include "../../../wiring_i2c.h"

/* I2C handler declaration */
I2C_HandleTypeDef _arduino_wire;

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO TX/RX clock */
    I2Cx_SCL_GPIO_CLK_ENABLE();
    I2Cx_SDA_GPIO_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = I2Cx_SCL_AF;

    HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
    GPIO_InitStruct.Alternate = I2Cx_SDA_AF;

    HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Enable I2C peripheral Clock ########################################*/
    /* Enable I2C1 clock */
    I2Cx_CLK_ENABLE();

    /*##-4- Configure the NVIC for I2C #########################################*/
    /* NVIC for I2C1 */
    HAL_NVIC_SetPriority(I2Cx_ER_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2Cx_ER_IRQn);
    HAL_NVIC_SetPriority(I2Cx_EV_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(I2Cx_EV_IRQn);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    /*##-1- Reset peripherals ##################################################*/
    I2Cx_FORCE_RESET();
    I2Cx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure I2C Tx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
    /* Configure I2C Rx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

    /*##-3- Disable the NVIC for I2C ###########################################*/
    HAL_NVIC_DisableIRQ(I2Cx_ER_IRQn);
    HAL_NVIC_DisableIRQ(I2Cx_EV_IRQn);
}

/* @brief  This function handles I2C event interrupt request. */
void I2Cx_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&_arduino_wire);
}

/* @brief  This function handles I2C error interrupt request. */
void I2Cx_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&_arduino_wire);
}

ubi_err_t wiring_i2c_init(uint32_t addr, uint32_t clock)
{
    HAL_StatusTypeDef stm_err;

    /*##-1- Configure the I2C peripheral #######################################*/
    _arduino_wire.Instance             = I2Cx;
    _arduino_wire.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    _arduino_wire.Init.ClockSpeed      = clock;
    _arduino_wire.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    _arduino_wire.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    _arduino_wire.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    _arduino_wire.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    _arduino_wire.Init.OwnAddress1     = addr;
    _arduino_wire.Init.OwnAddress2     = 0;

    stm_err = HAL_I2C_Init(&_arduino_wire);
    ubi_assert(stm_err == HAL_OK);

    return UBI_ERR_OK;
}

ubi_err_t wiring_i2c_deinit(void)
{
    HAL_StatusTypeDef stm_err;

    stm_err = HAL_I2C_DeInit(&_arduino_wire);
    ubi_assert(stm_err == HAL_OK);

    return UBI_ERR_OK;
}

ubi_err_t wiring_i2c_master_write(uint8_t slave_addr, uint8_t *data, uint16_t size, uint8_t is_first, uint8_t is_last)
{
    ubi_err_t ubi_err = UBI_ERR_ERROR;
    HAL_StatusTypeDef stm_err;
    uint32_t XferOptions;

    if (is_first)
    {
        if (is_last)
        {
            XferOptions = I2C_FIRST_AND_LAST_FRAME;
        }
        else
        {
            XferOptions = I2C_FIRST_FRAME;
        }
    }
    else
    {
        if (is_last)
        {
            XferOptions = I2C_OTHER_AND_LAST_FRAME;
        }
        else
        {
            XferOptions = I2C_LAST_FRAME;
        }
    }

    do
    {
        if (data == NULL)
        {
            break;
        }

        while (HAL_I2C_GetState(&_arduino_wire) != HAL_I2C_STATE_READY)
        {
        }
        do
        {
            stm_err = HAL_I2C_Master_Seq_Transmit_IT(&_arduino_wire, (uint16_t) slave_addr, (uint8_t*) data, size, XferOptions);
            if (stm_err != HAL_OK)
            {
                continue;
            }
            while (HAL_I2C_GetState(&_arduino_wire) != HAL_I2C_STATE_READY)
            {
            }
        }
        while(HAL_I2C_GetError(&_arduino_wire) == HAL_I2C_ERROR_AF);

        ubi_err = UBI_ERR_OK;

        break;
    } while(1);

    return ubi_err;
}

ubi_err_t wiring_i2c_master_read(uint8_t slave_addr, uint8_t *data, uint16_t size, uint8_t is_first, uint8_t is_last)
{
    ubi_err_t ubi_err = UBI_ERR_ERROR;
    HAL_StatusTypeDef stm_err;
    uint32_t XferOptions;

    if (is_first)
    {
        if (is_last)
        {
            XferOptions = I2C_FIRST_AND_LAST_FRAME;
        }
        else
        {
            XferOptions = I2C_FIRST_FRAME;
        }
    }
    else
    {
        if (is_last)
        {
            XferOptions = I2C_OTHER_AND_LAST_FRAME;
        }
        else
        {
            XferOptions = I2C_LAST_FRAME;
        }
    }

    do
    {
        if (data == NULL)
        {
            break;
        }

        while (HAL_I2C_GetState(&_arduino_wire) != HAL_I2C_STATE_READY)
        {
        }
        do
        {
            stm_err = HAL_I2C_Master_Seq_Receive_IT(&_arduino_wire, (uint16_t) slave_addr, (uint8_t*) data, size, XferOptions);
            if (stm_err != HAL_OK)
            {
                continue;
            }
            while (HAL_I2C_GetState(&_arduino_wire) != HAL_I2C_STATE_READY)
            {
            }
        }
        while(HAL_I2C_GetError(&_arduino_wire) == HAL_I2C_ERROR_AF);

        ubi_err = UBI_ERR_OK;

        break;
    } while(1);

    return ubi_err;
}


#endif /* (ARDUINOCORE_API__USE_WIRE == 1) */
#endif /* (UBINOS__BSP__STM32_STM32XXXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

