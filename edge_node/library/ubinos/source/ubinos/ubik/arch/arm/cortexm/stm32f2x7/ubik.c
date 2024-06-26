/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__CORTEX_MX == 1)
#if ((UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32F217IG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32F207ZG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L476ZG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L476RG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L475VG))

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)

#include <ubinos/bsp_ubik.h>

#include <assert.h>

#if ((UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32F217IG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32F207ZG))

#include "stm32f2xx_hal.h"

#define UBIK_TICK__TIM_PRESCALER_VALUE ((SystemCoreClock / 2) / (UBINOS__UBIK__TICK_PER_SEC) / (UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK) - 1)
// SystemCoreClock = 120 Mhz
// TIM Clock = 120 Mhz / 2
// UBINOS__UBIK__TICK_PER_SEC = 1000
// UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK = 2
// UBIK_TICK__TIM_PRESCALER_VALUE = 60 MHz / 1000 / 2 - 1 = 29999 = 0x752F

#elif ((UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L476ZG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L476RG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L475VG))

#include "stm32l4xx_hal.h"

#define UBIK_TICK__TIM_PRESCALER_VALUE ((SystemCoreClock / 1) / (UBINOS__UBIK__TICK_PER_SEC) / (UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK) - 1)
// SystemCoreClock = 80 Mhz
// TIM Clock = 80 Mhz / 1
// UBINOS__UBIK__TICK_PER_SEC = 1000
// UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK = 2
// UBIK_TICK__TIM_PRESCALER_VALUE = 80 MHz / 1000 / 2 - 1 = 39999 = 0x9C3F

#else

#error "Unsupported UBINOS__BSP__CPU_MODEL"

#endif

#if (UBINOS__UBIK__TICK_RTC_NO == 5)
#define UBIK_TICK_TIM                    TIM5
#define UBIK_TICK_TIM_CLK_ENABLE()       __HAL_RCC_TIM5_CLK_ENABLE()
#define UBIK_TICK_TIM_IRQn               TIM5_IRQn
#define UBIK_TICK_TIM_IRQHandler         TIM5_IRQHandler
#define UBIK_TICK_TIM_CH1_PULSE          UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK
#define UBIK_TICK_TIM_CH2_PULSE          (UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK * 10)
#else
	#error "Unsupported UBINOS__UBIK__TICK_RTC_NO"
#endif

TIM_HandleTypeDef _ubik_tick_tim_handler;

int _ubik_inittick(void)
{
    int i;
    uint32_t uwPrescalerValue = 0;
    TIM_OC_InitTypeDef sConfig;

    __disable_irq();

    _ubik_tickcount = 0;
    _ubik_tickcounth = 0;

#if (UBINOS__UBIK__TICK_RTC_CHECK == 1)
    _ubik_tickrtccount = 0;
    _ubik_tickrtccount_init = 0;
#endif

    UBIK_TICK_TIM_CLK_ENABLE();

    uwPrescalerValue = (uint32_t) UBIK_TICK__TIM_PRESCALER_VALUE;

    _ubik_tick_tim_handler.Instance = UBIK_TICK_TIM;

    _ubik_tick_tim_handler.Init.Period = UBINOS__UBIK__TICK_RTC_COUNT_MAX;
    _ubik_tick_tim_handler.Init.Prescaler = uwPrescalerValue;
    _ubik_tick_tim_handler.Init.ClockDivision = 0;
    _ubik_tick_tim_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    _ubik_tick_tim_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_OC_Init(&_ubik_tick_tim_handler) != HAL_OK)
    {
        return -1;
    }

    // Tick interrupt
    sConfig.OCMode = TIM_OCMODE_TIMING;
    sConfig.Pulse = UBIK_TICK_TIM_CH1_PULSE;
    sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
    if (HAL_TIM_OC_ConfigChannel(&_ubik_tick_tim_handler, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        return -1;
    }

    // For solving the problem of missing the next tick interrupt due to delay in tick hander
    sConfig.OCMode = TIM_OCMODE_TIMING;
    sConfig.Pulse = UBIK_TICK_TIM_CH2_PULSE;
    sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
    if (HAL_TIM_OC_ConfigChannel(&_ubik_tick_tim_handler, &sConfig, TIM_CHANNEL_2) != HAL_OK)
    {
        return -1;
    }

    // Set exception priority
    NVIC_SetPriorityGrouping(NVIC_PRIO_GROUP);
    for (i = NVIC_IRQN_START; i <= NVIC_IRQN_END; i++) {
        NVIC_SetPriority(i, NVIC_PRIO_LOWEST);
    }
    NVIC_SetPriority(SVCall_IRQn, NVIC_PRIO_SVC);
    NVIC_SetPriority(PendSV_IRQn, NVIC_PRIO_PENDSV);

    NVIC_SetPriority(UBIK_TICK_TIM_IRQn, NVIC_PRIO_SYSTICK);
    NVIC_EnableIRQ(UBIK_TICK_TIM_IRQn);

    if (HAL_TIM_OC_Start_IT(&_ubik_tick_tim_handler, TIM_CHANNEL_1) != HAL_OK)
    {
        return -1;
    }
    if (HAL_TIM_OC_Start_IT(&_ubik_tick_tim_handler, TIM_CHANNEL_2) != HAL_OK)
    {
        return -1;
    }

    return 0;
}

