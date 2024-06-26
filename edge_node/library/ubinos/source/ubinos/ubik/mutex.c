/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

int mutex_create(mutex_pt * mutex_p) {
	return mutex_create_ext(mutex_p, 0);
}
int mutex_create_ext(mutex_pt * mutex_p, unsigned int option) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif
	_sigobj_pt sigobj;

	assert(mutex_p != NULL);

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	if (_kernel_monitor_mutex) {
		r = mutex_lock(_kernel_monitor_mutex);
		if (0 != r) {
			logme("fail at mutex_lock()");
			r = -1;
			goto end0;
		}
	}
	#endif

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme("fail at _sigobj_create()");
		goto end1;
	}

	if (0 != (option & MUTEX_OPT__NOPRIORITYINHERITANCE)) {
		sigobj->nopriorityinheritance = 1;
	}

	sigobj->type		= OBJTYPE__UBIK_MUTEX;
	sigobj->valid 		= 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*mutex_p = (mutex_pt) sigobj;

end1:
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	if (_kernel_monitor_mutex) {
		r2 = mutex_unlock(_kernel_monitor_mutex);
		if (0 != r2) {
			logme("fail at mutex_unlock()");
			r = -1;
		}
	}

end0:
	#endif

	return r;
}

int mutex_delete(mutex_pt * mutex_p) {
	assert(mutex_p != NULL);
	assert(*mutex_p != NULL);

	return _sigobj_delete((_sigobj_pt *) mutex_p);
}

int mutex_lock(mutex_pt _mutex) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _mutex;
	_task_pt task = _task_cur;

	assert(_mutex != NULL);

	if (0 == _bsp_kernel_active) {
		r = 0;
		goto end0;
	}

	if (0 == task->sysflag01) {
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

		if (0 != _ubik_tasklockcount) {
			logme("task is locked");
			r = -1;
			goto end0;
		}
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 == sigobj->count) {
		if (0 == sigobj->nopriorityinheritance) {
			r = _sigobj_setsender(sigobj, task);
			if (0 != r) {
				logme("fail at _sigobj_setsender()");
				r = -1;
				goto end1;
			}
		}
		sigobj->count++;
		r = 0;
	}
	else {
		if (task == _task_osigobjlist_owner(sigobj)) {
			if (sigobj->maxcount == sigobj->count) {
				logme("count is over max count");
				r = UBIK_ERR__OVERFLOWED;
			}
			else {
				sigobj->count++;
				r = 0;
			}
		}
		else {
			if (0 == task->sysflag01) {
				r = _sigobj_wait(sigobj, task->wtask_p);
				if (SIGOBJ_SIGTYPE__TIMEOUT != r && 0 != r) {
					logme("fail at _sigobj_wait()");
				}
			}
			else {
				r = SIGOBJ_SIGTYPE__TIMEOUT;
			}
		}
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int mutex_lock_timed(mutex_pt _mutex, unsigned int tick) {
	int r;

	assert(_mutex != NULL);

	if (0 == _bsp_kernel_active) {
		r = 0;
		goto end0;
	}

	if (0 != bsp_isintr()) {
		logme("in interrupt");
		r = -1;
		goto end0;
	}

	_task_cur->timed		= 1;
	_task_cur->waittick		= tick;

	r = mutex_lock(_mutex);

	_task_cur->timed		= 0;

end0:
	return r;
}

int mutex_lock_timedms(mutex_pt _mutex, unsigned int timems) {
	assert(_mutex != NULL);

	return mutex_lock_timed(_mutex, ubik_timemstotick(timems));
}

int mutex_unlock(mutex_pt _mutex) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _mutex;
	_task_pt otask;
	_wtask_pt wtask;

	assert(_mutex != NULL);

	if (0 == _bsp_kernel_active) {
		r = 0;
		goto end0;
	}

	if (0 != bsp_isintr()) {
		logme("in interrupt");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (NULL != otask && _task_cur != otask) {
		logme("current task is not owner");
		r = -1;
		goto end1;
	}

	if (0 == sigobj->count) {
		logme("not locked");
		r = -1;
		goto end1;
	}

	if (1 < sigobj->count) {
		sigobj->count--;
		r = 0;
	}
	else {
		wtask = _sigobj_wtasklist_gettask(sigobj);
		if (NULL == wtask) {
			if (0 == sigobj->nopriorityinheritance) {
				r = _sigobj_setsender(sigobj, NULL);
				if (0 != r) {
					logme("fail at _sigobj_setsender()");
					r = -1;
					goto end1;
				}
			}
			sigobj->count--;
			r = 0;
		}
		else {
			if (0 == sigobj->nopriorityinheritance) {
				r = _sigobj_setsender(sigobj, wtask->task);
				if (0 != r) {
					logme("fail at _sigobj_setsender()");
					r = -1;
					goto end1;
				}
			}
			r = _sigobj_send(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
			if (0 != r) {
				logme("fail at _sigobj_send()");
			}
		}
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int mutex_islocked(mutex_pt _mutex) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _mutex;

	assert(_mutex != NULL);

	if (0 == _bsp_kernel_active) {
		r = 0;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != sigobj->count) {
		r = 1;
	}
	else {
		r = 0;
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */
