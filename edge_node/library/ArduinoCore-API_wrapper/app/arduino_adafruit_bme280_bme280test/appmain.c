/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ino/ino.h"

int appmain(int argc, char * argv[])
{
    int r;
    (void) r;

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("arduino_adafruit_bme280_bme280test (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    r = task_create(NULL, arduino_func, NULL, task_getlowestpriority(), 0, "arduino");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}
