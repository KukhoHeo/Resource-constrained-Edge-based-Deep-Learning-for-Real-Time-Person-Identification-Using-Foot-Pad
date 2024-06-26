/*
 * Copyright (c) 2019 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE)

#if (UBINOS__BSP__USE_DTTY == 1)
#if (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__UART)

#define SLEEP_TIMEMS	1

extern int _g_bsp_dtty_init;
extern int _g_bsp_dtty_in_init;
extern int _g_bsp_dtty_echo;
extern int _g_bsp_dtty_autocr;

#define GPIO_CFG(pin_number, pin_dir, pin_input, pin_pull, pin_drive, pin_sense) {     \
    NRF_GPIO->PIN_CNF[pin_number] =   ((uint32_t)pin_dir   << GPIO_PIN_CNF_DIR_Pos  )  \
                                    | ((uint32_t)pin_input << GPIO_PIN_CNF_INPUT_Pos)  \
                                    | ((uint32_t)pin_pull  << GPIO_PIN_CNF_PULL_Pos )  \
                                    | ((uint32_t)pin_drive << GPIO_PIN_CNF_DRIVE_Pos)  \
                                    | ((uint32_t)pin_sense << GPIO_PIN_CNF_SENSE_Pos); \
}

#define GPIO_PIN_OUTSET(pin_number) {             \
    NRF_GPIO->OUTSET = 1UL << pin_number;         \
}
#define GPIO_PIN_OUTCLR(pin_number) {             \
    NRF_GPIO->OUTCLR = 1UL << pin_number;         \
}

#define GPIO_CFG_OUTPUT(pin_number) {             \
    GPIO_CFG(pin_number,                          \
        GPIO_PIN_CNF_DIR_Output,                  \
        GPIO_PIN_CNF_INPUT_Disconnect,            \
        GPIO_PIN_CNF_PULL_Disabled,               \
        GPIO_PIN_CNF_DRIVE_S0S1,                  \
        GPIO_PIN_CNF_SENSE_Disabled);             \
}

#define GPIO_CFG_INPUT(pin_number, pull_config) { \
    GPIO_CFG(pin_number,                          \
        GPIO_PIN_CNF_DIR_Input,                   \
        GPIO_PIN_CNF_INPUT_Connect,               \
		pull_config,                              \
        GPIO_PIN_CNF_DRIVE_S0S1,                  \
        GPIO_PIN_CNF_SENSE_Disabled);             \
}

#define GPIO_CFG_DEFAULT(pin_number) {            \
    GPIO_CFG(pin_number,                          \
        GPIO_PIN_CNF_DIR_Input,                   \
        GPIO_PIN_CNF_INPUT_Disconnect,            \
        GPIO_PIN_CNF_PULL_Disabled,               \
        GPIO_PIN_CNF_DRIVE_S0S1,                  \
        GPIO_PIN_CNF_SENSE_Disabled);             \
}

static int _dtty_getc_advan(char *ch_p, int blocked);

int dtty_init(void)
{
    if (!_g_bsp_dtty_init && !_g_bsp_dtty_in_init)
    {
        _g_bsp_dtty_in_init = 1;

        NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);

        /* Disable receiver & transmitter interrupts */
        NRF_UART0->INTENCLR = 0xffffffff;

        /* Stop receiver & transmitter */
        NRF_UART0->TASKS_STOPTX = 1;
        NRF_UART0->TASKS_STOPRX = 1;

        /* Configure GPIO */
        GPIO_PIN_OUTSET(6); // TXD
        GPIO_CFG_OUTPUT(6);
        GPIO_CFG_INPUT(8, GPIO_PIN_CNF_PULL_Disabled); // RXD

        /* Configure serial link */
        NRF_UART0->BAUDRATE = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);
        NRF_UART0->CONFIG = (UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos) | (UART_CONFIG_HWFC_Disabled << UART_CONFIG_HWFC_Pos);
        NRF_UART0->PSELTXD = 6;
        NRF_UART0->PSELRXD = 8;
        NRF_UART0->EVENTS_ERROR = 0;
        NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);

        /* Start receiver & transmitter */
        NRF_UART0->TASKS_STARTTX = 1;
        NRF_UART0->TASKS_STARTRX = 1;

        _g_bsp_dtty_echo = 1;
        _g_bsp_dtty_autocr = 1;

        _g_bsp_dtty_init = 1;

		_g_bsp_dtty_in_init = 0;
	}

    return 0;
}

