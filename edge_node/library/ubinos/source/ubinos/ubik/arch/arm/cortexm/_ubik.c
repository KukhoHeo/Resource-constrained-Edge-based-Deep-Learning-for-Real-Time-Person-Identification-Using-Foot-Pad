/*
 * Copyright (c) 2019 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__CORTEX_MX == 1)

#include <assert.h>

void ubik_entercrit(void) {
	if (0 == bsp_isintr()) {
        ARM_INTERRUPT_DISABLE();
        _bsp_critcount++;
	}
    else
    {
        ARM_INTERRUPT_DISABLE();
        _bsp_critcount_in_isr++;
    }
}

void ubik_exitcrit(void) {
	if (0 == bsp_isintr()) {
        if (0 == _bsp_critcount) {
            dtty_puts("\nubik_exitcrit fail (_bsp_critcount is already 0)\n", 80);
            bsp_abortsystem();
        }
        _bsp_critcount--;
        if (0 == _bsp_critcount) {
            ARM_INTERRUPT_ENABLE();
        }
	}
    else
    {
        if (0 == _bsp_critcount_in_isr) {
            dtty_puts("\nubik_exitcrit fail (_bsp_critcount_in_isr is already 0)\n", 80);
            bsp_abortsystem();
        }
        _bsp_critcount_in_isr--;
        if (0 == _bsp_critcount_in_isr) {
            ARM_INTERRUPT_ENABLE();
        }
    }
}

#if (__FPU_USED == 1)

#define save_context() {                                                    \
    __asm__ __volatile__ (                                                  \
        "mov        r3, %0                                          \n\t"   \
        "msr        basepri, r3                                     \n\t"   \
        /* Set r3 to psp */                                                 \
        "mrs        r3, psp                                         \n\t"   \
        /* Save _bsp_critcount into psp stack */                            \
        "ldr        r2, =_bsp_critcount                             \n\t"   \
        "ldr        r2, [r2]                                        \n\t"   \
        "stmdb      r3!, {r2}                                       \n\t"   \
        /* Save r4-r11 into psp stack */                                    \
        "stmdb      r3!, {r4-r11}                                   \n\t"   \
        /* Save FPU registers and stack frame type */                       \
        "and        r2, lr, #0x10                                   \n\t"   \
        "cmp        r2, #0                                          \n\t"   \
        "it         eq                                              \n\t"   \
        "vstmdbeq   r3!, {s16-s31}                                  \n\t"   \
        "stmdb      r3!, {r2}                                       \n\t"   \
        /* Save r3(psp) to _task_cur->stacktop */                           \
        "ldr        r2, =_task_cur                                  \n\t"   \
        "ldr        r2, [r2]                                        \n\t"   \
        "str        r3, [r2, #4]                                    \n\t"   \
        :: "i" (NVIC_BASEPRI_REAL)                                          \
    );                                                                      \
}
#define restore_context() {                                                 \
    __asm__ __volatile__ (                                                  \
        /* Restore r3(psp) from _task_cur->stacktop */                      \
        "ldr        r2, =_task_cur                                  \n\t"   \
        "ldr        r2, [r2]                                        \n\t"   \
        "ldr        r3, [r2, #4]                                    \n\t"   \
        /* Restore FPU registers and stack frame type */                    \
        "ldmia      r3!, {r2}                                       \n\t"   \
        "cmp        r2, #0                                          \n\t"   \
        "itte       eq                                              \n\t"   \
        "vldmiaeq   r3!, {s16-s31}                                  \n\t"   \
        "bfceq      lr, #4, #1                                      \n\t"   \
        "orrne      lr, r2                                          \n\t"   \
        /* Restore r4-r11 from psp stack */                                 \
        "ldmia      r3!, {r4-r11}                                   \n\t"   \
        /* Restore _bsp_critcount from psp stack */                         \
        "ldmia      r3!, {r2}                                       \n\t"   \
        "ldr        r1, =_bsp_critcount                             \n\t"   \
        "str        r2, [r1]                                        \n\t"   \
        /* Set r3 to psp */                                                 \
        "msr        psp, r3                                         \n\t"   \
        /* Restore basepri from _bsp_critcount */                           \
        "cmp        r2, #0                                          \n\t"   \
        "ite        eq                                              \n\t"   \
        "moveq      r3, #0                                          \n\t"   \
        "movne      r3, %0                                          \n\t"   \
        "msr        basepri, r3                                     \n\t"   \
        :: "i" (NVIC_BASEPRI_REAL)                                          \
    );                                                                      \
}

