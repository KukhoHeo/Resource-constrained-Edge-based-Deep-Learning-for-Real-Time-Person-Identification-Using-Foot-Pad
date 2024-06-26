/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__STM32_STM32F100 == 1)

int bsp_getcpuclockfreq(unsigned int * freq_p) {
    if (NULL == freq_p) {
        return -2;
    }

    return -1;
}

int bsp_getcpuclockfreqk(unsigned int * freqk_p) {
    if (NULL == freqk_p) {
        return -2;
    }

    *freqk_p = UBINOS__BSP__CLOCK_FREQ_MAIN;

    return 0;
}

int bsp_getmckfreq(unsigned int * freq_p) {
    return bsp_getcpuclockfreq(freq_p);
}

int bsp_getmckfreqk(unsigned int * freqk_p) {
    return bsp_getcpuclockfreqk(freqk_p);
}

int bsp_getcpuidsize(void) {
    return 8;
}

int bsp_getcpuid(unsigned char * buf, int max) {
    return 4;
}

#endif /* (UBINOS__BSP__STM32_STM32F100 == 1) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

