/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/bsp.h>
#include <ubinos/ubiclib.h>

#include <stdio.h>
#include <stdlib.h>

int appmain(int argc, char * argv[])
{
    printf("\n\n\n");
    printf("================================================================================\n");
    printf("Hi, World! (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");
    printf("\n");

    for (unsigned int i = 0; ; i++) {
        printf("Hi, World! (%u)\n", i);
        bsp_busywaitms(1000);
    }

    return 0;
}

