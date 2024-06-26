/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if !(UBINOS__UBIK__EXCLUDE_CONDV == 1)

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__CONDV

int condv_create(condv_pt * condv_p) {
	logmfd("condv_p 0x%x", condv_p);

	return condv_create_ext(condv_p, 0);
}

int condv_create_ext(condv_pt * condv_p, unsigned int option) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif
	_sigobj_pt sigobj;

	assert(condv_p != NULL);

	logmfd("condv_p 0x%x, option 0x%x", condv_p, option);

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}
	#endif

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme("fail at _sigobj_create()");
		goto end1;
	}

	sigobj->type		= OBJTYPE__UBIK_CONDV;
	sigobj->valid 		= 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*condv_p = (condv_pt) sigobj;

end1:
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}

end0:
	#endif

	if (r == 0) {
		logmfd("condv 0x%x is created", sigobj);
	}

	return r;
}

int condv_delete(condv_pt * condv_p) {
	assert(condv_p != NULL);
	assert(*condv_p != NULL);

	logmfd("condv_p 0x%x, *condv_p 0x%x", condv_p, *condv_p);

	return _sigobj_delete((_sigobj_pt *) condv_p);
}

int condv_wait(condv_pt _condv, mutex_pt _mutex) {
	int r;
	int r2;
	int needlock = 0;
	unsigned int task_timed_old;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_sigobj_pt mutex = (_sigobj_pt) _mutex;
	_task_pt otask;
	_task_pt task = _task_cur;

	assert(_condv != NULL);
	assert(_mutex != NULL);

	logmfv("condv 0x%x, mutex 0x%x", _condv, _mutex);

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
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
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (	(	NULL == mutex							)	||
			(	0 == mutex->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != mutex->type		)	||
			(	1 != mutex->count						)		)
	{
		logme("parameter 2 is wrong");
		r = -3;
		goto end1;
	}

	otask = _task_osigobjlist_owner(mutex);
	if (NULL != otask && task != otask) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end1;
	}

	if (0 == task->sysflag01) {
		if (0 == _ubik_tasklockcount) {
			_ubik_tasklockcount++;
			r = mutex_unlock((mutex_pt) mutex);
			_ubik_tasklockcount--;
		}
		else {
			r = mutex_unlock((mutex_pt) mutex);
		}
		if (0 != r) {
			logme("fail at mutex_unlock()");
			r = -3;
			goto end1;
		}
		needlock = 1;

		task->wtask_p->mutex = (mutex_pt) mutex;
		r = _sigobj_wait(sigobj, task->wtask_p);
		if (SIGOBJ_SIGTYPE__TIMEOUT != r && 0 != r) {
			logme("fail at _sigobj_wait()");
		}
		task->wtask_p->mutex = NULL;
	}
	else {
		r = SIGOBJ_SIGTYPE__TIMEOUT;
	}

end1:
	ubik_exitcrit();

	if (0 != needlock) {
		task_timed_old = task->timed;
		task->timed = 0;
		r2 = mutex_lock((mutex_pt) mutex);
		if (0 != r2) {
			logme("fail at mutex_lock()");
			r = -3;
		}
		task->timed = task_timed_old;
	}

end0:
	return r;
}

int condv_wait_timed(condv_pt _condv, mutex_pt _mutex, unsigned int tick) {
	int r;

	logmfv("condv 0x%x, mutex 0x%x, tick %d", _condv, _mutex, tick);

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
		goto end0;
	}

	if (0 != bsp_isintr()) {
		logme("in interrupt");
		r = -1;
		goto end0;
	}

	_task_cur->timed		= 1;
	_task_cur->waittick		= tick;

	r = condv_wait(_condv, _mutex);

	_task_cur->timed		= 0;

end0:
	return r;
}

int condv_wait_timedms(condv_pt _condv, mutex_pt _mutex, unsigned int timems) {
	logmfv("condv 0x%x, mutex 0x%x, timems %d", _condv, _mutex, timems);

	return condv_wait_timed(_condv, _mutex, ubik_timemstotick(timems));
}

int condv_signal(condv_pt _condv) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_task_pt otask;

	assert(_condv != NULL);

	logmfv("condv 0x%x", _condv);

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

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (NULL != otask && _task_cur != otask) {
		logme("current task is not an appropriate sender");
		r = -1;
		goto end1;
	}

	r = _sigobj_send(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
	if (0 != r) {
		logme("fail at _sigobj_send()");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int condv_broadcast(condv_pt _condv) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_task_pt otask;

	assert(_condv != NULL);

	logmfv("condv 0x%x", _condv);

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

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (NULL != otask && _task_cur != otask) {
		logme("current task is not an appropriate sender");
		r = -1;
		goto end1;
	}

	r = _sigobj_broadcast(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
	if (0 != r) {
		logme("fail at _sigobj_broadcast()");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int condv_setsender(condv_pt _condv, task_pt _task) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_task_pt task = (_task_pt) _task;

	assert(_condv != NULL);

	logmfd("condv 0x%x, task 0x%x", _condv, _task);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	r = _sigobj_setsender(sigobj, task);
	if (0 != r) {
		logme("fail at _sigobj_setsender()");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */

#endif /* !(UBINOS__UBIK__EXCLUDE_CONDV == 1) */
