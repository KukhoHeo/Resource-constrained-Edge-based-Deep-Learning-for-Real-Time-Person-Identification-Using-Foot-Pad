/*
 * Copyright (c) 2019 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__CORTEX_MX == 1)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBIK

int _ubik_port_comp_init(unsigned int idle_stackdepth) {
	int r = 0;

	r = _ubik_inittick();
	if (r != 0) {
		return -1;
	}

	r = task_comp_init(idle_stackdepth);
	if (r != 0) {
		return -1;
	}

	return 0;
}

void _ubik_port_comp_start(void) {
	task_comp_start();
}

int ubik_isactive(void) {
	return _bsp_kernel_active;
}

tickcount_t ubik_gettickcount(void) {
	tickcount_t tc;

	ubik_entercrit();

	tc.high	= _ubik_tickcounth;
	tc.low	= _ubik_tickcount;

	ubik_exitcrit();

	return tc;
}

tickcount_t ubik_gettickdiff(tickcount_t tick1, tickcount_t tick2) {
	tickcount_t diff;

	if (tick1.high > tick2.high) {
		diff.high = UBINOS__UBIK__TICK_COUNT_MAX - tick1.high + 1 + tick2.high;
	}
	else {
		diff.high = tick2.high - tick1.high;
	}

	if (tick1.low > tick2.low) {
		diff.high--;
		diff.low = UBINOS__UBIK__TICK_COUNT_MAX - tick1.low + 1 + tick2.low;
	}
	else {
		diff.low = tick2.low - tick1.low;
	}

	return diff;
}

void _task_stackinit(_task_pt task, void * arg) {
	unsigned int *	stacktop;
	unsigned int	stacklimit;

	stacktop 		= (unsigned int *) (((unsigned int) task->stack) + task->stacksize);
	stacklimit		= ((unsigned int) task->stack) + ARM_CONTEXT_SIZE + UBINOS__BSP__STACK_OVERFLOW_CHECK_MARGIN;

	stacktop        = (unsigned int *) (((unsigned int) stacktop) & (~(8 - 1)));    // 8 bytes alignment
	stacklimit      = ((stacklimit + (8 - 1)) & (~(8 - 1)));                        // 8 bytes alignment

	*(--stacktop) = ((unsigned int) 0x01000000);					/* xpsr					*/
	*(--stacktop) = ((unsigned int) _task_rootfunc) | 0x00000001;	/* r15 (pc)				*/
	*(--stacktop) = ((unsigned int) 0x00000000);					/* r14 (lr)				*/
	*(--stacktop) = ((unsigned int) 0x12121212);					/* r12					*/
	*(--stacktop) = ((unsigned int) 0x03030303);					/* r3 					*/
	*(--stacktop) = ((unsigned int) 0x02020202);					/* r2 					*/
	*(--stacktop) = ((unsigned int) 0x01010101);					/* r1 					*/
	*(--stacktop) = ((unsigned int) arg       );					/* r0 					*/

    *(--stacktop) = ((unsigned int) 0x00000000);                    /* _bsp_critcount       */

	*(--stacktop) = ((unsigned int) 0x11111111);					/* r11					*/
	*(--stacktop) = ((unsigned int) 0x10101010);					/* r10              	*/
	*(--stacktop) = ((unsigned int) 0x09090909);					/* r9					*/
	*(--stacktop) = ((unsigned int) 0x08080808);					/* r8 					*/
	*(--stacktop) = ((unsigned int) 0x07070707);					/* r7 					*/
	*(--stacktop) = ((unsigned int) 0x06060606);					/* r6 					*/
	*(--stacktop) = ((unsigned int) 0x05050505);					/* r5 					*/
	*(--stacktop) = ((unsigned int) 0x04040404);					/* r4 					*/

#if (__FPU_USED == 1)
	*(--stacktop) = ((unsigned int) 0x00000010);                    /* stack frame type     */
#endif

	task->stacktop = stacktop;
	task->stacktop_max = stacktop;
	task->stacklimit = (unsigned int *) stacklimit;

	return;
}

int task_getstacksize(task_pt _task, unsigned int * stacksize_p) {
	int r;
	_task_pt task = (_task_pt)_task;

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == stacksize_p) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (NULL == task && 0 == _bsp_kernel_active) {
		r = bsp_getstacksize(0, stacksize_p);
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || (OBJTYPE__UBIK_TASK != task->type && OBJTYPE__UBIK_IDLETASK != task->type)) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	*stacksize_p = task->stacksize;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_getmaxstackusage(task_pt _task, unsigned int * maxstackusage_p) {
	if (NULL != maxstackusage_p) {
		*maxstackusage_p = 0;
	}

	return -1;
}

int ubik_iscrit(void) {
	if (	(0 != _bsp_critcount) ||
			(0 != bsp_isintr()) 		) {
		return 1;
	}
	else {
		return 0;
	}
}

int ubik_istask(void) {
	if (	(0 != _bsp_kernel_active && 0 == bsp_isintr()) 	) {
		return 1;
	}
	else {
		return 0;
	}
}

