/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/bsp.h>

#if (INCLUDE__UBINOS__BSP == 1)

char * _ubinos_version = UBINOS__VERSION;

volatile unsigned int _bsp_kernel_active = 0;
volatile unsigned int _bsp_critcount = 0;
volatile unsigned int _bsp_critcount_in_isr = 0;
volatile unsigned int _bsp_aborted = 0;

#endif /* (INCLUDE__UBINOS__BSP == 1) */

