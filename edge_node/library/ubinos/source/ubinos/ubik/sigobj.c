/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)
#if (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1)
#include <ubinos/bsp_ubik.h>
#endif /* (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

int _sigobj_create(_sigobj_pt * sigobj_p) {
	int r;
	_sigobj_pt sigobj;

	assert(sigobj_p != NULL);

	if (0 != bsp_isintr()) {
		logme("in interrupt");
		r = -1;
		goto end0;
	}

	if (0 != _bsp_critcount) {
		logme("in critical section");
		r = -1;
		goto end0;
	}

	if (NULL == sigobj_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	sigobj = malloc(sizeof(_sigobj_t));
	if (NULL == sigobj) {
		logme("fail at malloc()");
		r = -1;
		goto end0;
	}

	sigobj->type					= 0;
	sigobj->valid					= 0;
	sigobj->reserved1				= 0;

	sigobj->reserved2				= 0;
	sigobj->nopriorityinheritance	= 0;
	sigobj->ignoreoverflow			= 0;
	sigobj->msgsize_1				= 0;
	sigobj->wtasklist_maxpriority	= 0;

	edlist_init(&sigobj->wtasklist);
	sigobj->wtasklist.data			= sigobj;

	edlist_link_init(&sigobj->osigobjlist_link);

	sigobj->count					= 0;
	sigobj->maxcount				= UINT_MAX;
	sigobj->msgbuf					= NULL;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	edlist_link_init(&sigobj->kernel_monitor_sigobjlist_link);
	#endif

	*sigobj_p = sigobj;

	r = 0;

end0:
	return r;
}

int _sigobj_delete(_sigobj_pt * sigobj_p) {
	int r;
	int r2;
	_sigobj_pt sigobj;
	cirbuf_pt msgbuf = NULL;
	void * sigobjbuf = NULL;
	_task_pt owner;

	assert(sigobj_p != NULL);
	assert(*sigobj_p != NULL);

	if (0 != bsp_isintr()) {
		logme("in interrupt");
		r = -1;
		goto end0;
	}

	if (0 != _bsp_critcount) {
		logme("in critical section");
		r = -1;
		goto end0;
	}

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == sigobj_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	sigobj = *sigobj_p;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}
	#endif

	ubik_entercrit();

	if (	(	NULL == sigobj								)	||
			(	0 == sigobj->valid							) 	||
			(	OBJTYPE__UBIK_SIGNAL	!= sigobj->type	&&
				OBJTYPE__UBIK_CONDV 	!= sigobj->type &&
				OBJTYPE__UBIK_MUTEX 	!= sigobj->type	&&
				OBJTYPE__UBIK_SEM 		!= sigobj->type	&&
				OBJTYPE__UBIK_MSGQ 		!= sigobj->type		)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	owner = _task_osigobjlist_owner(sigobj);
	if (NULL != owner) {
		if (0 != _bsp_kernel_active && 0 == bsp_isintr() && owner == _task_cur) {
			r = _sigobj_setsender(sigobj, NULL);
			if (0 != r) {
				logme("fail at _sigobj_setsender()");
				r = -1;
				goto end1;
			}
		}
		else {
			logme("still owned");
			r = -2;
			goto end1;
		}
	}

	if (0 == _ubik_tasklockcount) {
		_ubik_tasklockcount++;
		r = _sigobj_broadcast(sigobj, SIGOBJ_SIGTYPE__TERMINATED);
		_ubik_tasklockcount--;
	}
	else {
		r = _sigobj_broadcast(sigobj, SIGOBJ_SIGTYPE__TERMINATED);
	}
	if (0 != r) {
		logme("fail at _sigobj_broadcast()");
		r = -2;
		goto end1;
	}

	msgbuf 			= sigobj->msgbuf;
	sigobjbuf 		= sigobj;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_sigobjlist_remove(sigobj);
	#endif

	sigobj->valid 	= 0;

	*sigobj_p 		= NULL;

	_task_schedule();

	r = 0;

end1:
	ubik_exitcrit();

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}
	#endif

	if (NULL != msgbuf) {
		r2 = cirbuf_delete(&msgbuf);
		if (0 != r2) {
			logme("fail at cirbuf_delete()");
			r = -1;
		}
	}

	if (NULL != sigobjbuf) {
		free(sigobjbuf);
	}

end0:
	return r;
}

