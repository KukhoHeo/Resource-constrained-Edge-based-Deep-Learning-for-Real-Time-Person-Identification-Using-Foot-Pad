/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32VLDISCO)

#include "../stm32f1/stm32f1xx_ll_bus.h"
#include "../stm32f1/stm32f1xx_ll_system.h"
#include "../stm32f1/stm32f1xx_ll_rcc.h"
#include "../stm32f1/stm32f1xx_ll_pwr.h"

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 24000000
  *            HCLK(Hz)                       = 24000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
 */
__WEAK void SystemClock_Config(void) {
  /* Enable HSE oscillator */
  LL_RCC_HSE_Enable();
#if (UBINOS__BSP__USE_QEMU != 1)
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };
#endif /* (UBINOS__BSP__USE_QEMU != 1) */

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_2, LL_RCC_PLL_MUL_6);
  LL_RCC_PLL_Enable();
#if (UBINOS__BSP__USE_QEMU != 1)
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
#endif /* (UBINOS__BSP__USE_QEMU != 1) */

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
#if (UBINOS__BSP__USE_QEMU != 1)
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
#endif /* (UBINOS__BSP__USE_QEMU != 1) */

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms */
  //SysTick_Config(24000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  //SystemCoreClock = 24000000;
  SystemCoreClock = (UBINOS__BSP__CLOCK_FREQ_MAIN * 1000);
}

__WEAK void MPU_Config(void) {
}

#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32VLDISCO) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

