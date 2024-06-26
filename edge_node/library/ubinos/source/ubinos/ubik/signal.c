/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if !(UBINOS__UBIK__EXCLUDE_SIGNAL == 1)

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__SIGNAL

int signal_create(signal_pt * signal_p) {
	logmfd("signal_p 0x%x", signal_p);

	return signal_create_ext(signal_p, 0);
}

int signal_create_ext(signal_pt * signal_p, unsigned int option) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif
	_sigobj_pt sigobj;

	assert(signal_p != NULL);

	logmfd("signal_p 0x%x, option 0x%x", signal_p, option);

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

	sigobj->type		= OBJTYPE__UBIK_SIGNAL;
	sigobj->valid 		= 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*signal_p = (signal_pt) sigobj;

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
		logmfd("signal 0x%x is created", sigobj);
	}

	return r;
}

int signal_delete(signal_pt * signal_p) {
	assert(signal_p != NULL);
	assert(*signal_p != NULL);

	logmfd("signal_p 0x%x, *signal_p 0x%x", signal_p, *signal_p);

	return _sigobj_delete((_sigobj_pt *) signal_p);
}

int signal_wait(signal_pt _signal) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt task = _task_cur;

	assert(_signal != NULL);

	logmfv("signal 0x%x", _signal);

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
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 == task->sysflag01) {
		r = _sigobj_wait(sigobj, task->wtask_p);
	}
	else {
		r = SIGOBJ_SIGTYPE__TIMEOUT;
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int signal_wait_timed(signal_pt _signal, unsigned int tick) {
	int r;

	assert(_signal != NULL);

	logmfv("signal 0x%x, tick %d", _signal , tick);

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

	r = signal_wait(_signal);

	_task_cur->timed		= 0;

end0:
	return r;
}

int signal_wait_timedms(signal_pt _signal, unsigned int timems) {
	logmfv("signal 0x%x, timems %d", _signal , timems);

	return signal_wait_timed(_signal, ubik_timemstotick(timems));
}

int signal_send(signal_pt _signal, int sigtype) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt otask;

	assert(_signal != NULL);

	logmfv("signal 0x%x, sigtype %d", _signal, sigtype);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (0 > sigtype) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == bsp_isintr() && NULL != otask && _task_cur != otask)	||
			(0 != bsp_isintr() && NULL != otask						)		)
	{
		logme("current task is not an appropriate sender");
		r = -1;
		goto end1;
	}

	r = _sigobj_send(sigobj, sigtype);
	if (0 != r) {
		logme("fail at _sigobj_send()");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int signal_broadcast(signal_pt _signal, int sigtype) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt otask;

	assert(_signal != NULL);

	logmfv("signal 0x%x, sigtype %d", _signal, sigtype);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (0 > sigtype) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == bsp_isintr() && NULL != otask && _task_cur != otask)	||
			(0 != bsp_isintr() && NULL != otask						)		)
	{
		logme("current task is not an appropriate sender");
		r = -1;
		goto end1;
	}

	r = _sigobj_broadcast(sigobj, sigtype);
	if (0 != r) {
		logme("fail at _sigobj_broadcast()");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int signal_setsender(signal_pt _signal, task_pt _task) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt task = (_task_pt) _task;

	assert(_signal != NULL);

	logmfd("signal 0x%x, task 0x%x", _signal, _task);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
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

#endif /* !(UBINOS__UBIK__EXCLUDE_SIGNAL == 1) */
