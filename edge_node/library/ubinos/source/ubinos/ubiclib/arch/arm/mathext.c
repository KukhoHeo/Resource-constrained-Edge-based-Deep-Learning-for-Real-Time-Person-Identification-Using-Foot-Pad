/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../_ubiclib.h"

/*
 * Ref: http://aggregate.org/MAGIC/
 */

#if (INCLUDE__UBINOS__UBICLIB == 1)
#if (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__ARM)

#if !(UBINOS__UBICLIB__EXCLUDE_MATH_EXT == 1)

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

#define WORDBITS    32
#undef  LOG0UNDEFINED

unsigned int uiones(register unsigned int x) {
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    x += (x >> 16);
    return(x & 0x0000003f);
}

unsigned int uilog2_floor(register unsigned int x) {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
#ifdef  LOG0UNDEFINED
    return(uiones(x) - 1);
#else
    return(uiones(x >> 1));
#endif
}

unsigned int uilog2_ceil(register unsigned int x) {
    register int y = (x & (x - 1));

    y |= -y;
    y >>= (WORDBITS - 1);
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
#ifdef  LOG0UNDEFINED
    return(uiones(x) - 1 - y);
#else
    return(uiones(x >> 1) - y);
#endif
}

unsigned int uidiv_ceil(register unsigned int x, register unsigned int d) {
    unsigned int r;

    if (0 == x) {
        return 0;
    }
    r = x / d;
    if (0 == x % d) {
        return (r);
    }
    else {
        return (r+1);
    }
}

unsigned int uinlp2(register unsigned int x) {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return(x+1);
}

unsigned int uienlp2(register unsigned int x) {
    if (0 == x) {
        return 1;
    }
    else {
        x -= 1;
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return(x+1);
    }
}

unsigned int uilsb(register unsigned int x) {
    return (x & -x);
}

unsigned int uilzc(register unsigned int x) {
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return(WORDBITS - uiones(x));
}

unsigned int uimsb(register unsigned int x) {
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return(x & ~(x >> 1));
}

unsigned int uipow(unsigned int x, unsigned int y) {
    unsigned int v = 1;

    while (y > 0) {
        v *= x;
        y--;
    }

    return v;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_MATH_EXT == 1) */

#endif /* (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__ARM) */
#endif /* (INCLUDE__UBINOS__UBICLIB == 1) */