void UBIK_TICK_TIM_IRQHandler(void)
{
    uint32_t uhCount = 0;
    uint32_t uhNextCh1 = 0;
    uint32_t uhNextCh2 = 0;

    bsp_ubik_tick_handler();

    uhCount = __HAL_TIM_GET_COUNTER(&_ubik_tick_tim_handler);

    uhNextCh1 = (uhCount + UBIK_TICK_TIM_CH1_PULSE) % UBINOS__UBIK__TICK_RTC_COUNT_MAX;
    uhNextCh2 = (uhCount + UBIK_TICK_TIM_CH2_PULSE) % UBINOS__UBIK__TICK_RTC_COUNT_MAX;

    __HAL_TIM_SET_COMPARE(&_ubik_tick_tim_handler, TIM_CHANNEL_1, (uhNextCh1));
    __HAL_TIM_SET_COMPARE(&_ubik_tick_tim_handler, TIM_CHANNEL_2, (uhNextCh2));
}

void _ubik_tick_rtcisr_clear(void)
{
    __HAL_TIM_CLEAR_IT(&_ubik_tick_tim_handler, TIM_IT_CC1);
    __HAL_TIM_CLEAR_IT(&_ubik_tick_tim_handler, TIM_IT_CC2);
}

unsigned int _ubik_tick_rtccount_get(void)
{
    return __HAL_TIM_GET_COUNTER(&_ubik_tick_tim_handler);
}

#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)

#error "Not yet supported"

void _ubik_idle_cpu_wakeup(void) {
#if (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1)
	if (nrf_rtc_int_is_enabled(_TICK_RTC, NRF_RTC_INT_COMPARE0_MASK)) {

		nrf_rtc_int_disable(_TICK_RTC, NRF_RTC_INT_COMPARE0_MASK);
		nrf_rtc_event_clear(_TICK_RTC, NRF_RTC_EVENT_COMPARE_0);

		nrf_rtc_int_enable(_TICK_RTC, RTC_INTENSET_TICK_Msk);

		__DSB();
	}
#endif /* (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1) */
}

static void __pwr_mgmt_fpu_sleep_prepare(void)
{
   uint32_t original_fpscr;

   original_fpscr = __get_FPSCR();
   /*
    * Clear FPU exceptions.
    * Without this step, the FPU interrupt is marked as pending,
    * preventing system from sleeping. Exceptions cleared:
    * - IOC - Invalid Operation cumulative exception bit.
    * - DZC - Division by Zero cumulative exception bit.
    * - OFC - Overflow cumulative exception bit.
    * - UFC - Underflow cumulative exception bit.
    * - IXC - Inexact cumulative exception bit.
    * - IDC - Input Denormal cumulative exception bit.
    */
   __set_FPSCR(original_fpscr & ~0x9Fu);
   __DMB();
   NVIC_ClearPendingIRQ(FPU_IRQn);

   /*
    * The last chance to indicate an error in FPU to the user
    * as the FPSCR is now cleared
    *
    * This assert is related to previous FPU operations
    * and not power management.
    *
    * Critical FPU exceptions signaled:
    * - IOC - Invalid Operation cumulative exception bit.
    * - DZC - Division by Zero cumulative exception bit.
    * - OFC - Overflow cumulative exception bit.
    */
   assert((original_fpscr & 0x7) == 0);
}

