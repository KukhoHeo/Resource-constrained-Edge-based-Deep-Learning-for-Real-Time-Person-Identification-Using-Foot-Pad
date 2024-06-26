/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)

#include <DttySerial.h>

using namespace arduino;

DttySerial SerialDtty;

int DttySerial::available(void)
{
    // FIXME: need to implement dtty_available()
    return dtty_kbhit();
}

int DttySerial::read(void)
{
    int r;
    char ch;

    r = dtty_getc_unblocked(&ch);
    if (r == 0)
    {
        r = (int) ch;
    }
    else {
        r = -1;
    }

    return r;
}

int DttySerial::peek(void)
{
    // FIXME: need to implement dtty_peek()
    return 0;
}

size_t DttySerial::write(uint8_t c)
{
    int r;

    r = dtty_putc((int) c);
    if (r == 0)
    {
        r = 1;
    }
    else
    {
        r = 0;
    }

    return r;
}

#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

