/*
 * Copyright (c) 2019 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA)

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
    unsigned int tmp;
    unsigned int tmp2;
    unsigned char * tmp_p;

    if (NULL == buf) {
        return -2;
    }
    if (8 > max) {
        return -3;
    }

    tmp = (unsigned int) ((NRF_FICR->INFO.PART >> FICR_INFO_PART_PART_Pos) & FICR_INFO_PART_PART_Msk);
    tmp2 = (unsigned int) ((NRF_FICR->INFO.VARIANT >> FICR_INFO_VARIANT_VARIANT_Pos) & FICR_INFO_VARIANT_VARIANT_Msk);

#if (UBINOS__BSP__CPU_ENDIAN__BIG == UBINOS__BSP__CPU_ENDIAN)
    tmp_p = (unsigned char *) &tmp;
    buf[0] = tmp_p[0];
    buf[1] = tmp_p[1];
    buf[2] = tmp_p[2];
    buf[3] = tmp_p[3];
    tmp_p = (unsigned char *) &tmp2;
    buf[4] = tmp_p[0];
    buf[5] = tmp_p[1];
    buf[6] = tmp_p[2];
    buf[7] = tmp_p[3];
#else
    tmp_p = (unsigned char *) &tmp;
    buf[0] = tmp_p[3];
    buf[1] = tmp_p[2];
    buf[2] = tmp_p[1];
    buf[3] = tmp_p[0];
    tmp_p = (unsigned char *) &tmp2;
    buf[4] = tmp_p[3];
    buf[5] = tmp_p[2];
    buf[6] = tmp_p[1];
    buf[7] = tmp_p[0];
#endif

    return 4;
}

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

