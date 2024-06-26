/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SPI_H_
#define SPI_H_

#include <api/HardwareSPI.h>

namespace arduino {

class UbiSPI : public SPIClass
{
public:
    UbiSPI()
    {
    }

    virtual ~UbiSPI()
    {
    }

    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);
    void transfer(const void *tx_buf, void *rx_buf, size_t count);

    // Transaction Functions
    void usingInterrupt(int interruptNumber);
    void notUsingInterrupt(int interruptNumber);
    void beginTransaction(SPISettings settings);
    void endTransaction(void);

    // SPI Configuration methods
    void attachInterrupt();
    void detachInterrupt();

    void begin();
    void end();
};

}

extern arduino::UbiSPI SPI;

#endif /* SPI_H_ */

