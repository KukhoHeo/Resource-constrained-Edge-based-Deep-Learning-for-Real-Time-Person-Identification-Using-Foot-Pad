/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM9XE512EK)

#if (UBINOS__BSP__USE_DTTY == 1)
#if (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__UART)

#define SLEEP_TIMEMS	1

extern int _g_bsp_dtty_init;
extern int _g_bsp_dtty_in_init;
extern int _g_bsp_dtty_echo;
extern int _g_bsp_dtty_autocr;

int dtty_init(void) {
    if (!_g_bsp_dtty_init && !_g_bsp_dtty_in_init)
    {
        _g_bsp_dtty_in_init = 1;

		/* Reset and disable receiver */
		AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX;

		/* Disable interrupts */
		AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;

		/* Configure PIOs for DBGU */
		AT91C_BASE_PIOB->PIO_ASR = AT91C_PB14_DRXD | AT91C_PB15_DTXD;
		AT91C_BASE_PIOB->PIO_BSR = 0;
		AT91C_BASE_PIOB->PIO_PDR = AT91C_PB14_DRXD | AT91C_PB15_DTXD;

		/* === Configure serial link === */
		/* Define the baud rate divisor register [BRGR = MCK / (115200 * 16)] */
		AT91C_BASE_DBGU->DBGU_BRGR = 54;
		/* Define the USART mode */
		AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE;

		/* Disable the RX and TX PDC transfer requests */
		AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTDIS;
		AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_TXTDIS;

		/* Enable transmitter */
		AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN | AT91C_US_RXEN;

		_g_bsp_dtty_echo = 1;
		_g_bsp_dtty_autocr = 1;

		_g_bsp_dtty_init = 1;

		_g_bsp_dtty_in_init = 0;
	}

	return 0;
}

int dtty_enable(void) {
	return 0;
}

int dtty_disable(void) {
	return 0;
}

int dtty_geterror(void) {
	return 0;
}

int dtty_getc(char * ch_p) {
	unsigned int i;

	if (NULL == ch_p) {
		return -2;
	}

	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

#if (INCLUDE__UBINOS__UBIK == 1)
	if(_bsp_kernel_active) {
		for (i=0; ; i++)
		{
			if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) {
				break;
			}
			if (255 <= i) {
				bsp_task_sleepms(SLEEP_TIMEMS);
				i = 0;
			}
		}
	}
	else {
		for (i=0; ; i++)
		{
			if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) {
				break;
			}
		}
	}
#else
	for (i=0; ; i++)
	{
		if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) {
			break;
		}
	}
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

	*ch_p = (char) ((AT91C_BASE_DBGU->DBGU_RHR) & 0xFF);

	if (0 != _g_bsp_dtty_echo) {
		dtty_putc(*ch_p);
	}

	return 0;
}

#pragma GCC push_options
#pragma GCC target ("arm")
int dtty_putc(int ch) {
	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

	if (0 != _g_bsp_dtty_autocr && '\n' == ch) {
	    for (;;)
	    {
	        if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) {
	            break;
	        }
	    }
	    AT91C_BASE_DBGU->DBGU_THR = ('\r' & 0xFF);
	}

    for (;;)
    {
        if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) {
            break;
        }
    }
    AT91C_BASE_DBGU->DBGU_THR = (ch & 0xFF);

    return 0;
}

int dtty_putn(const char * str, int len) {
    int i = 0;

    if (NULL == str) {
        return -2;
    }

    if (0 > len) {
        return -3;
    }

    for (i = 0; i < len; i++) {
        dtty_putc(*str);
        str++;
    }

    return i;
}
#pragma GCC pop_options

int dtty_kbhit(void) {
	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

	if (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) {
		return 1;
	} else {
		return 0;
	}
}

int dtty_flush(void) {
	return 0;
}

#endif /* (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__UART) */
#endif /* (UBINOS__BSP__USE_DTTY == 1) */

#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM9XE512EK) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

