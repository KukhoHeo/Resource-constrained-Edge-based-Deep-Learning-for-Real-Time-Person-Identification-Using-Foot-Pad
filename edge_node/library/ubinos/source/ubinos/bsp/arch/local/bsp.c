/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__LOCAL)

#include <unistd.h>

void bsp_busywaitms(unsigned int timems)
{
    usleep(timems * 1000);
}

#endif /* (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__LOCAL) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

