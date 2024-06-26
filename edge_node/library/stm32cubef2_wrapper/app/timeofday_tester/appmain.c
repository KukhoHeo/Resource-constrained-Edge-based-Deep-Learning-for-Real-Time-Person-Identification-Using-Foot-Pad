/*
 * Copyright (c) 2022 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

static void root_func(void * arg);

int appmain(int argc, char * argv[])
{
    int r;
    (void) r;

    r = task_create(NULL, root_func, NULL, task_getmiddlepriority() + 1, 0, "root");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}

static void root_func(void * arg)
{
    int r;
    (void) r;

    struct timeval tv;
    struct tm tm_data;
    struct tm* tm_ptr;
    char time_string[40];

    unsigned int delayms;

    int year, mon, mday, hour, min, sec;

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("timeofday_tester (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    printf("Input time of day: year-mon-mday hour:min:sec\n");
    printf("    Ex) 2022-03-28 20:12:10\n");
    scanf("%d-%d-%d %d:%d:%d", &year, &mon, &mday, &hour, &min, &sec);
    printf("\n");

    tm_data.tm_year = year - 1900;
    tm_data.tm_mon = mon - 1;
    tm_data.tm_mday = mday;
    tm_data.tm_hour = hour;
    tm_data.tm_min = min;
    tm_data.tm_sec = sec;
    tm_data.tm_isdst = -1;

    tv.tv_sec = mktime(&tm_data);
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);

    srand(time(NULL));

    for (unsigned int i = 0; ; i++)
    {
        delayms = (rand() % 100 + 1) * 100;

        gettimeofday(&tv, NULL);
        tm_ptr = localtime(&tv.tv_sec);
        strftime(time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", tm_ptr);

        printf("%s (%5u) (delay = %5d ms)\n", time_string, i, delayms);

        task_sleepms(delayms);
    }
}

