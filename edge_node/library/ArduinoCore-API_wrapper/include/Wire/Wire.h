/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBIWIRE_H_
#define UBIWIRE_H_

#include <api/HardwareI2C.h>

namespace arduino {

#define UBI_WIRE_RX_BUF_LEN 32
#define UBI_WIRE_TX_BUF_LEN 32

class UbiWire : public HardwareI2C
{
public:
    UbiWire()
        : state(UBI_WIRE_STATE_IDLE), addr(0x01), freq(100000), slaveAddr(0x00), rxBufIdx(0), rxBufReadIdx(0), txBufIdx(0), isFirstFrame(0)
    {
    }
    virtual ~UbiWire()
    {
    }

    void begin();
    void begin(uint8_t address);
    void end();

    void setClock(uint32_t freq);

    void beginTransmission(uint8_t address);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);

    size_t requestFrom(uint8_t address, size_t len, bool stopBit);
    size_t requestFrom(uint8_t address, size_t len);

    void onReceive(void(*)(int));
    void onRequest(void(*)(void));

    int available(void);
    int read(void);
    int peek(void);
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

private:
    enum{
        UBI_WIRE_STATE_IDLE,
        UBI_WIRE_STATE_TX,
        UBI_WIRE_STATE_RX,
    } STATE;

    uint8_t state;
    uint8_t addr;
    uint32_t freq;
    uint8_t slaveAddr;
    uint8_t rxBufIdx;
    uint8_t rxBufReadIdx;
    uint8_t txBufIdx;
    uint8_t isFirstFrame;

    uint8_t rxBuf[UBI_WIRE_RX_BUF_LEN];
    uint8_t txBuf[UBI_WIRE_TX_BUF_LEN];

    void resetRxBuf(void);

};

}

#if defined(ARDUINO_ARDUINO_NANO33BLE)
extern arduino::UbiWire Wire1;
#else
extern arduino::UbiWire Wire;
#endif

typedef arduino::UbiWire TwoWire;

#endif /* UBIWIRE_H_ */

