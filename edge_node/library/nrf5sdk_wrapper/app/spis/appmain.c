/**
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
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

#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "sdk_config.h"
#include "nrf_drv_spis.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SPIS_INSTANCE 1 /**< SPIS instance index. */
static const nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE);/**< SPIS instance. */

#define TEST_STRING "Nordic"
static uint8_t m_tx_buf[] = TEST_STRING; /**< TX buffer. */
static uint8_t m_rx_buf[sizeof(TEST_STRING) + 1]; /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf); /**< Transfer length. */

//static volatile bool spis_xfer_done; /**< Flag used to indicate that SPIS instance completed the transfer. */

static sem_pt spi_xfer_done_sem = NULL;

/**
 * @brief SPIS user event handler.
 *
 * @param event
 */
void spis_event_handler(nrf_drv_spis_event_t event) {
    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE) {
        //spis_xfer_done = true;
        NRF_LOG_INFO(" Transfer completed. Received: %s", (uint32_t )m_rx_buf);
        sem_give(spi_xfer_done_sem);
    }
}

static void root_func(void *arg);
static void spi_xfer_func(void *arg);
static void task1_func(void *arg);
static void task2_func(void *arg);

int appmain(int argc, char *argv[]) {
    int r;
    (void) r;

    dtty_init();
    dtty_setecho(0);

    srand(time(NULL));

    r = task_create(NULL, root_func, NULL, task_getmiddlepriority(), 0, "root");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}

static void root_func(void *arg) {
    int r;
    (void) r;

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("spis (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    // Enable the constant latency sub power mode to minimize the time it takes
    // for the SPIS peripheral to become active after the CSN line is asserted
    // (when the CPU is in sleep mode).
    NRF_POWER->TASKS_CONSTLAT = 1;

    bsp_board_init(BSP_INIT_LEDS);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG;
    spis_config.csn_pin = APP_SPIS_CS_PIN;
    spis_config.miso_pin = APP_SPIS_MISO_PIN;
    spis_config.mosi_pin = APP_SPIS_MOSI_PIN;
    spis_config.sck_pin = APP_SPIS_SCK_PIN;

    APP_ERROR_CHECK(nrf_drv_spis_init(&spis, &spis_config, spis_event_handler));

    r = sem_create(&spi_xfer_done_sem);
    ubi_assert(r == 0);

    r = task_create(NULL, spi_xfer_func, NULL, task_getmiddlepriority(), 0, "spi_xfer");
    ubi_assert(r == 0);

    r = task_create(NULL, task1_func, NULL, task_getmiddlepriority(), 0, "task2");
    ubi_assert(r == 0);

    r = task_create(NULL, task2_func, NULL, task_getmiddlepriority(), 0, "task3");
    ubi_assert(r == 0);
}

static void spi_xfer_func(void *arg) {
    task_sleepms(1000);

    while (1) {
        memset(m_rx_buf, 0, m_length);

        APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length, m_rx_buf, m_length));

        sem_take(spi_xfer_done_sem);

        NRF_LOG_FLUSH();

        bsp_board_led_invert(BSP_BOARD_LED_0);
    }
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

