/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if ((UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L476GEVAL) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOL476RG) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L475EIOT01))

#if (UBINOS__BSP__USE_DTTY == 1)
#if (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__UART)

#include "../stm32l4/stm32l4xx_ll_bus.h"
#include "../stm32l4/stm32l4xx_ll_gpio.h"
#include "../stm32l4/stm32l4xx_ll_usart.h"

#if (UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER == 1)

#define USARTx_INSTANCE               USART1
#define USARTx_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1)

#define USARTx_TX_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define USARTx_TX_PIN                 LL_GPIO_PIN_6
#define USARTx_TX_GPIO_PORT           GPIOB
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_AF_7)

#define USARTx_RX_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define USARTx_RX_PIN                 LL_GPIO_PIN_7
#define USARTx_RX_GPIO_PORT           GPIOB
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_AF_7)

#elif (UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER == 2)

#define USARTx_INSTANCE               USART2
#define USARTx_CLK_ENABLE()           LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)

#define USARTx_TX_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define USARTx_TX_PIN                 LL_GPIO_PIN_2
#define USARTx_TX_GPIO_PORT           GPIOA
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_AF_7)

#define USARTx_RX_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define USARTx_RX_PIN                 LL_GPIO_PIN_3
#define USARTx_RX_GPIO_PORT           GPIOA
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_AF_7)

#elif (UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER == 3)

#define USARTx_INSTANCE               USART3
#define USARTx_CLK_ENABLE()           LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)

#define USARTx_TX_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define USARTx_TX_PIN                 LL_GPIO_PIN_4
#define USARTx_TX_GPIO_PORT           GPIOC
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_AF_7)

#define USARTx_RX_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define USARTx_RX_PIN                 LL_GPIO_PIN_5
#define USARTx_RX_GPIO_PORT           GPIOC
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_AF_7)

#else

#error "Unsupported UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER"

#endif

#if ((UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L476GEVAL))

#define APB_Div 2

#elif ((UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOL476RG) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L475EIOT01))

#define APB_Div 1

#else

#error "Unsupported UBINOS__BSP__BOARD_MODEL"

#endif

#define SLEEP_TIMEMS	1

extern int _g_bsp_dtty_init;
extern int _g_bsp_dtty_in_init;
extern int _g_bsp_dtty_echo;
extern int _g_bsp_dtty_autocr;

static void Configure_USART(void) {

	/* (1) Enable the peripheral clock of GPIO Port */

	/* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
	USARTx_TX_GPIO_CLK_ENABLE();
	LL_GPIO_SetPinMode(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_MODE_ALTERNATE);
	USARTx_SET_TX_GPIO_AF();
	LL_GPIO_SetPinSpeed(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_PULL_UP);

	/* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
	USARTx_RX_GPIO_CLK_ENABLE();
	LL_GPIO_SetPinMode(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_MODE_ALTERNATE);
	USARTx_SET_RX_GPIO_AF();
	LL_GPIO_SetPinSpeed(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_PULL_UP);

	/* (2) Enable USART peripheral clock and clock source ***********************/
	USARTx_CLK_ENABLE();

	/* (3) Configure USART functional parameters ********************************/

	/* Disable USART prior modifying configuration registers */
	/* Note: Commented as corresponding to Reset value */
	LL_USART_Disable(USARTx_INSTANCE);

	/* TX/RX direction */
	LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);

	/* 8 data bit, 1 start bit, 1 stop bit, no parity */
	LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	/* No Hardware Flow control */
	/* Reset value is LL_USART_HWCONTROL_NONE */
	LL_USART_SetHWFlowCtrl(USARTx_INSTANCE, LL_USART_HWCONTROL_NONE);

	/* Oversampling by 16 */
	/* Reset value is LL_USART_OVERSAMPLING_16 */
	LL_USART_SetOverSampling(USARTx_INSTANCE, LL_USART_OVERSAMPLING_16);

	/* Set Baudrate to 115200 using APB frequency set to 120000000/APB_Div Hz */
	/* Frequency available for USART peripheral can also be calculated through LL RCC macro */
	/* Ex :
	 Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance

	 In this example, Peripheral Clock is expected to be equal to 120000000/APB_Div Hz => equal to SystemCoreClock/APB_Div
	 */
	LL_USART_SetBaudRate(USARTx_INSTANCE, SystemCoreClock / APB_Div, LL_USART_OVERSAMPLING_16, 115200);

	/* (4) Enable USART *********************************************************/
	LL_USART_Enable(USARTx_INSTANCE);
}

int dtty_init(void) {
    if (!_g_bsp_dtty_init && !_g_bsp_dtty_in_init)
    {
        _g_bsp_dtty_in_init = 1;

		Configure_USART();

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

int dtty_getc(char *ch_p) {
	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

	unsigned int i;

	if (NULL == ch_p) {
		return -2;
	}

#if (INCLUDE__UBINOS__UBIK == 1)
	if (_bsp_kernel_active) {
		for (i = 0;; i++) {
			if (LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE)) {
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
			if (LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE)) {
				break;
			}
		}
	}
#else
	for (i=0; ; i++)
	{
		if (LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE)) {
			break;
		}
	}
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

	*ch_p = (char) LL_USART_ReceiveData8(USARTx_INSTANCE);

	if (0 != _g_bsp_dtty_echo) {
		dtty_putc(*ch_p);
	}

	return 0;
}

int dtty_putc(int ch) {
	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

	if (0 != _g_bsp_dtty_autocr && '\n' == ch) {
		for (;;) {
			if (LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE)) {
				break;
			}
		}
		LL_USART_TransmitData8(USARTx_INSTANCE, '\r');
	}

	for (;;) {
		if (LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE)) {
			break;
		}
	}
	LL_USART_TransmitData8(USARTx_INSTANCE, ch);

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

int dtty_kbhit(void) {
	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

	if (LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE)) {
		return 1;
	} else {
		return 0;
	}
}

int dtty_flush(void)
{
    return 0;
}

#endif /* (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__UART) */
#endif /* (UBINOS__BSP__USE_DTTY == 1) */

#endif /* ((UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L476GEVAL) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOL476RG) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L475EIOT01)) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

