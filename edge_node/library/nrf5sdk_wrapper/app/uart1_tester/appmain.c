/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#include <ubinos/bsp/arch.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

static void root_func(void *arg);
static void recv_func(void *arg);

static int clihookfunc(char *str, int len, void *arg);
static void clihelphookfunc();

static int uart1_cli_send(char *str, int len, void *arg);

static nrf_drv_uart_t _g_uart1 = NRF_DRV_UART_INSTANCE(1);
static void uart1_io_reset(void);
static void uart1_io_event_handler(nrf_drv_uart_event_t *p_event, void *p_context);

#define UART1_IOBUF_SIZE 1
static uint8_t _g_recv_iobuf[UART1_IOBUF_SIZE];
static uint8_t _g_send_iobuf[UART1_IOBUF_SIZE];

#define UART1_CBUF_SIZE 1024
static cbuf_pt _g_recv_cbuf;
static cbuf_pt _g_send_cbuf;

static uint8_t _g_send_iobusy = 0;

static sem_pt _g_recv_sem;

int appmain(int argc, char *argv[])
{
    int r;
    (void) r;

    r = task_create(NULL, root_func, NULL, task_getmiddlepriority(), 0, "root");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}

static void root_func(void *arg)
{
    int r;
    (void) r;
    ubi_err_t ubi_err;
    (void) ubi_err;

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("uart1_tester (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    ubi_err = cbuf_create(&_g_recv_cbuf, UART1_CBUF_SIZE);
    ubi_assert_ok(ubi_err);
    ubi_err = cbuf_create(&_g_send_cbuf, UART1_CBUF_SIZE);
    ubi_assert_ok(ubi_err);

    r = semb_create(&_g_recv_sem);
    ubi_assert(r == 0);

    uart1_io_reset();

    r = cli_sethookfunc(clihookfunc, NULL);
    ubi_assert(r == 0);

    r = cli_sethelphookfunc(clihelphookfunc);
    ubi_assert(r == 0);

    r = cli_setprompt("uart1_tester> ");
    ubi_assert(r == 0);

    r = task_create(NULL, cli_main, NULL, task_getmiddlepriority(), 192, "cli_main");
    ubi_assert(r == 0);

    r = task_create(NULL, recv_func, NULL, task_getmiddlepriority(), 192, "recv");
    ubi_assert(r == 0);
}

static void recv_func(void *arg)
{
    uint32_t len;
    cbuf_pt cbuf = _g_recv_cbuf;

    while (1)
    {
        len = cbuf_get_len(cbuf);
        if (len > 0)
        {
            printf("%c", *cbuf_get_head_addr(cbuf));
            cbuf_read(cbuf, NULL, 1, NULL);
        }
        else
        {
            sem_take(_g_recv_sem);
        }
    }
}

static int clihookfunc(char *str, int len, void *arg)
{
    int r = -1;
    char *tmpstr;
    int tmplen;
    char *cmd = NULL;
    int cmdlen = 0;

    tmpstr = str;
    tmplen = len;

    do
    {
        cmd = "send ";
        cmdlen = strlen(cmd);
        if (tmplen >= cmdlen && strncmp(tmpstr, cmd, cmdlen) == 0)
        {
            tmpstr = &tmpstr[cmdlen];
            tmplen -= cmdlen;

            r = uart1_cli_send(tmpstr, tmplen, arg);
            break;
        }

        break;
    } while (1);

    return r;
}

static void clihelphookfunc()
{
    printf("send <data>                                 : send data\n");
}

static int uart1_cli_send(char *str, int len, void *arg)
{
    int r = -1;
    uint8_t *iobuf = _g_send_iobuf;
    cbuf_pt cbuf = _g_send_cbuf;

    if (len > 0)
    {
        cbuf_write(cbuf, (const uint8_t *) str, len, NULL);

        if (_g_send_iobusy == 0)
        {
            _g_send_iobusy = 1;
            len = min(cbuf_get_len(cbuf), UART1_IOBUF_SIZE);
            cbuf_read(cbuf, iobuf, len, NULL);
            nrf_drv_uart_tx(&_g_uart1, iobuf, len);
        }
    }

    r = 0;

    return r;
}

static void uart1_io_reset(void)
{
    ret_code_t nrf_err;
    nrf_drv_uart_config_t config;

    uint32_t len;
    uint8_t *iobuf;

    config.pseltxd = ARDUINO_1_PIN;
    config.pselrxd = ARDUINO_0_PIN;

    config.pselcts = CTS_PIN_NUMBER;
    config.pselrts = RTS_PIN_NUMBER;
    config.p_context = NULL;
    config.hwfc = NRF_UART_HWFC_DISABLED;
    config.parity = NRF_UART_PARITY_EXCLUDED;
    config.baudrate = NRF_UART_BAUDRATE_115200;
    config.interrupt_priority = NVIC_PRIO_LOWEST;
#if defined(NRF_DRV_UART_WITH_UARTE) && defined(NRF_DRV_UART_WITH_UART)
    config.use_easy_dma = true;
#endif
    nrf_err = nrf_drv_uart_init(&_g_uart1, &config, uart1_io_event_handler);
    assert(nrf_err == NRF_SUCCESS);

    iobuf = _g_recv_iobuf;
    len = UART1_IOBUF_SIZE;
    nrf_drv_uart_rx(&_g_uart1, iobuf, len);
}

static void uart1_io_event_handler(nrf_drv_uart_event_t *p_event, void *p_context)
{
    uint32_t len;
    uint8_t *iobuf;
    cbuf_pt cbuf;
    uint8_t need_signal = 0;

    switch (p_event->type)
    {
    case NRF_DRV_UART_EVT_RX_DONE:
        cbuf = _g_recv_cbuf;
        iobuf = _g_recv_iobuf;

        len = p_event->data.rxtx.bytes;
        if (len > 0)
        {
            if (cbuf_get_len(cbuf) == 0)
            {
                need_signal = 1;
            }

            cbuf_write(cbuf, iobuf, len, NULL);

            if (need_signal)
            {
                sem_give(_g_recv_sem);
            }
        }
        len = UART1_IOBUF_SIZE;
        nrf_drv_uart_rx(&_g_uart1, iobuf, len);

        break;

    case NRF_DRV_UART_EVT_TX_DONE:
        cbuf = _g_send_cbuf;
        iobuf = _g_send_iobuf;

        len = min(cbuf_get_len(cbuf), UART1_IOBUF_SIZE);
        if (len > 0)
        {
            cbuf_read(cbuf, iobuf, len, NULL);
            nrf_drv_uart_tx(&_g_uart1, iobuf, len);
        }
        else
        {
            _g_send_iobusy = 0;
        }
        break;

    case NRF_DRV_UART_EVT_ERROR:
        break;

    default:
        break;
    }
}