int _sigobj_wait(_sigobj_pt sigobj, _wtask_pt wtask) {
	int r;
	unsigned int critcount;
	unsigned int spintick;

	assert(sigobj != NULL);
	assert(wtask != NULL);

	if (0 >= _bsp_critcount) {
		logme("not in critical section");
		bsp_abortsystem();
	}

	if (0 != _task_cur->timed) {
		if (0 == _task_cur->waittick) {
			r = SIGOBJ_SIGTYPE__TIMEOUT;
			goto end0;
		}
		_task_cur->wakeuptick = _ubik_tickcount + _task_cur->waittick;
	}

	_task_cur->wtask_count		= 1;
	_task_cur->wtask_waitcount	= 1;
	_task_cur->wtask_recvcount 	= 0;

	wtask->sigtype = SIGOBJ_SIGTYPE__FAIL;

	_sigobj_wtasklist_insertprev(sigobj, NULL, wtask);
	_sigobj_wtasklist_notifychange(sigobj);

	if (OBJTYPE__UBIK_IDLETASK  == _task_cur->type) {
		for (;;) {
			if (0 != _task_cur->timed) {
				if (_task_cur->waittick <= 0) {
					wtask->sigtype = SIGOBJ_SIGTYPE__TIMEOUT;
					_task_cur->wtask_recvcount++;
					_sigobj_wtasklist_remove(wtask);
					_sigobj_wtasklist_notifychange(sigobj);

					_task_cur->waittick = 0;
					break;
				}
				spintick = min(UBINOS__UBIK__IDLETASK_SPINWAIT_INTERVALTICK, _task_cur->waittick);
			}
			else {
				spintick = UBINOS__UBIK__IDLETASK_SPINWAIT_INTERVALTICK;
			}

			critcount = _bsp_critcount;
			_bsp_critcount = 1;
			ubik_exitcrit();
			bsp_busywait(bsp_getbusywaitcountperms() * ubik_ticktotimems(spintick));
			ubik_entercrit();
			_bsp_critcount = critcount;

			if (0 != _task_cur->timed) {
				_task_cur->waittick -= spintick;
			}

			if (SIGOBJ_SIGTYPE__FAIL != wtask->sigtype) {
				break;
			}
		}
	}
	else {
		_task_cur->state =  TASK_STATE__BLOCKED;
		_task_changelist(_task_cur);

		_task_yield();
	}

	_task_cur->wtask_count		= 0;

	r = wtask->sigtype;

end0:
	return r;
}

int _sigobj_send(_sigobj_pt sigobj, int sigtype) {
	_wtask_pt wtask;
	_task_pt task;

	assert(sigobj != NULL);

	wtask = _sigobj_wtasklist_gettask(sigobj);
	if (NULL != wtask) {
#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)
#if (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1)
	    if (0 != _bsp_kernel_active) {
	        if (0 != bsp_isintr()) {
	            bsp_ubik_tick_handler();
	        }
	    }
#endif /* (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */

		task = wtask->task;

		/////////////
		wtask->sigtype = sigtype;
		task->wtask_recvcount++;
		_sigobj_wtasklist_remove(wtask);

		if (task->wtask_waitcount == task->wtask_recvcount) {
			task->state = TASK_STATE__READY;
			_task_recalculate_waittick(task);
			_task_changelist(task);
		}
		else if (0 == task->waitall) {
			_task_sigobj_removewtask(task);

			task->state = TASK_STATE__READY;
			_task_recalculate_waittick(task);
			_task_changelist(task);
		}
		////////////////

		_sigobj_wtasklist_notifychange(sigobj);

		_task_schedule();
	}

	return 0;
}

int _sigobj_broadcast(_sigobj_pt sigobj, int sigtype) {
	_wtask_pt wtask;
	_wtask_pt wtask_next;
	_task_pt task;

	assert(sigobj != NULL);

	wtask = _sigobj_wtasklist_head(sigobj);
	if (NULL != wtask) {
#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)
#if (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1)
        if (0 != _bsp_kernel_active) {
            if (0 != bsp_isintr()) {
                bsp_ubik_tick_handler();
            }
        }
#endif /* (UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */
		for (;;) {
			wtask_next = _sigobj_wtasklist_next(wtask);

			task = wtask->task;
			if (0 == task->suspended || SIGOBJ_SIGTYPE__TERMINATED == sigtype) {

				/////////////
				wtask->sigtype = sigtype;
				task->wtask_recvcount++;
				_sigobj_wtasklist_remove(wtask);

				if (task->wtask_waitcount == task->wtask_recvcount) {
					task->state = TASK_STATE__READY;
					_task_recalculate_waittick(task);
					_task_changelist(task);
				}
				else if (0 == task->waitall) {
					_task_sigobj_removewtask(task);

					task->state = TASK_STATE__READY;
					_task_recalculate_waittick(task);
					_task_changelist(task);
				}
				////////////////

			}

			wtask = wtask_next;
			if (NULL == wtask) {
				break;
			}
		}

		_sigobj_wtasklist_notifychange(sigobj);

		_task_schedule();
	}

	return 0;
}

int _sigobj_setsender(_sigobj_pt sigobj, _task_pt task) {
	_task_pt task_old;

	assert(sigobj != NULL);

	task_old = _task_osigobjlist_owner(sigobj);
	if (task_old != task) {
		if (NULL != task_old) {
			_task_osigobjlist_remove(sigobj);
			_task_applypriority(task_old);
		}
		if (NULL != task) {
			_task_osigobjlist_insertprev(task, NULL, sigobj);
			_task_applypriority(task);
		}
	}

	return 0;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */
