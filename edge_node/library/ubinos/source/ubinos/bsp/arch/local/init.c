/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/bsp.h>

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__LOCAL)

#include <stdio.h>

int main(int argc, char* argv[])
{
    int r = 0;
    
    r = appmain(argc, argv);

    return r;
}

#endif /* (UBINOS__BSP__CPU_ARCH == ...) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

