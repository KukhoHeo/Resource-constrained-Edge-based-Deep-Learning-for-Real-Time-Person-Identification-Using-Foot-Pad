/*
 * Copyright (c) 2019 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__CORTEX_MX == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA)

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)

#if !(INCLUDE__NRF5SDK == 1)
	#error "Project nrf5sdk is necessary to use RTC tick"
#endif

#include <assert.h>

#include <nrf.h>
#include <nrf_gpio.h>
#include <nrf_drv_rtc.h>
#include <nrf_drv_clock.h>
#ifdef SOFTDEVICE_PRESENT
#include <nrf_sdh.h>
#endif
#include <boards.h>
#include <app_error.h>

#if (UBINOS__UBIK__TICK_RTC_NO == 0)
	#define _TICK_RTC NRF_RTC0
	#define _TICK_RTC_IRQ_NO RTC0_IRQn
#elif (UBINOS__UBIK__TICK_RTC_NO == 1)
	#define _TICK_RTC NRF_RTC1
	#define _TICK_RTC_IRQ_NO RTC1_IRQn
#elif (UBINOS__UBIK__TICK_RTC_NO == 2)
	#define _TICK_RTC NRF_RTC2
	#define _TICK_RTC_IRQ_NO RTC2_IRQn
#else
	#error "Unsupported UBINOS__UBIK__TICK_RTC_NO"
#endif

int _ubik_inittick(void) {
	ret_code_t err_code;
    int i;
    unsigned int tickpersec = 0;
    unsigned int prescaler = 0;

    __disable_irq();

    _ubik_tickcount = 0;
    _ubik_tickcounth = 0;

#if (UBINOS__UBIK__TICK_RTC_CHECK == 1)
    _ubik_tickrtccount = 0;
    _ubik_tickrtccount_init = 0;
#endif

    tickpersec = ubik_gettickpersec();
    prescaler = (32768 / tickpersec) - 1;

	err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(err_code);

	nrf_drv_clock_lfclk_request(NULL);

	nrf_rtc_event_clear(_TICK_RTC, NRF_RTC_EVENT_TICK);
    nrf_rtc_event_clear(_TICK_RTC, NRF_RTC_EVENT_COMPARE_0);
	nrf_rtc_prescaler_set(_TICK_RTC, prescaler);

    // Set exception priority
    NVIC_SetPriorityGrouping(NVIC_PRIO_GROUP);
    for (i = NVIC_IRQN_START; i <= NVIC_IRQN_END; i++) {
        NVIC_SetPriority(i, NVIC_PRIO_LOWEST);
    }
    NVIC_SetPriority(SVCall_IRQn, NVIC_PRIO_SVC);
    NVIC_SetPriority(PendSV_IRQn, NVIC_PRIO_PENDSV);

    NVIC_SetPriority(_TICK_RTC_IRQ_NO, NVIC_PRIO_SYSTICK);
    NVIC_EnableIRQ(_TICK_RTC_IRQ_NO);

    nrf_rtc_int_enable(_TICK_RTC, NRF_RTC_INT_TICK_MASK);

	nrf_rtc_task_trigger(_TICK_RTC, NRF_RTC_TASK_CLEAR);
	nrf_rtc_task_trigger(_TICK_RTC, NRF_RTC_TASK_START);

    return 0;
}

void _ubik_tick_rtcisr_clear(void) {
	nrf_rtc_event_clear(_TICK_RTC, NRF_RTC_EVENT_TICK);
}

unsigned int _ubik_tick_rtccount_get(void) {
	return nrf_rtc_counter_get(_TICK_RTC);
}

#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)

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

#ifdef SOFTDEVICE_PRESENT
    if (nrf_sdh_is_enabled())
    {
        ret_code_t ret_code = sd_app_evt_wait();
        ASSERT((ret_code == NRF_SUCCESS) || (ret_code == NRF_ERROR_SOFTDEVICE_NOT_ENABLED));
        UNUSED_VARIABLE(ret_code);
    }
    else
#endif // SOFTDEVICE_PRESENT
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

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA) */
#endif /* (UBINOS__BSP__CORTEX_MX == 1) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


