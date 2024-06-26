/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL)
#if (UBINOS__BSP__BOARD_VARIATION__STM3221GEVAL == 1)

#include "main.h"

void gpio_init(void)
{
    static GPIO_InitTypeDef GPIO_InitStruct;

    /* -1- Enable GPIOG, GPIOC and GPIOI Clock (to be able to program the configuration registers) */
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    /* -2- Configure PG.6, PG.8, PI.9 and PC.7 IOs in output push-pull mode to
            drive external LEDs */
    GPIO_InitStruct.Pin = (GPIO_PIN_6 | GPIO_PIN_8);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void gpio_toggle(void)
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_6);
    /* Insert delay 100 ms */
    task_sleepms(100);
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_8);
    /* Insert delay 100 ms */
    task_sleepms(100);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_9);
    /* Insert delay 100 ms */
    task_sleepms(100);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
    /* Insert delay 100 ms */
    task_sleepms(100);
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

#endif /* (UBINOS__BSP__BOARD_VARIATION__STM3221GEVAL == 1) */
#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL) */