int dtty_enable(void)
{
    int r;

    if (!_g_bsp_dtty_init)
    {
        dtty_init();
    }
    NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
    r = 0;

    return r;
}

int dtty_disable(void)
{
    int r;

    if (!_g_bsp_dtty_init)
    {
        dtty_init();
    }
    NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);
    r = 0;

    return r;
}

int dtty_geterror(void)
{
    int r;

    if (!_g_bsp_dtty_init)
    {
        dtty_init();
    }
    r = NRF_UART0->EVENTS_ERROR;

    return r;
}

static int _dtty_getc_advan(char *ch_p, int blocked)
{
    unsigned int i;
    int r = 0;

    if (NULL == ch_p)
    {
        return -2;
    }

    if (!_g_bsp_dtty_init)
    {
        dtty_init();
    }

#if (INCLUDE__UBINOS__UBIK == 1)
    if (_bsp_kernel_active)
    {
        for (i = 0;; i++)
        {
            if (NRF_UART0->EVENTS_RXDRDY)
            {
                break;
            }
            if (blocked != 1)
            {
                r = -1;
                break;
            }
            if (255 <= i)
            {
                bsp_task_sleepms(SLEEP_TIMEMS);
                i = 0;
            }
        }
    }
    else
    {
        for (i = 0;; i++)
        {
            if (NRF_UART0->EVENTS_RXDRDY)
            {
                break;
            }
            if (blocked != 1)
            {
                r = -1;
                break;
            }
        }
    }
#else
    for (i=0;; i++)
    {
        if(NRF_UART0->EVENTS_RXDRDY)
        {
            break;
        }
        if (blocked != 1)
        {
            r = -1;
            break;
        }
    }
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

    if (r == 0)
    {
        *ch_p = (char) (0x000000ff & (NRF_UART0->RXD));

        NRF_UART0->EVENTS_RXDRDY = 0x0UL;

        if (0 != _g_bsp_dtty_echo)
        {
            dtty_putc(*ch_p);
        }
    }

    return r;
}

int dtty_getc(char *ch_p)
{
    return _dtty_getc_advan(ch_p, 1);
}

int dtty_getc_unblocked(char *ch_p)
{
    return _dtty_getc_advan(ch_p, 0);
}

int dtty_putc(int ch)
{
    int r;

    if (!_g_bsp_dtty_init)
    {
        dtty_init();
    }
    if (0 != _g_bsp_dtty_autocr && '\n' == ch)
    {
        NRF_UART0->EVENTS_TXDRDY = 0x0UL;
        NRF_UART0->TXD = (0x000000ff & '\r');
        for (;;)
        {
            if (NRF_UART0->EVENTS_TXDRDY)
            {
                break;
            }
        }
    }

    NRF_UART0->EVENTS_TXDRDY = 0x0UL;
    NRF_UART0->TXD = (0x000000ff & ch);
    for (;;)
    {
        if (NRF_UART0->EVENTS_TXDRDY)
        {
            break;
        }
    }

    r = 0;

    return r;
}

int dtty_putn(const char *str, int len)
{
    int i = 0;

    if (NULL == str)
    {
        return -2;
    }

    if (0 > len)
    {
        return -3;
    }

    for (i = 0; i < len; i++)
    {
        dtty_putc(*str);
        str++;
    }

    return i;
}

int dtty_kbhit(void)
{
    int r;

    if (!_g_bsp_dtty_init)
    {
        dtty_init();
    }
    if (NRF_UART0->EVENTS_RXDRDY)
    {
        r = 1;
    }
    else
    {
        r = 0;
    }

    return r;
}

int dtty_flush(void)
{
    return 0;
}

#endif /* (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__UART) */
#endif /* (UBINOS__BSP__USE_DTTY == 1) */

#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