#else

#define save_context() {                                                    \
    __asm__ __volatile__ (                                                  \
        "mov        r3, %0                                          \n\t"   \
        "msr        basepri, r3                                     \n\t"   \
        /* Set r3 to psp */                                                 \
        "mrs        r3, psp                                         \n\t"   \
        /* Save _bsp_critcount into psp stack */                            \
        "ldr        r2, =_bsp_critcount                             \n\t"   \
        "ldr        r2, [r2]                                        \n\t"   \
        "stmdb      r3!, {r2}                                       \n\t"   \
        /* Save r4-r11 into psp stack */                                    \
        "stmdb      r3!, {r4-r11}                                   \n\t"   \
        /* Save r3(psp) to _task_cur->stacktop */                           \
        "ldr        r2, =_task_cur                                  \n\t"   \
        "ldr        r2, [r2]                                        \n\t"   \
        "str        r3, [r2, #4]                                    \n\t"   \
        :: "i" (NVIC_BASEPRI_REAL)                                          \
    );                                                                      \
}
#define restore_context() {                                                 \
    __asm__ __volatile__ (                                                  \
        /* Restore r3(psp) from _task_cur->stacktop */                      \
        "ldr        r2, =_task_cur                                  \n\t"   \
        "ldr        r2, [r2]                                        \n\t"   \
        "ldr        r3, [r2, #4]                                    \n\t"   \
        /* Restore r4-r11 from psp stack */                                 \
        "ldmia      r3!, {r4-r11}                                   \n\t"   \
        /* Restore _bsp_critcount from psp stack */                         \
        "ldmia      r3!, {r2}                                       \n\t"   \
        "ldr        r1, =_bsp_critcount                             \n\t"   \
        "str        r2, [r1]                                        \n\t"   \
        /* Set r3 to psp */                                                 \
        "msr        psp, r3                                         \n\t"   \
        /* Restore basepri from _bsp_critcount */                           \
        "cmp        r2, #0                                          \n\t"   \
        "ite        eq                                              \n\t"   \
        "moveq      r3, #0                                          \n\t"   \
        "movne      r3, %0                                          \n\t"   \
        "msr        basepri, r3                                     \n\t"   \
        :: "i" (NVIC_BASEPRI_REAL)                                          \
    );                                                                      \
}

#endif

void __attribute__((naked)) bsp_ubik_psv_handler(void) {
    save_context();
    __asm__ __volatile__ (
        "push       {lr}                                            \n\t"
    );

	_task_prev = _task_cur;
	if(TASK_STATE__RUNNING == _task_cur->state) {
		_task_cur->state = TASK_STATE__READY;
	}
	_task_cur = _tasklist_getcurnext(_task_list_ready_cur);
	_task_cur->state = TASK_STATE__RUNNING;

    __asm__ __volatile__ (
        "pop        {lr}                                            \n\t"
    );
    restore_context();
    __asm__ __volatile__ (
        "bx         lr                                              \n\t"
    );
}

void bsp_ubik_swi_handler(void) {
    unsigned int swino;

    __asm__ __volatile__ (
        /* Set r0 to task stack */
        "tst        lr, #4                                          \n\t"
        "ite        eq                                              \n\t"
        "mrseq      r0, msp                                         \n\t"
        "mrsne      r0, psp                                         \n\t"
        /* Set r0 to swino */
        "ldr        r0, [r0, #24]                                   \n\t"
        "subs       r0, r0, #2                                      \n\t"
        "ldrb       r0, [r0]                                        \n\t"
        "mov        %0, r0                                          \n\t"
        : "=r" (swino)
    );

    switch(swino) {
	case SWINO__TASK_YIELD:
        if (0 == _ubik_tasklockcount && 0 != _bsp_kernel_active) {
			arm_set_pendsv();
        }
		break;
	default:
		break;
	}
}

