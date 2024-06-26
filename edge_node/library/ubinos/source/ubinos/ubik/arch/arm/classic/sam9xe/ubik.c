/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM9XE512EK)

#if		((60 > UBINOS__UBIK__TICK_PER_SEC) || (10000 < UBINOS__UBIK__TICK_PER_SEC))
	#error "Unrecommended UBINOS__UBIK__TICK_PER_SEC"
#endif


#if !(UBINOS__UBIK__EXCLUDE_HRTICK == 1)

extern AT91S_TC * 	_ubik_hrtick_htimer_p;

#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK == 1) */


unsigned int ubik_gettickpersec(void) {
	return UBINOS__UBIK__TICK_PER_SEC;
}

unsigned int ubik_timemstotick(unsigned int timems) {
	if (0 == timems) {
		return 0;
	}

#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	timems = timems * (UBINOS__UBIK__TICK_PER_SEC / 1000);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	timems = timems * (UBINOS__UBIK__TICK_PER_SEC  / 100) /   10;
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	timems = timems * (UBINOS__UBIK__TICK_PER_SEC   / 10) /  100;
#else
	timems = timems * (UBINOS__UBIK__TICK_PER_SEC       ) / 1000;
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

#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	tick = tick        / (UBINOS__UBIK__TICK_PER_SEC / 1000);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	tick = tick * 10   / (UBINOS__UBIK__TICK_PER_SEC  / 100);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	tick = tick * 100  / (UBINOS__UBIK__TICK_PER_SEC   / 10);
#else
	tick = tick * 1000 / (UBINOS__UBIK__TICK_PER_SEC       );
#endif

	if (0 == tick) {
		return 1;
	}

	return tick;
}

void bsp_ubik_tick_handler(void) {
	unsigned int 	status = 0;
	edlist_pt		tempedlist;
	_task_pt		task;
	#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
	_stimer_pt		stimer;
	#endif /* !(UBINOS__UBIK__EXCLUDE_STIMER == 1) */
	_wtask_pt		wtask;
	_sigobj_pt 		sigobj;
	int				i;
	#if !(UBINOS__UBIK__EXCLUDE_HRTICK_TICKISR_DELAY_CHECK == 1)
	unsigned int hrtick;
	#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK_TICKISR_DELAY_CHECK == 1) */

	if (_bsp_kernel_active != 0) {

		status = AT91C_BASE_PITC->PITC_PISR;
		if (0 != (AT91C_PITC_PITS & status)) {

			////////////////

			status = AT91C_BASE_PITC->PITC_PIVR;

			////////////////

			_ubik_tickcount++;

			#if !(UBINOS__UBIK__EXCLUDE_HRTICK == 1)
			if (0 != _ubik_hrtick_active) {
				#if !(UBINOS__UBIK__EXCLUDE_HRTICK_TICKISR_DELAY_CHECK == 1)
				hrtick	= _ubik_hrtick_htimer_p->TC_CV;
				if (hrtick > (_ubik_hrtick_hrtickpertick * 2)) {
					dtty_puts("bsp_ubik_tick_handler: interrupt was delayed\n", 80);
					bsp_abortsystem();
				}
				#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK_TICKISR_DELAY_CHECK == 1) */

				// Reset and enable TC
				_ubik_hrtick_htimer_p->TC_CCR = (AT91C_TC_CLKEN | AT91C_TC_SWTRG);
			}
			#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK == 1) */

			////////////////

			if (0 == _ubik_tickcount) {
				_ubik_tickcounth++;

				tempedlist						= _task_list_blocked_timed_cur;
				_task_list_blocked_timed_cur	= _task_list_blocked_timed_next;
				_task_list_blocked_timed_next	= tempedlist;

				#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
				tempedlist						= _stimer_list_cur;
				_stimer_list_cur				= _stimer_list_next;
				_stimer_list_next				= tempedlist;
				#endif
			}

			for (;;) {
				task = _tasklist_head(_task_list_blocked_timed_cur);
				if (NULL != task && _ubik_tickcount >= task->wakeuptick) {
					for (i=0; i<task->wtask_count; i++) {
						wtask = &task->wtask_p[i];
						sigobj = _sigobj_wtasklist_owner(wtask);
						if (NULL != sigobj) {
							wtask->sigtype	= SIGOBJ_SIGTYPE__TIMEOUT;
							task->wtask_recvcount++;
							_sigobj_wtasklist_remove(wtask);
							_sigobj_wtasklist_notifychange(sigobj);
						}
					}
					task->state				= TASK_STATE__READY;
					task->waittick			= 0;
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

			////////////////

			#if !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1)
			if (NULL != _ubik_tickhookfunc_func) {
				_ubik_tickhookfunc_func();
			}
			#endif /* !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1) */

			////////////////

			if (0 == _ubik_tasklockcount) {
				_task_prev 			= _task_cur;
				_task_cur->state	= TASK_STATE__READY;
				_task_cur			= _tasklist_getcurnext(_task_list_ready_cur);
				_task_cur->state	= TASK_STATE__RUNNING;
			}

			//////////////////

		}
	}
}

int _ubik_inittick(void) {
	int r;
	unsigned int clockfreqk = 0;
	unsigned int counter = 0;
	unsigned int tickpersec = 0;

	_ubik_tickcount = 0;
	_ubik_tickcounth = 0;

	r = bsp_getmckfreqk(&clockfreqk);
	if (0 != r) {
		return -1;
	}
	tickpersec = ubik_gettickpersec();
	counter = ((clockfreqk * 1000) / 16 / tickpersec) - 1;

	r = intr_connectisr(AT91C_ID_SYS, bsp_ubik_tick_handler, intr_gethighestpriority(), INTR_OPT__LEVEL | INTR_OPT__HIGH);
	if (0 != r) {
		return -1;
	}

	/* Configure the PIT period. */
	AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | counter;

	r = intr_enable(AT91C_ID_SYS);
	if (0 != r) {
		return -1;
	}

	return 0;
}


#endif /* (UBINOS__BSP__BOARD_MODEL == ...) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


