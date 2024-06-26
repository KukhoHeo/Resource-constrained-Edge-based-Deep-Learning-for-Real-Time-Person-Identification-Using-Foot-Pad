/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if !(UBINOS__UBIK__EXCLUDE_SEM == 1)

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__SEM

int sem_create(sem_pt * sem_p) {
	logmfd("sem_p 0x%x", sem_p);

	return sem_create_ext(sem_p, 0, UINT_MAX, 0);
}

int semb_create(sem_pt * sem_p) {
	logmfd("sem_p 0x%x", sem_p);

	return sem_create_ext(sem_p, 0, 1, SEM_OPT__IGNOREOVERFLOW);
}

int sem_create_ext(sem_pt * sem_p, unsigned int initcount, unsigned int maxcount, unsigned int option) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif
	_sigobj_pt sigobj;

	assert(sem_p != NULL);

	logmfd("sem_p 0x%x, initcount %d, maxcount %d, option 0x%x", sem_p, initcount, maxcount, option);

	if (initcount > maxcount) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (0 >= maxcount) {
		logme("parameter 3 is wrong");
		r = -4;
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

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme("fail at _sigobj_create()");
		goto end1;
	}

	sigobj->count 		= initcount;
	sigobj->maxcount 	= maxcount;

	if (0 != (option & SEM_OPT__IGNOREOVERFLOW)) {
		sigobj->ignoreoverflow = 1;
	}

	sigobj->type		= OBJTYPE__UBIK_SEM;
	sigobj->valid 		= 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*sem_p = (sem_pt) sigobj;

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
		logmfd("sem 0x%x is created", sigobj);
	}

	return r;
}

int sem_delete(sem_pt * sem_p) {
	assert(sem_p != NULL);
	assert(*sem_p != NULL);

	logmfd("sem_p 0x%x, *sem_p 0x%x", sem_p, *sem_p);

	return _sigobj_delete((_sigobj_pt *) sem_p);
}

int sem_take(sem_pt _sem) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;
	_task_pt task = _task_cur;

	assert(_sem != NULL);

	logmfv("sem 0x%x", _sem);

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
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 < sigobj->count) {
		sigobj->count--;
		r = 0;
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

end1:
	ubik_exitcrit();

end0:
	return r;
}

int sem_take_timed(sem_pt _sem, unsigned int tick) {
	int r;
	unsigned int sysflag01_old = 0;

	assert(_sem != NULL);

	logmfv("sem 0x%x, tick %d", _sem , tick);

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
		goto end0;
	}

	if (0 != bsp_isintr()) {
		if (0 == tick) {
			sysflag01_old = _task_cur->sysflag01;
			_task_cur->sysflag01 = 1;
		}
		else {
			logme("in interrupt");
			r = -1;
			goto end0;
		}
	}
	else {
		_task_cur->timed		= 1;
		_task_cur->waittick		= tick;
	}

	r = sem_take(_sem);

	if (0 != bsp_isintr()) {
		 _task_cur->sysflag01 = sysflag01_old;
	}
	else {
		_task_cur->timed		= 0;
	}

end0:
	return r;
}

int sem_take_timedms(sem_pt _sem, unsigned int timems) {
	logmfv("sem 0x%x, timems %d", _sem , timems);

	return sem_take_timed(_sem, ubik_timemstotick(timems));
}

int sem_give(sem_pt _sem) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;
	_task_pt otask;
	_wtask_pt wtask;

	assert(_sem != NULL);

	logmfv("sem 0x%x", _sem);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == bsp_isintr() && NULL != otask && _task_cur != otask)	||
			(0 != bsp_isintr() && NULL != otask						)		)
	{
		logme("current task is not an appropriate giver");
		r = -1;
		goto end1;
	}

	if (sigobj->maxcount == sigobj->count) {
		if (0 == sigobj->ignoreoverflow) {
			logme("count is over max count");
			r  = UBIK_ERR__OVERFLOWED;
		}
		else {
			r = 0;
		}
	}
	else {
		wtask = _sigobj_wtasklist_gettask(sigobj);
		if (NULL == wtask) {
			sigobj->count++;
			r = 0;
		}
		else {
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

int sem_clear(sem_pt _sem) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;

	assert(_sem != NULL);

	logmfd("sem 0x%x", _sem);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	sigobj->count = 0;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int sem_setsender(sem_pt _sem, task_pt _task) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;
	_task_pt task = (_task_pt) _task;

	assert(_sem != NULL);

	logmfd("sem 0x%x, task 0x%x", _sem, _task);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
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

int sem_getcount(sem_pt _sem, unsigned int * count_p) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;

	assert(_sem != NULL);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == count_p) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	*count_p = sigobj->count;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */

#endif /* !(UBINOS__UBIK__EXCLUDE_SEM == 1) */
