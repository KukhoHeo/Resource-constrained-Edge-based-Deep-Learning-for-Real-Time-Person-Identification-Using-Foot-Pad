/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)

uint32_t microsecondsToClockCycles(uint32_t time_ms)
{
    int r;
    (void) r;

    unsigned int freqk_p;

    r = bsp_getcpuclockfreqk(&freqk_p);
    ubi_assert(r == 0);

    return (time_ms * (freqk_p / 1000));
}

void yield(void)
{
    task_yield();
}

#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