void _ubik_idle_cpu_sleep(void) {
#if (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1)
	_task_pt task = NULL;
	unsigned int next_wakeuptick = UBINOS__UBIK__TICK_COUNT_MAX;
	unsigned int ignore_tick_count = 0;
	unsigned int ignore_rtctick_count = 0;
	unsigned int next_wakeuprtctick = UBINOS__UBIK__TICK_RTC_COUNT_MAX;

	if (_ubik_tickrtccount_init && !nrf_rtc_int_is_enabled(_TICK_RTC, NRF_RTC_INT_COMPARE0_MASK)) {
		ubik_entercrit();

		task = _tasklist_head(_task_list_blocked_timed_cur);
		if (NULL != task) {
			next_wakeuptick = task->wakeuptick;
		}
#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
		_stimer_pt      stimer = NULL;
		stimer = _stimerlist_head(_stimer_list_cur);
		if (NULL != stimer) {
			if(next_wakeuptick > stimer->wakeuptick) {
				next_wakeuptick = stimer->wakeuptick;
			}
		}
#endif /* !(UBINOS__UBIK__EXCLUDE_STIMER == 1) */

		ignore_tick_count = next_wakeuptick - _ubik_tickcount;
		ignore_tick_count = min(ignore_tick_count, (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MAX / UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK));
		ignore_rtctick_count = ignore_tick_count * UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK;

		if (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MIN <= ignore_rtctick_count) {
			ignore_rtctick_count -= UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MARGIN;

			if ((UBINOS__UBIK__TICK_RTC_COUNT_MAX - _ubik_tickrtccount) >= ignore_rtctick_count) {
				next_wakeuprtctick = _ubik_tickrtccount + ignore_rtctick_count;
			}
			else {
				next_wakeuprtctick = ignore_rtctick_count - (UBINOS__UBIK__TICK_RTC_COUNT_MAX - _ubik_tickrtccount) - 1;
			}

			nrf_rtc_int_disable(_TICK_RTC, NRF_RTC_INT_TICK_MASK);
			nrf_rtc_event_clear(_TICK_RTC, NRF_RTC_EVENT_TICK);

			nrf_rtc_cc_set(_TICK_RTC, 0, next_wakeuprtctick);
			nrf_rtc_event_clear(_TICK_RTC, NRF_RTC_EVENT_COMPARE_0);
			nrf_rtc_int_enable(_TICK_RTC, NRF_RTC_INT_COMPARE0_MASK);

			__DSB();
		}

#if (__FPU_USED == 1)
		__pwr_mgmt_fpu_sleep_prepare();
#endif /* (__FPU_USED == 1) */

		ubik_exitcrit();
	}
	else {

#if (__FPU_USED == 1)
		ubik_entercrit();
		__pwr_mgmt_fpu_sleep_prepare();
		ubik_exitcrit();
#endif /* (__FPU_USED == 1) */

	}
#else /* (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1) */

#if (__FPU_USED == 1)
	ubik_entercrit();
	__pwr_mgmt_fpu_sleep_prepare();
	ubik_exitcrit();
#endif /* (__FPU_USED == 1) */

#endif /* (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1) */

    {
        // Wait for an event.
        __WFE();
        // Clear the internal event register.
        __SEV();
        __WFE();
    }
}

#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */

#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */

#endif /* ((UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32F217IG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32F207ZG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L476ZG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L476RG) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__STM32L475VG)) */
#endif /* (UBINOS__BSP__CORTEX_MX == 1) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

