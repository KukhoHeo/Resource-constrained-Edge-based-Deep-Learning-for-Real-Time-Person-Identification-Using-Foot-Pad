/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__STIMER

int stimer_create(stimer_pt * stimer_p) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif
	_stimer_pt stimer;

	assert(stimer_p != NULL);

	logmfd("stimer_p 0x%x", stimer_p);

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

	if (NULL == stimer_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}
	#endif

	stimer = malloc(sizeof(_stimer_t));
	if (NULL == stimer) {
		logme("fail at malloc()");
		r = -1;
		goto end1;
	}

	stimer->type					= OBJTYPE__UBIK_STIMER;
	stimer->valid					= 0;
	stimer->reserved1				= 0;

	stimer->reserved2				= 0;
	stimer->broadcast				= 0;
	stimer->oneshot					= 0;
	stimer->running					= 0;

	edlist_link_init(&stimer->stimerlist_link);

	stimer->waittick				= 0;
	stimer->wakeuptick				= 0;

	stimer->sigobj					= NULL;
	stimer->sigtype					= 0;

	stimer->valid					= 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	edlist_link_init(&stimer->kernel_monitor_stimerlist_link);
	_kernel_monitor_stimerlist_insertnext(&_kernel_monitor_stimerlist, NULL, stimer);
	#endif

	*stimer_p = (stimer_pt) stimer;

	r = 0;

end1:
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}
	#endif

end0:
	if (r == 0) {
		logmfd("stimer 0x%x is created", stimer);
	}

	return r;
}

int stimer_delete(stimer_pt * stimer_p) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif
	_stimer_pt stimer;
	void * stimerbuf = NULL;

	assert(stimer_p != NULL);
	assert(*stimer_p != NULL);

	logmfd("stimer_p 0x%x, *stimer_p 0x%x", stimer_p, *stimer_p);

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

	if (NULL == stimer_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	stimer = (_stimer_pt) (*stimer_p);

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}
	#endif

	ubik_entercrit();

	if (	(	NULL == stimer								)	||
			(	0 == stimer->valid							) 	||
			(	OBJTYPE__UBIK_STIMER		!= stimer->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme("stimer is running");
		r = -2;
		goto end1;
	}

	stimerbuf 		= stimer;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_stimerlist_remove(stimer);
	#endif

	stimer->valid 	= 0;

	*stimer_p 		= NULL;

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

	if (NULL != stimerbuf) {
		free(stimerbuf);
	}

end0:
	return r;
}

int stimer_set(stimer_pt _stimer, unsigned int tick, sem_pt sem, unsigned int option) {
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;
	_sigobj_pt sigobj = (_sigobj_pt) sem;

	assert(_stimer != NULL);
	assert(sem != NULL);

	logmfd("stimer 0x%x, tick %d, sem 0x%x, option 0x%x", _stimer, tick, sem, option);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (0 >= tick) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER != stimer->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme("stimer is running");
		r = -2;
		goto end1;
	}

	if (	(	NULL == sigobj								)	||
			(	0 == sigobj->valid							) 	||
			(	OBJTYPE__UBIK_SEM 		!= sigobj->type		)		)
	{
		logme("parameter 3 is wrong");
		r = -4;
		goto end1;
	}

	if (0 != (option & STIMER_OPT__ONESHOT)) {
		stimer->oneshot	= 1;
	}
	else {
		stimer->oneshot	= 0;
	}

	stimer->waittick	= tick;

	stimer->sigobj		= sigobj;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int stimer_setms(stimer_pt _stimer, unsigned int timems, sem_pt sem, unsigned int option) {
	logmfd("stimer 0x%x, timems %d, sem 0x%x, option 0x%x", _stimer, timems, sem, option);

	return stimer_set(_stimer, ubik_timemstotick(timems), sem, option);
}

int stimer_set_signal(stimer_pt _stimer, unsigned int tick, signal_pt signal, int sigtype, unsigned int option) {
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;
	_sigobj_pt sigobj = (_sigobj_pt) signal;

	assert(_stimer != NULL);
	assert(signal != NULL);

	logmfd("stimer 0x%x, tick %d, signal 0x%x, sigtype %d, option 0x%x", _stimer, tick, signal, sigtype, option);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (0 >= tick) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (0 > sigtype) {
		logme("parameter 4 is wrong");
		r = -5;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER != stimer->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme("stimer is running");
		r = -2;
		goto end1;
	}

	if (	(	NULL == sigobj								)	||
			(	0 == sigobj->valid							) 	||
			(	OBJTYPE__UBIK_SIGNAL 	!= sigobj->type		)		)
	{
		logme("parameter 3 is wrong");
		r = -4;
		goto end1;
	}

	if (0 != (option & STIMER_OPT__ONESHOT)) {
		stimer->oneshot	= 1;
	}
	else {
		stimer->oneshot	= 0;
	}

	if (0 != (option & STIMER_OPT__BROADCAST)) {
		stimer->broadcast	= 1;
	}
	else {
		stimer->broadcast	= 0;
	}

	stimer->waittick	= tick;

	stimer->sigobj		= sigobj;

	stimer->sigtype		= sigtype;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int stimer_setms_signal(stimer_pt _stimer, unsigned int timems, signal_pt signal, int sigtype, unsigned int option) {
	logmfd("stimer 0x%x, tick %d, signal 0x%x, sigtype %d, option 0x%x", _stimer, timems, signal, sigtype, option);

	return stimer_set_signal(_stimer, ubik_timemstotick(timems), signal, sigtype, option);
}

int stimer_start(stimer_pt _stimer) {
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;
	_sigobj_pt sigobj;

	assert(_stimer != NULL);

	logmfd("stimer 0x%x", _stimer);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER 	!= stimer->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme("stimer is running");
		r = -2;
		goto end1;
	}

	if (0 >= stimer->waittick) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	sigobj = stimer->sigobj;

	if (	(	NULL == sigobj																)	||
			(	0 == sigobj->valid															) 	||
			(	OBJTYPE__UBIK_SEM != sigobj->type && OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	_stimerlist_add(stimer);

	stimer->running = 1;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int stimer_stop(stimer_pt _stimer) {
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;

	assert(_stimer != NULL);

	logmfd("stimer 0x%x", _stimer);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER 	!= stimer->type	)	)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 == stimer->running) {
		logme("stimer is not running");
		r = -2;
		goto end1;
	}

	_stimerlist_remove(stimer);

	stimer->running = 0;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */

#endif /* !(UBINOS__UBIK__EXCLUDE_STIMER == 1) */
