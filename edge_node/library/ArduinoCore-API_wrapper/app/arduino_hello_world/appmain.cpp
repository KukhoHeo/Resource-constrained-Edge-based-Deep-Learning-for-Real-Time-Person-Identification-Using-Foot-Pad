/*
 * Copyright (c) 2021 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <Arduino.h>

static void root_func(void * arg);
static void task1_func(void * arg);
static void task2_func(void * arg);

#define MSG_BUF_SIZE 512

static char _g_msg_buf_1[MSG_BUF_SIZE];
static char _g_msg_buf_2[MSG_BUF_SIZE];

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

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("Arduino Hello, World! (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    srand(time(NULL));

    r = task_create(NULL, task1_func, NULL, task_getmiddlepriority(), 0, "task1");
    ubi_assert(r == 0);

    r = task_create(NULL, task2_func, NULL, task_getmiddlepriority(), 0, "task2");
    ubi_assert(r == 0);
}

static void task1_func(void * arg)
{
    unsigned int delayms;

    printf("\n");

    for (unsigned int i = 0; ; i++)
    {
        delayms = (rand() % 10 + 1) * 200;
        snprintf(_g_msg_buf_1, MSG_BUF_SIZE, "task1: hello world ! (%u) (delay = %4d ms)\n", i, delayms);
        Serial.println(_g_msg_buf_1);
        delay(delayms);
    }
}

static void task2_func(void * arg)
{
    unsigned int delayms;

    printf("\n");

    for (unsigned int i = 0; ; i++)
    {
        delayms = (rand() % 10 + 1) * 200;
        snprintf(_g_msg_buf_2, MSG_BUF_SIZE, "task2: hello world ! (%u) (delay = %4d ms)\n", i, delayms);
        Serial.println(_g_msg_buf_2);
        delay(delayms);
    }
}