int ubik_isrt(void) {
	if (	(0 != _bsp_critcount) ||
			(0 != bsp_isintr()) ||
			(0 != _ubik_tasklockcount) ||
			(0 != _bsp_kernel_active && NULL != _task_cur && _task_cur->priority >= _ubik_rtpriority)	) {
		return 1;
	}
	else {
		return 0;
	}
}

void _task_yield(void) {
	if (0 == _ubik_tasklockcount && 0 != _bsp_kernel_active) {
	    if (0 == bsp_isintr()) {
            if (0 == _bsp_critcount) {
                logme("not in critical section");
                bsp_abortsystem();
            }

#if (UBINOS__UBIK__USE_PENDSV_TASK_YIELD == 1)
	        arm_set_pendsv();
	        ARM_INTERRUPT_ENABLE();
	        while (arm_get_pendsv()) {
		        ARM_INTERRUPT_ENABLE();
	        }
#else
            __asm__ __volatile__ (
                "svc        %0                                              \n\t"
                :: "i" (SWINO__TASK_YIELD)
            );
#endif /* (UBINOS__UBIK__USE_PENDSV_TASK_YIELD == 1) */
	    }
	    else {
#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)
			_ubik_idle_cpu_wakeup();
#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */
	        arm_set_pendsv();
	    }
	}
}

unsigned int ubik_gettickpersec(void) {
    return UBINOS__UBIK__TICK_PER_SEC;
}

unsigned int ubik_timemstotick(unsigned int timems) {
    if (0 == timems) {
        return 0;
    }

#if     (0 == UBINOS__UBIK__TICK_PER_SEC%1000)
    timems = timems * (UBINOS__UBIK__TICK_PER_SEC / 1000);
#elif   (0 == UBINOS__UBIK__TICK_PER_SEC%100)
    timems = timems * (UBINOS__UBIK__TICK_PER_SEC  / 100) /   10;
#elif   (0 == UBINOS__UBIK__TICK_PER_SEC%10)
    timems = timems * (UBINOS__UBIK__TICK_PER_SEC   / 10) /  100;
#else
    timems = (unsigned int) ((unsigned long long) timems * UBINOS__UBIK__TICK_PER_SEC / 1000);
#endif

    if (0 == timems) {
        return 1;
    }

    return timems;
}

unsigned int ubik_ticktotimems(unsigned int tick) {
    if (0 == tick) {
        return 0;
    }

#if     (0 == UBINOS__UBIK__TICK_PER_SEC % 1000)
    tick = tick        / (UBINOS__UBIK__TICK_PER_SEC / 1000);
#elif   (0 == UBINOS__UBIK__TICK_PER_SEC % 100)
    tick = tick * 10   / (UBINOS__UBIK__TICK_PER_SEC  / 100);
#elif   (0 == UBINOS__UBIK__TICK_PER_SEC % 10)
    tick = tick * 100  / (UBINOS__UBIK__TICK_PER_SEC   / 10);
#else
    tick = (unsigned int) ((unsigned long long) tick * 1000 / UBINOS__UBIK__TICK_PER_SEC);
#endif

    if (0 == tick) {
        return 1;
    }

    return tick;
}

unsigned long ubik_tickcouttotimems(tickcount_t tc) {
    uint64_t ms;

    ms = (((uint64_t) tc.high) << (sizeof(tc.low) * 8)) + tc.low;
    ms = ms * 1000 / UBINOS__UBIK__TICK_PER_SEC;

    return (unsigned long) ms;
}

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#else

int _ubik_inittick(void) {
    int r;
    int i;
    unsigned int clockfreqk = 0;
    unsigned int counter = 0;
    unsigned int tickpersec = 0;

    __disable_irq();

    _ubik_tickcount = 0;
    _ubik_tickcounth = 0;

    r = bsp_getmckfreqk(&clockfreqk);
    if (0 != r) {
        return -1;
    }
    tickpersec = ubik_gettickpersec();
    counter = (clockfreqk * 1000 / tickpersec) - 1;

    if ((SysTick_LOAD_RELOAD_Msk - 1) < counter) {
        return -1;
    }

    // System Tick Configuration
    r = SysTick_Config(counter);
    if (0 != r) {
        return -1;
    }

    // Set exception priority
    NVIC_SetPriorityGrouping(NVIC_PRIO_GROUP);
    for (i = NVIC_IRQN_START; i <= NVIC_IRQN_END; i++) {
        NVIC_SetPriority(i, NVIC_PRIO_LOWEST);
    }
    NVIC_SetPriority(SVCall_IRQn, NVIC_PRIO_SVC);
    NVIC_SetPriority(PendSV_IRQn, NVIC_PRIO_PENDSV);
    NVIC_SetPriority(SysTick_IRQn, NVIC_PRIO_SYSTICK);

    return 0;
}

#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */

void _ubik_task_schedule_irq() {
    if (0 == _ubik_tasklockcount && 0 != _bsp_kernel_active) {
        if(_task_list_ready_index > _task_cur->priority) {
			arm_set_pendsv();
        }
    }
}

#endif /* (UBINOS__BSP__CORTEX_MX == 1) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


