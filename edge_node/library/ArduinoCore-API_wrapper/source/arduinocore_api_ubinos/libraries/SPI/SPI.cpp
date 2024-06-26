/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (ARDUINOCORE_API__USE_SPI == 1)

#include <SPI.h>

using namespace arduino;

UbiSPI SPI;

uint8_t UbiSPI::transfer(uint8_t data)
{
    // FIXME: need to implement
    return 0;
}

uint16_t UbiSPI::transfer16(uint16_t data)
{
    // FIXME: need to implement
    return 0;
}

void UbiSPI::transfer(void *buf, size_t count)
{
    // FIXME: need to implement
}

void UbiSPI::transfer(const void *tx_buf, void *rx_buf, size_t count)
{
    // FIXME: need to implement
}

void UbiSPI::usingInterrupt(int interruptNumber)
{
    // FIXME: need to implement
}

void UbiSPI::notUsingInterrupt(int interruptNumber)
{
    // FIXME: need to implement
}

void UbiSPI::beginTransaction(SPISettings settings)
{
    // FIXME: need to implement
}

void UbiSPI::endTransaction(void)
{
    // FIXME: need to implement
}

#endif /* (ARDUINOCORE_API__USE_SPI == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

