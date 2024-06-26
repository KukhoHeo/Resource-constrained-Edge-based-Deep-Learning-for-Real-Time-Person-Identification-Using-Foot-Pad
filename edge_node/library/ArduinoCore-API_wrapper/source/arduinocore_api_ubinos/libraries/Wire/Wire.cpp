/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (ARDUINOCORE_API__USE_WIRE == 1)

#include <Wire.h>

#include "../../arch/wiring_i2c.h"

using namespace arduino;

#if defined(ARDUINO_ARDUINO_NANO33BLE)
UbiWire Wire1;
#else
UbiWire Wire;
#endif

void UbiWire::begin()
{
    wiring_i2c_init(addr, freq);
}

void UbiWire::begin(uint8_t address)
{
    this->addr = address;
    isFirstFrame = 1;
    begin();
}

void UbiWire::end()
{
    wiring_i2c_deinit();
}

void UbiWire::setClock(uint32_t freq)
{
    end();
    this->freq = freq;
    begin();
}


void UbiWire::beginTransmission(uint8_t address)
{
    state = UBI_WIRE_STATE_TX;
    slaveAddr = address << 1;
    txBufIdx = 0;
}

uint8_t UbiWire::endTransmission(bool stopBit)
{
    int r = 1;
    ubi_err_t ubi_err;
    uint8_t tmp_buf[4];

    do
    {
        if (txBufIdx > 0)
        {
            ubi_err = wiring_i2c_master_write(slaveAddr, txBuf, txBufIdx, isFirstFrame, stopBit);
        }
        else
        {
            // For detecting device
            ubi_err = wiring_i2c_master_read(slaveAddr, tmp_buf, 1, isFirstFrame, stopBit);
        }
        if (ubi_err == UBI_ERR_OK)
        {
            state = UBI_WIRE_STATE_IDLE;
            txBufIdx = 0;
            if (stopBit)
            {
                isFirstFrame = 1;
            }
            else
            {
                isFirstFrame = 0;
            }
            r = 0;
        }

        break;
    } while (1);

    return r;
}

uint8_t UbiWire::endTransmission(void)
{
    return endTransmission(true);
}

size_t UbiWire::requestFrom(uint8_t address, size_t len, bool stopBit)
{
    size_t read = 0;
    ubi_err_t ubi_err;

    do
    {
        if (state != UBI_WIRE_STATE_IDLE)
        {
            break;
        }
        if(len > UBI_WIRE_RX_BUF_LEN)
        {
            break;
        }

        state = UBI_WIRE_STATE_RX;

        resetRxBuf();
        slaveAddr = address << 1;
        rxBufIdx = 0;
        rxBufReadIdx = 0;
        ubi_err = wiring_i2c_master_read(slaveAddr, rxBuf, len, isFirstFrame, stopBit);
        if (ubi_err == UBI_ERR_OK)
        {
            rxBufIdx = len;
            if (stopBit)
            {
                isFirstFrame = 1;
            }
            else
            {
                isFirstFrame = 0;
            }
            read = len;
        }

        state = UBI_WIRE_STATE_IDLE;

        break;
    } while (1);

    return read;
}

size_t UbiWire::requestFrom(uint8_t address, size_t len)
{
    return requestFrom(address, len, true);
}

void UbiWire::onReceive(void (*function)(int))
{
    // FIXME: need to implement
}

void UbiWire::onRequest(void (*function)(void))
{
    // FIXME: need to implement
}


int UbiWire::available(void)
{
    // FIXME: need to implement
    return 0;
}

int UbiWire::read(void)
{
    int value = 0;

    do
    {
        if (state == UBI_WIRE_STATE_RX)
        {
            break;
        }
        if (rxBufIdx <= rxBufReadIdx)
        {
            break;
        }

        value = rxBuf[rxBufReadIdx++];

        break;
    } while (1);

    return value;
}

int UbiWire::peek(void)
{
    // FIXME: need to implement
    return 0;
}

size_t UbiWire::write(uint8_t c)
{
    size_t written = 0;

    do
    {
        if (state != UBI_WIRE_STATE_TX)
        {
            break;
        }
        if (txBufIdx >= UBI_WIRE_TX_BUF_LEN)
        {
            break;
        }

        txBuf[txBufIdx++] = c;
        written++;

        break;
    } while (1);

    return written;
}

size_t UbiWire::write(const uint8_t *buffer, size_t size)
{
    size_t written = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (state != UBI_WIRE_STATE_TX)
        {
            break;
        }
        if (txBufIdx >= UBI_WIRE_TX_BUF_LEN)
        {
            break;
        }
        if (buffer == NULL)
        {
            break;
        }

        txBuf[txBufIdx++] = buffer[i];
        written++;
    }

    return written;
}

inline void UbiWire::resetRxBuf(void)
{
    memset(rxBuf, 0, UBI_WIRE_RX_BUF_LEN);
}

#endif /* (ARDUINOCORE_API__USE_WIRE == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

