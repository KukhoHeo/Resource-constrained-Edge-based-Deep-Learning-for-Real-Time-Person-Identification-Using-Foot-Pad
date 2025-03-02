/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32429IEVAL)

#include "../stm32f4/stm32f4xx_ll_bus.h"
#include "../stm32f4/stm32f4xx_ll_system.h"
#include "../stm32f4/stm32f4xx_ll_rcc.h"
#include "../stm32f4/stm32f4xx_ll_pwr.h"

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
__WEAK void SystemClock_Config(void) {
	/* Enable HSE oscillator */
	LL_RCC_HSE_Enable();
	while (LL_RCC_HSE_IsReady() != 1) {
	};

	/* Set FLASH latency */
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

	/* Enable PWR clock */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	/* Activation OverDrive Mode */
	LL_PWR_EnableOverDriveMode();
	while (LL_PWR_IsActiveFlag_OD() != 1) {
	};

	/* Activation OverDrive Switching */
	LL_PWR_EnableOverDriveSwitching();
	while (LL_PWR_IsActiveFlag_ODSW() != 1) {
	};

	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 360, LL_RCC_PLLP_DIV_2);
	LL_RCC_PLL_ConfigDomain_PLLQ(LL_RCC_PLLQ_DIV_7);
	LL_RCC_PLL_Enable();
	while (LL_RCC_PLL_IsReady() != 1) {
	};

	/* Sysclk activation on the main PLL */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
	};

	/* Set APB1 & APB2 prescaler */
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

	/* Set systick to 1ms */
	//SysTick_Config(180000000 / 1000);
	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	SystemCoreClock = (UBINOS__BSP__CLOCK_FREQ_MAIN * 1000);
}

__WEAK void MPU_Config(void) {
}

#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32429IEVAL) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

