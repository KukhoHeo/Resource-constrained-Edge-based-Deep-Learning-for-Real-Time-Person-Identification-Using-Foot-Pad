/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DTTYSERIAL_H_
#define DTTYSERIAL_H_

#include <api/Stream.h>

namespace arduino {

class DttySerial : public Stream
{
public:
    DttySerial()
    {
    }
    virtual ~DttySerial()
    {
    }

    void begin(unsigned long baud)
    {
    }
    operator bool()
    {
      return true;
    }

    int available(void);
    int read(void);
    int peek(void);
    size_t write(uint8_t);
};

}

extern arduino::DttySerial SerialDtty;

#endif /* DTTYSERIAL_H_ */