void bsp_ubik_tick_handler(void) {
#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
	#if (UBINOS__UBIK__TICK_RTC_CHECK == 1)
	unsigned int tickrtccount;
	unsigned int tickcount_diff;
	#endif /* (UBINOS__UBIK__TICK_RTC_CHECK == 1) */
#else
    unsigned int    status = 0;
#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */
    edlist_pt       tempedlist;
    _task_pt        task;
#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
    _stimer_pt      stimer;
#endif /* !(UBINOS__UBIK__EXCLUDE_STIMER == 1) */
    _wtask_pt       wtask;
    _sigobj_pt      sigobj;
    int             j;

////////
#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)

	_ubik_tick_rtcisr_clear();

#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)
	_ubik_idle_cpu_wakeup();
#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */

	if (_bsp_kernel_active != 0) {

	////////
	#if (UBINOS__UBIK__TICK_RTC_CHECK == 1)

		tickrtccount = _ubik_tick_rtccount_get();

		if (_ubik_tickrtccount_init) {
			if (_ubik_tickrtccount <= tickrtccount) {
				tickcount_diff = (tickrtccount - _ubik_tickrtccount) / UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK;
			}
			else {
				tickcount_diff = (tickrtccount + (UBINOS__UBIK__TICK_RTC_COUNT_MAX - _ubik_tickrtccount) + 1) / UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK;
			}
		}
		else {
			tickcount_diff = 1;
			_ubik_tickrtccount_init = 1;
		}

		_ubik_tickrtccount = tickrtccount;

		////////
		#if (UBINOS__UBIK__TICK_RTC_CHECK_TYPE == UBINOS__UBIK__TICK_RTC_CHECK_TYPE__CORRECT)

		while(tickcount_diff > 0) {

			////////////////

			ARM_INTERRUPT_DISABLE();

			if (_ubik_tickcount == UBINOS__UBIK__TICK_COUNT_MAX) {
				tickcount_diff -= 1;
				_ubik_tickcount = 0;
			}
			else {
				if ((_ubik_tickcount + tickcount_diff) < _ubik_tickcount) {
					tickcount_diff -= (UBINOS__UBIK__TICK_COUNT_MAX - _ubik_tickcount);
					_ubik_tickcount = UBINOS__UBIK__TICK_COUNT_MAX;
				}
				else {
					_ubik_tickcount += tickcount_diff;
					tickcount_diff = 0;
				}
			}

		#elif (UBINOS__UBIK__TICK_RTC_CHECK_TYPE == UBINOS__UBIK__TICK_RTC_CHECK_TYPE__ABORT)

		if (tickcount_diff != 1) {
			dtty_puts("\nrtc tick check fail\n", 80);
			bsp_abortsystem();
		}

		{

			////////////////

			ARM_INTERRUPT_DISABLE();

			_ubik_tickcount++;

		#else /* (UBINOS__UBIK__TICK_RTC_CHECK_TYPE == UBINOS__UBIK__TICK_RTC_CHECK_TYPE__...) */

			#error "Unsupported UBINOS__UBIK__TICK_RTC_CHECK_TYPE"

		#endif /* (UBINOS__UBIK__TICK_RTC_CHECK_TYPE == UBINOS__UBIK__TICK_RTC_CHECK_TYPE__...) */
		////////

	#else /* (UBINOS__UBIK__TICK_RTC_CHECK == ...) */

		{

			////////////////

			ARM_INTERRUPT_DISABLE();

			_ubik_tickcount++;

	#endif /* (UBINOS__UBIK__TICK_RTC_CHECK == ...) */
	////////

#else /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__...) */

	if (_bsp_kernel_active != 0) {

		status = SysTick->CTRL;
		if (0 != (0x00010000 & status)) {

			////////////////

			ARM_INTERRUPT_DISABLE();

			_ubik_tickcount++;

#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__...) */
////////

			if (0 == _ubik_tickcount) {
				_ubik_tickcounth++;

				tempedlist                      = _task_list_blocked_timed_cur;
				_task_list_blocked_timed_cur    = _task_list_blocked_timed_next;
				_task_list_blocked_timed_next   = tempedlist;

				#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
				tempedlist                      = _stimer_list_cur;
				_stimer_list_cur                = _stimer_list_next;
				_stimer_list_next               = tempedlist;
				#endif
			}

			for (;;) {
				task = _tasklist_head(_task_list_blocked_timed_cur);
				if (NULL != task && _ubik_tickcount >= task->wakeuptick) {
					for (j=0; j<task->wtask_count; j++) {
						wtask = &task->wtask_p[j];
						sigobj = _sigobj_wtasklist_owner(wtask);
						if (NULL != sigobj) {
							wtask->sigtype  = SIGOBJ_SIGTYPE__TIMEOUT;
							task->wtask_recvcount++;
							_sigobj_wtasklist_remove(wtask);
							_sigobj_wtasklist_notifychange(sigobj);
						}
					}
					task->state             = TASK_STATE__READY;
					task->waittick          = 0;
					_task_changelist(task);
				}
				else {
					break;
				}
			}

#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
			for (;;) {
				stimer = _stimerlist_head(_stimer_list_cur);
				if (NULL != stimer && _ubik_tickcount >= stimer->wakeuptick) {
					switch (stimer->sigobj->type) {
					case OBJTYPE__UBIK_SEM:
						sem_give((sem_pt) stimer->sigobj);
						if (0 == stimer->oneshot) {
							_stimerlist_add(stimer);
						}
						else {
							_stimerlist_remove(stimer);
							stimer->running = 0;
						}
						break;
					case OBJTYPE__UBIK_SIGNAL:
						if (0 == stimer->broadcast) {
							signal_send((signal_pt) stimer->sigobj, stimer->sigtype);
						} else {
							signal_broadcast((signal_pt) stimer->sigobj, stimer->sigtype);
						}
						if (0 == stimer->oneshot) {
							_stimerlist_add(stimer);
						}
						else {
							_stimerlist_remove(stimer);
							stimer->running = 0;
						}
						break;
					default:
						_stimerlist_remove(stimer);
						stimer->running = 0;
						break;
					}
				}
				else {
					break;
				}
			}
#endif /* !(UBINOS__UBIK__EXCLUDE_STIMER == 1) */

			ARM_INTERRUPT_ENABLE();

			////////////////

#if !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1)
			if (NULL != _ubik_tickhookfunc_func) {
				_ubik_tickhookfunc_func();
			}
#endif /* !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1) */

			////////////////

			if (0 == _ubik_tasklockcount) {
				arm_set_pendsv();
			}

			//////////////////
		}
	}
}

void __attribute__((naked)) bsp_ubik_irq_handler(void) {
    __asm__ __volatile__ (
        "mrs    r0, ipsr                \n\t" // get exception number
        "tst    lr, #4                  \n\t" // test bit 2 of EXC_RETURN
        "ite    eq                      \n\t"
        "mrseq  r1, msp                 \n\t" // if 0, stacking used msp, copy to r0
        "mrsne  r1, psp                 \n\t" // if 1, stacking used psp, copy to r0
        "b      bsp_ubik_irq_handler_c  \n\t"
    );
}

void bsp_ubik_irq_handler_c(unsigned int exceptiontype, unsigned int * prev_sp) {
#if (UBINOS__BSP__NRF52_IGNORE_HARDFAULT_EXCEPTION_FROM_SOFTDEVICE == 1)
    uint16_t prev_insturction;

    if (exceptiontype == 3) // hard fault
    {
        prev_insturction = *((uint16_t *) &(((char *)prev_sp[6])[-2]));
        if (prev_insturction == 0xDFFF) // svc 0xff
        {
            // ignore
            return;
        }
    }
#endif /* (UBINOS__BSP__NRF52_IGNORE_HARDFAULT_EXCEPTION_FROM_SOFTDEVICE == 1) */

    dtty_puts("\n\nunknown exception (", 80);
    switch (exceptiontype) {
    default:
    	if (exceptiontype >= 100) {
            dtty_putc((int) ((exceptiontype / 100 % 10) + '0'));
    	}
    	if (exceptiontype >= 10) {
            dtty_putc((int) ((exceptiontype / 10 % 10) + '0'));
    	}
    	dtty_putc((int) ((exceptiontype % 10) + '0'));
        break;
    }
    dtty_puts(")\n", 80);
    bsp_abortsystem();
}


void task_comp_start(void) {
    unsigned long control;
    (void) control;

    /* Disable interrupt */
    __disable_irq();
    __set_BASEPRI(0);

    // Make sure in thread (not in ISR).
    assert((__get_IPSR() & IPSR_ISR_Msk) == 0);

#if (__FPU_USED == 1)
    // Clear FPCA
    control = __get_CONTROL();
    control &= ~CONTROL_FPCA_Msk;
    __set_CONTROL(control);
#endif

    control = __get_CONTROL();
    // Make sure in privledged.
    assert((control & CONTROL_nPRIV_Msk) == 0);
    // Make sure using MSP stack.
    assert((control & CONTROL_SPSEL_Msk) == 0);
#if (__FPU_USED == 1)
    // Make sure FP extension is not active.
    assert((control & CONTROL_FPCA_Msk) == 0);
#endif
    // Make sure that interrupt is disabled.
    assert(__get_PRIMASK() == 1);

    _bsp_kernel_active = 1;

    _task_cur->state = TASK_STATE__RUNNING;

    __asm__ __volatile__ (
        /* Restore r3(psp) from _task_cur->stacktop */
        "ldr        r2, =_task_cur                                  \n\t"
        "ldr        r2, [r2]                                        \n\t"
        "ldr        r3, [r2, #4]                                    \n\t"
#if (__FPU_USED == 1)
        /* Restore FPU registers and stack frame type */
        "ldmia      r3!, {r2}                                       \n\t"
        "cmp        r2, #0                                          \n\t"
        "itte       eq                                              \n\t"
        "vldmiaeq   r3!, {s16-s31}                                  \n\t"
        "bfceq      lr, #4, #1                                      \n\t"
        "orrne      lr, r2                                          \n\t"
#endif
        /* Restore r4-r11 from psp stack */
        "ldmia      r3!, {r4-r11}                                   \n\t"
        /* Restore _bsp_critcount from psp stack */
        "ldmia      r3!, {r2}                                       \n\t"
        "ldr        r1, =_bsp_critcount                             \n\t"
        "str        r2, [r1]                                        \n\t"
        /* Set r3 to psp */
        "msr        psp, r3                                         \n\t"

        /* Use psp */
        "mrs        r3, control                                     \n\t"
        "orr        r3, r3, #2                                      \n\t"
        "msr        control, r3                                     \n\t"
        "isb                                                        \n\t"
        /* Reset msp */
        "ldr        r3, =__stack                                    \n\t"
        "msr        msp, r3                                         \n\t"

        /* Pop r0~r3, r12, lr, xpsr */
        "ldr        r0, [sp, #28]                                   \n\t"
        "msr        apsr_nzcvq , r0                                 \n\t"
        "ldr        r0, [sp, #24]                                   \n\t"
        "str        r0, [sp, #28]                                   \n\t"
        "pop        {r0-r3, r12, lr}                                \n\t"
        "add        sp, sp, #4                                      \n\t"

        /* Enable interrupt and pop pc */
        "cpsie      i                                               \n\t"
        "pop        {pc}                                            \n\t"
    );
}

#endif /* (UBINOS__BSP__CORTEX_MX == 1) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


