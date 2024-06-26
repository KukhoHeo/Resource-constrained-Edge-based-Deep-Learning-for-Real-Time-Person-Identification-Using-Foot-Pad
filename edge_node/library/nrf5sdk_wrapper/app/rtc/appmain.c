/**
 * Copyright (c) 2016 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**@cond To Make Doxygen skip documentation generation for this file.
 * @{
 */

#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include <nrf.h>
#include <nrf_gpio.h>
#include <nrf_drv_rtc.h>
#include <nrf_drv_clock.h>
#include <boards.h>
#include <app_error.h>

#define RTC NRF_RTC0
#define RTC_IRQ_NO RTC0_IRQn

static void rtc_isr(void) {
    nrf_gpio_pin_toggle(BSP_LED_0);
    nrf_rtc_event_clear(RTC, NRF_RTC_EVENT_TICK);
}


static void root_func(void *arg);
static void task1_func(void *arg);
static void task2_func(void *arg);


int appmain(int argc, char *argv[]) {
    int r;
    (void) r;

    srand(time(NULL));

    r = task_create(NULL, root_func, NULL, task_getmiddlepriority(), 0, "root");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}

static void root_func(void *arg) {
    int r;
    (void) r;
    ret_code_t err_code;
    (void) err_code;

    //
    printf("\n\n\n");
    printf("================================================================================\n");
    printf("rtc (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
    r = heap_printheapinfo(NULL);
    if (0 == r) {
        printf("\n");
        printf("================================================================================\n");
        printf("\n");
    }
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

    //
    bsp_board_init(BSP_INIT_LEDS);

    //
#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#else
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
#endif

    //
    nrf_rtc_prescaler_set(RTC, 4095);
    nrf_rtc_int_enable(RTC, RTC_INTENSET_TICK_Msk);
    nrf_rtc_task_trigger(RTC, NRF_RTC_TASK_CLEAR);
    nrf_rtc_task_trigger(RTC, NRF_RTC_TASK_START);

    intr_connectisr(RTC_IRQ_NO, rtc_isr, intr_getlowestpriority(), 0);
    intr_enable(RTC_IRQ_NO);

    r = task_create(NULL, task1_func, NULL, task_getmiddlepriority(), 0, "task1");
    ubi_assert(r == 0);

    r = task_create(NULL, task2_func, NULL, task_getmiddlepriority(), 0, "task2");
    ubi_assert(r == 0);
}

static void task1_func(void *arg) {
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        delayms = (rand() % 10 + 1) * 200;
        printf("1: hello world ! (%u) (delay = %4d ms)\n", i, delayms);
        task_sleepms(delayms);
    }
}

static void task2_func(void *arg) {
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        delayms = (rand() % 10 + 1) * 200;
        printf("2: hello world ! (%u) (delay = %4d ms)\n", i, delayms);
        task_sleepms(delayms);
    }
}

