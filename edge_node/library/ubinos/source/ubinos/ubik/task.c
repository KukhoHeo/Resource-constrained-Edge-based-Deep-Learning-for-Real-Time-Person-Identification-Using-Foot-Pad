/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__TASK

#ifndef UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT
	#define UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT	UBINOS__UBIK__TASK_STACK_DEPTH_MIN
#endif

int task_comp_init(unsigned int idle_stackdepth) {
	int i;
	int r;

	edlist_init(&_task_list_blocked_timed_a[0]);
	edlist_init(&_task_list_blocked_timed_a[1]);
	_task_list_blocked_timed_cur  	= &_task_list_blocked_timed_a[0];
	_task_list_blocked_timed_next 	= &_task_list_blocked_timed_a[1];

	edlist_init(&_task_list_blocked);

	for (i=0; i<=UBINOS__UBIK__TASK_PRIORITY_MAX; i++) {
		edlist_init(&_task_list_ready_a[i]);
	}
	_task_list_ready_index = 0;
	_task_list_ready_cur = &_task_list_ready_a[_task_list_ready_index];

	edlist_init(&_task_list_suspended);
	edlist_init(&_task_list_terminated);
	edlist_init(&_task_list_terminated_noautodel);

	_task_cur = NULL;
	r = task_create(NULL, &_task_idlefunc, NULL, TASK_PRIORITY__IDLE, idle_stackdepth, "idle");
	if (0 != r) {
		logme("fail at task_create");
		goto end0;
	}
	_task_cur->type = OBJTYPE__UBIK_IDLETASK;

end0:
	return r;
}

int task_create(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name) {
	logmfd("task_p 0x%x, func 0x%x, arg 0x%x, priority %d, stackdepth %d, name %s",
			task_p, func, arg, priority, stackdepth, name);

	return task_create_ext(task_p, func, arg, priority, stackdepth, name, 0, 0);
}

int task_create_noautodel(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name) {
	logmfd("task_p 0x%x, func 0x%x, arg 0x%x, priority %d, stackdepth %d, name %s",
			task_p, func, arg, priority, stackdepth, name);

	return task_create_ext(task_p, func, arg, priority, stackdepth, name, 0, TASK_OPT__NOAUTODEL);
}

int task_create_ext(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name, unsigned int tag, unsigned int option) {
	_task_pt task;
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	#endif

	logmfd("task_p 0x%x, func 0x%x, arg 0x%x, priority %d, stackdepth %d, name %s, tag %d, option 0x%x",
			task_p, func, arg, priority, stackdepth, name, tag, option);

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

	_task_collectgarbage();

	if (NULL == func) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}
	if (TASK_PRIORITY__IDLE >= priority || UBINOS__UBIK__TASK_PRIORITY_MAX < priority) {
		if (NULL != _task_cur || TASK_PRIORITY__IDLE != priority) {
			logme("parameter 4 is wrong");
			r = -5;
			goto end0;
		}
	}

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}
	#endif

	if (0 == stackdepth) {
		stackdepth = UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT;
	}

	if (UBINOS__UBIK__TASK_STACK_DEPTH_MIN > stackdepth) {
		stackdepth = UBINOS__UBIK__TASK_STACK_DEPTH_MIN;
	}

	task = malloc(sizeof(_task_t));
	if (NULL == task) {
		logme("fail at malloc()");
		r = -1;
		goto end1;
	}
	_task_init(task);

	task->stacksize = stackdepth * INT_SIZE;
	task->stack = malloc(task->stacksize);
	if (NULL == task->stack) {
		logme("fail at malloc()");
		free(task);
		r = -1;
		goto end1;
	}
	_task_stackinit(task, arg);

	#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
	r = signal_create(&task->monitor);
	if (0 != r) {
		logme("fail at signal_create()");
		free(task->stack);
		free(task);
		r = -1;
		goto end1;
	}
	#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

	task->func = func;

	task->priority_ori 	= priority;
	task->priority 		= priority;

	if ((option & TASK_OPT__NOAUTODEL) != 0) {
		task->noautodel = 1;
	}

	if (NULL != name) {
		strncpy(task->name, name, UBINOS__UBIK__TASK_NAME_SIZE_MAX);
	}

	logmfd("task 0x%x is created", task);

	ubik_entercrit();

	task->valid = 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_tasklist_insertnext(&_kernel_monitor_tasklist, NULL, task);
	#endif

	if (NULL == _task_cur) {
		_task_cur = task;
	}

	if (NULL != task_p) {
		*task_p = (task_pt) task;
	}

	_task_changelist(task);

	_task_schedule();

	r = 0;

	ubik_exitcrit();

end1:
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}
	#endif

end0:
	return r;
}

int task_delete(task_pt * task_p) {
	int r;
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r2;
	unsigned int needunlock = 0;
	#endif
	_task_pt task;
	int noautodel = 0;
	#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
	signal_pt monitor;
	#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

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

	_task_collectgarbage();

	if (NULL == task_p || NULL == *task_p || _task_cur == (_task_pt) (*task_p)) {
		task = _task_cur;

		logmfd("task_p 0x%x, task 0x%x", task_p, task);

		if (0 == _bsp_kernel_active) {
			logme("ubik is not active");
			r = -1;
			goto end0;
		}

		if (0 != _ubik_tasklockcount) {
			logme("task is locked");
			r = -1;
			goto end0;
		}

		#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
		r = mutex_lock(_kernel_monitor_mutex);
		if (0 != r) {
			logme("fail at mutex_lock()");
			r = -1;
			goto end0;
		}
		needunlock = 1;
		#endif

		ubik_entercrit();

		#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
		if (0 != _bsp_kernel_active) {
			if (1 != task->osigobjlist.count) {
				logme("task is holding objects");
				r = -1;
				goto end1;
			}
		}
		else {
			if (0 != task->osigobjlist.count) {
				logme("task is holding objects");
				r = -1;
				goto end1;
			}
		}
		#else
		if (0 != task->osigobjlist.count) {
			logme("task is holding objects");
			r = -1;
			goto end1;
		}
		#endif

		#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
		_kernel_monitor_tasklist_remove(task);
		#endif

		task->valid = 0;

		#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
		monitor = task->monitor;
		if (0 == _ubik_tasklockcount) {
			_ubik_tasklockcount++;
			_sigobj_broadcast((_sigobj_pt) monitor, SIGOBJ_SIGTYPE__TERMINATED);
			_ubik_tasklockcount--;
		}
		else {
			_sigobj_broadcast((_sigobj_pt) monitor, SIGOBJ_SIGTYPE__TERMINATED);
		}
		#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */


		if (NULL != task_p) {
			*task_p = NULL;
		}

		task->state = TASK_STATE__TERMINATED;
		_task_changelist(task);

		#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
		if (0 != needunlock) {
			needunlock = 0;
			r2 = mutex_unlock(_kernel_monitor_mutex);
			if (0 != r2) {
				logme("fail at mutex_unlock()");
				r = -1;
			}
		}
		#endif

		_task_yield();
	}
	else {
		task = (_task_pt) (*task_p);

		logmfd("task_p 0x%x, task 0x%x", task_p, task);

		if (NULL == _task_cur) {
			logme("ubik is not initialized");
			r = -1;
			goto end0;
		}

		#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
		r = mutex_lock(_kernel_monitor_mutex);
		if (0 != r) {
			logme("fail at mutex_lock()");
			r = -1;
			goto end0;
		}
		needunlock = 1;
		#endif

		ubik_entercrit();

		if (task->noautodel) {
			noautodel = 1;
		}

		if (OBJTYPE__UBIK_TASK != task->type) {
			logme("parameter 1 is wrong");
			r = -2;
			goto end1;
		}

		if (task->valid) {
			if (0 != task->osigobjlist.count) {
				logme("task is holding objects");
				r = -1;
				goto end1;
			}

			#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
			_kernel_monitor_tasklist_remove(task);
			#endif

			task->valid = 0;

			#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
			monitor = task->monitor;
			if (0 == _ubik_tasklockcount) {
				_ubik_tasklockcount++;
				_sigobj_broadcast((_sigobj_pt) monitor, SIGOBJ_SIGTYPE__TERMINATED);
				_ubik_tasklockcount--;
			}
			else {
				_sigobj_broadcast((_sigobj_pt) monitor, SIGOBJ_SIGTYPE__TERMINATED);
			}
			#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

			*task_p = NULL;

			_task_sigobj_removewtask(task);

			task->state = TASK_STATE__TERMINATED;
			_task_changelist(task);
		}
		else {
			if (!noautodel) {
				logme("parameter 1 is wrong");
				r = -2;
				goto end1;
			}
		}

		if (noautodel) {
			_tasklist_remove(task);

			if (_task_prev == task) {
				_task_prev = NULL;
			}
		}
	}

	r = 0;

end1:
	ubik_exitcrit();

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	if (0 != needunlock) {
		needunlock = 0;
		r2 = mutex_unlock(_kernel_monitor_mutex);
		if (0 != r2) {
			logme("fail at mutex_unlock()");
			r = -1;
		}
	}
	#endif

	if (noautodel) {
		#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
		if (NULL != task->monitor) {
			signal_delete(&task->monitor);
		}
		#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

		if (&task->wtask != task->wtask_p && NULL != task->wtask_p) {
			free(task->wtask_p);
		}
		free(task->stack);
		free(task);
	}
	else {
		_task_collectgarbage();
	}

end0:
	return r;
}

int task_suspend(task_pt _task) {
	int r;
	_task_pt task = (_task_pt) _task;

	logmfd("task 0x%x", _task);

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

	if (NULL == task || _task_cur == task) {
		task = _task_cur;

		if (0 == _bsp_kernel_active) {
			logme("ubik is not active");
			r = -1;
			goto end0;
		}

		if (0 != _ubik_tasklockcount) {
			logme("task is locked");
			r = -1;
			goto end0;
		}

		ubik_entercrit();

		task->suspended		= 1;
		task->state			= TASK_STATE__READY;
		_task_changelist(task);

		_task_yield();
	}
	else {
		if (NULL == _task_cur) {
			logme("ubik is not initialized");
			r = -1;
			goto end0;
		}

		ubik_entercrit();

		if (0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
			logme("parameter 1 is wrong");
			r = -2;
			goto end1;
		}

		if (0 != task->suspended) {
			logme("task is already suspended");
			r = -1;
			goto end1;
		}

		task->suspended = 1;

		if (TASK_STATE__BLOCKED == task->state) {
			_task_recalculate_waittick(task);
			_task_sigobj_notifywtaskchange(task);
		}

		_task_changelist(task);
	}

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_resume(task_pt _task) {
	int r;
	_task_pt task = (_task_pt) _task;
	_wtask_pt wtask;
	_sigobj_pt sigobj;
	int i;
	unsigned int task_sysflag01_old;

	logmfd("task 0x%x", _task);

	if (NULL == task || _task_cur == task) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

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

	if (0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 == task->suspended) {
		logme("task is not suspended");
		r = -1;
		goto end1;
	}

	task->suspended = 0;

	if (TASK_STATE__BLOCKED == task->state) {
		task_sysflag01_old 		= _task_cur->sysflag01;
		_task_cur->sysflag01	= 1;

		for (i=0; i<task->wtask_count; i++) {
			wtask = &task->wtask_p[i];
			sigobj = _sigobj_wtasklist_owner(wtask);

			if (NULL != sigobj) {
				switch(sigobj->type) {
				#if !(UBINOS__UBIK__EXCLUDE_SIGNAL == 1)
				case OBJTYPE__UBIK_SIGNAL:
					r = SIGOBJ_SIGTYPE__TIMEOUT;
					break;
				#endif /* !(UBINOS__UBIK__EXCLUDE_SIGNAL == 1) */
				#if !(UBINOS__UBIK__EXCLUDE_CONDV == 1)
				case OBJTYPE__UBIK_CONDV:
					if (NULL == wtask->mutex) {
						logme("wtask->mutex is wrong");
						r = -1;
					}
					else {
						switch (((_sigobj_pt) (wtask->mutex))->type) {
						case OBJTYPE__UBIK_MUTEX:
							r = SIGOBJ_SIGTYPE__TIMEOUT;
							break;
						default:
							logme("wtask->mutex is wrong");
							r = -1;
							break;
						}
					}
					break;
				#endif /* !(UBINOS__UBIK__EXCLUDE_CONDV == 1) */
				case OBJTYPE__UBIK_MUTEX:
					if (0 == sigobj->count) {
						r = mutex_lock((mutex_pt) sigobj);
					}
					else {
						r = SIGOBJ_SIGTYPE__TIMEOUT;
					}
					break;
				#if !(UBINOS__UBIK__EXCLUDE_SEM == 1)
				case OBJTYPE__UBIK_SEM:
					r = sem_take((sem_pt) sigobj);
					break;
				#endif /* !(UBINOS__UBIK__EXCLUDE_SEM == 1) */
				#if !(UBINOS__UBIK__EXCLUDE_MSGQ == 1)
				case OBJTYPE__UBIK_MSGQ:
					r = msgq_receive((msgq_pt) sigobj, wtask->msg);
					break;
				#endif /* !(UBINOS__UBIK__EXCLUDE_MSGQ == 1) */
				default:
					logme("sigobj->type is wrong");
					r = -1;
					break;
				}

				if (SIGOBJ_SIGTYPE__SUCCESS == r) {
					wtask->sigtype = SIGOBJ_SIGTYPE__SUCCESS;
					task->wtask_recvcount++;
					_sigobj_wtasklist_remove(wtask);
					_sigobj_wtasklist_notifychange(sigobj);
				}
			}
		}

		_task_cur->sysflag01	= task_sysflag01_old;

		if (task->wtask_waitcount == task->wtask_recvcount) {
			task->state = TASK_STATE__READY;
			_task_changelist(task);
		}
		else if (0 == task->waitall && 0 < task->wtask_recvcount) {
			_task_sigobj_removewtask(task);

			task->state = TASK_STATE__READY;
			_task_changelist(task);
		}
		else if (0 != task->timed) {
			task->wakeuptick = _ubik_tickcount + task->waittick;
		}
	}

	_task_applypriority(task);

	_task_changelist(task);

	_task_schedule();

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_sleep(unsigned int tick) {
	int r;

	logmfv("tick %d", tick);

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

	if (tick <= 0) {
		r = 0;
		goto end0;
	}

	if ((0 == _bsp_kernel_active) || (OBJTYPE__UBIK_IDLETASK  == _task_cur->type)) {
		bsp_busywait(bsp_getbusywaitcountperms() * ubik_ticktotimems(tick));
		_task_cur->waittick 	= 0;
	}
	else {
		ubik_entercrit();

		_task_cur->timed		= 1;
		_task_cur->waittick 	= tick;
		_task_cur->wakeuptick 	= _ubik_tickcount + tick;
		_task_cur->state 		= TASK_STATE__BLOCKED;

		_task_changelist(_task_cur);

		_task_yield();

		_task_cur->timed		= 0;

		ubik_exitcrit();
	}

	r = 0;

end0:
	return r;
}

int task_sleepms(unsigned int timems) {
	logmfv("timems %d", timems);

	return task_sleep(ubik_timemstotick(timems));
}

int task_setpriority(task_pt _task, int priority) {
	int r;
	_task_pt task = (_task_pt) _task;

	logmfd("task 0x%x, priority %d", _task, priority);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (TASK_PRIORITY__IDLE >= priority || UBINOS__UBIK__TASK_PRIORITY_MAX < priority) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	task->priority_ori = priority;

	_task_applypriority(task);

	_task_schedule();

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_getpriority(task_pt _task) {
	int r;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == task && 0 == _bsp_kernel_active) {
		return task_getmiddlepriority();
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	r = task->priority;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_gethighestpriority(void) {
	return UBINOS__UBIK__TASK_PRIORITY_MAX;
}

int task_getlowestpriority(void) {
	return TASK_PRIORITY__IDLE+1;
}

int task_getmiddlepriority(void) {
	return ((UBINOS__UBIK__TASK_PRIORITY_MAX - TASK_PRIORITY__IDLE) / 2 + 1);
}

void task_lock(void) {
	ubik_entercrit();

	if (UINT_MAX > _ubik_tasklockcount) {
		_ubik_tasklockcount++;
	}

	ubik_exitcrit();
}

void task_unlock(void) {
	logmfd("");

	ubik_entercrit();

	if (0 < _ubik_tasklockcount) {
		_ubik_tasklockcount--;
		_task_schedule();
	}

	ubik_exitcrit();
}

void task_yield(void)
{
	ubik_entercrit();

	_task_schedule();

	ubik_exitcrit();
}

int task_setmaxwaitsigobj(task_pt _task, int max) {
	int r;
	_task_pt task = (_task_pt) _task;
	_wtask_pt wtask_p = NULL;
	void * tempptr = NULL;
	int i;

	logmfd("task 0x%x, max %d", _task, max);

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

	if (0 >= max || UBINOS__UBIK__TASK_MAXWAITSIGOBJ_MAX < max) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	if (1 != max) {
		wtask_p = malloc(sizeof(_wtask_t) * max);
		tempptr = wtask_p;
		if (NULL == wtask_p) {
			logme("fail at malloc");
			r = -1;
			goto end0;
		}

		for (i=0; i<max; i++) {
			edlist_link_init(&wtask_p[i].link);
			wtask_p[i].task			= task;
			wtask_p[i].sigtype		= SIGOBJ_SIGTYPE__FAIL;
			wtask_p[i].msg			= NULL;
			wtask_p[i].mutex		= NULL;
		}
	}

	ubik_entercrit();

	if (0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != task->wtask_count) {
		logme("waiting for object");
		r = -1;
		goto end1;
	}

	if (max == task->wtask_max) {
		r = 0;
		goto end1;
	}

	tempptr = task->wtask_p;
	if (&task->wtask == tempptr) {
		tempptr = NULL;
	}

	if (1 != max) {
		task->wtask_p = wtask_p;
	}
	else {
		task->wtask_p = &task->wtask;
	}
	task->wtask_max = max;

	r = 0;

end1:
	ubik_exitcrit();

	if (NULL != tempptr) {
		free(tempptr);
	}

end0:
	return r;
}

int task_getmaxwaitsigobj(task_pt _task) {
	int r;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	r = task->wtask_max;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_waitforsigobjs(void ** _sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt) {
	int r;
	int r2;
	_sigobj_pt * sigobj_p = (_sigobj_pt *) _sigobj_p;
	_sigobj_pt sigobj;
	_wtask_pt wtask;
	int i;
	int count_invalid = 0;
	int count_receive = 0;
	int needlock = 0;
	unsigned int task_sysflag01_old;
	unsigned int task_timed_old;

	logmfv("sigobj_p 0x%x, sigtype_p 0x%x, param_p 0x%x, count %d, waitopt 0x%x",
			_sigobj_p, sigtype_p, param_p, count, waitopt);

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
		goto end0;
	}

	if (0 == _task_cur->sysflag01) {
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

	if (NULL == sigobj_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	if (NULL == sigtype_p) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (0 >= count || _task_cur->wtask_max < count) {
		logme("parameter 4 is wrong");
		r = -5;
		goto end1;
	}

	task_sysflag01_old 		= _task_cur->sysflag01;
	_task_cur->sysflag01	= 1;

	for (i=0; i<count; i++) {
		wtask = &_task_cur->wtask_p[i];
		sigobj = sigobj_p[i];

		if (	(	NULL == sigobj							)	||
				(	0 == sigobj->valid						)		)
		{
			count_invalid++;
			r = -2;
		}
		else {
			switch(sigobj->type) {
			#if !(UBINOS__UBIK__EXCLUDE_SIGNAL == 1)
			case OBJTYPE__UBIK_SIGNAL:
				r = signal_wait((signal_pt) sigobj);
				break;
			#endif /* !(UBINOS__UBIK__EXCLUDE_SIGNAL == 1) */
			#if !(UBINOS__UBIK__EXCLUDE_CONDV == 1)
			case OBJTYPE__UBIK_CONDV:
				if (NULL == param_p) {
					r = -3;
				}
				else {
					r = condv_wait((condv_pt) sigobj, (mutex_pt) param_p[i]);
				}
				break;
			#endif /* !(UBINOS__UBIK__EXCLUDE_CONDV == 1) */
			case OBJTYPE__UBIK_MUTEX:
				r = mutex_lock((mutex_pt) sigobj);
				break;
			#if !(UBINOS__UBIK__EXCLUDE_SEM == 1)
			case OBJTYPE__UBIK_SEM:
				r = sem_take((sem_pt) sigobj);
				break;
			#endif /* !(UBINOS__UBIK__EXCLUDE_SEM == 1) */
			#if !(UBINOS__UBIK__EXCLUDE_MSGQ == 1)
			case OBJTYPE__UBIK_MSGQ:
				if (NULL == param_p) {
					r = -3;
				}
				else {
					r = msgq_receive((msgq_pt) sigobj, (unsigned char *) param_p[i]);
				}
				break;
			#endif /* !(UBINOS__UBIK__EXCLUDE_MSGQ == 1) */
			default:
				r = -2;
				break;
			}

			if (SIGOBJ_SIGTYPE__SUCCESS == r) {
				count_receive++;
			}
			else if (SIGOBJ_SIGTYPE__TIMEOUT != r) {
				count_invalid++;
			}
		}

		wtask->sigtype = r;
	}

	_task_cur->sysflag01	= task_sysflag01_old;

	if (0 != count_invalid && 0 == (waitopt & TASK_WAITOPT__IGNOREINVALID)) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 != count_receive && 0 == (waitopt & TASK_WAITOPT__ALL)) {
		r = 0;
		goto end1;
	}

	if (count_receive == (count - count_invalid)) {
		r = 0;
		goto end1;
	}

	if (0 != _task_cur->timed) {
		if (0 == _task_cur->waittick) {
			r = SIGOBJ_SIGTYPE__TIMEOUT;
			goto end1;
		}
		else {
			_task_cur->wakeuptick = _ubik_tickcount + _task_cur->waittick;
		}
	}

	if (0 != (waitopt & TASK_WAITOPT__ALL)) {
		_task_cur->waitall		= 1;
	}

	_task_cur->wtask_count 		= count;
	_task_cur->wtask_waitcount	= count - count_invalid;
	_task_cur->wtask_recvcount 	= count_receive;

	for (i=0; i<count; i++) {
		wtask = &_task_cur->wtask_p[i];
		wtask->msg = NULL;
		wtask->mutex = NULL;

		if (SIGOBJ_SIGTYPE__TIMEOUT == wtask->sigtype) {
			sigobj = sigobj_p[i];

			switch(sigobj->type) {
			#if !(UBINOS__UBIK__EXCLUDE_CONDV == 1)
			case OBJTYPE__UBIK_CONDV:
				if (0 == _ubik_tasklockcount) {
					_ubik_tasklockcount++;
					r = mutex_unlock((mutex_pt) param_p[i]);
					_ubik_tasklockcount--;
				}
				else {
					r = mutex_unlock((mutex_pt) param_p[i]);
				}
				if (0 != r) {
					logme("fail at mutex_unlock()");
					r = -4;
					goto end1;
				}
				wtask->mutex = (mutex_pt) param_p[i];
				break;
			#endif /* !(UBINOS__UBIK__EXCLUDE_CONDV == 1) */
			#if !(UBINOS__UBIK__EXCLUDE_MSGQ == 1)
			case OBJTYPE__UBIK_MSGQ:
				wtask->msg = (unsigned char *) param_p[i];
				break;
			#endif /* !(UBINOS__UBIK__EXCLUDE_MSGQ == 1) */
			}

			wtask->sigtype	= SIGOBJ_SIGTYPE__FAIL;

			_sigobj_wtasklist_insertprev(sigobj, NULL, wtask);
			_sigobj_wtasklist_notifychange(sigobj);
		}
	}
	needlock = 1;

	//TODO: (Ubinos) idle task 에서도 사용 가능하게 수정해야함

	_task_cur->state =  TASK_STATE__BLOCKED;
	_task_changelist(_task_cur);

	_task_yield();

	_task_cur->wtask_count		= 0;

	_task_cur->waitall 			= 0;

	r = 0;

end1:
	ubik_exitcrit();

	if (0 != needlock) {
		for (i=0; i<count; i++) {
			wtask = &_task_cur->wtask_p[i];
			sigtype_p[i] = wtask->sigtype;

			if (NULL != wtask->mutex && OBJTYPE__UBIK_MUTEX == ((_sigobj_pt) (wtask->mutex))->type) {
				task_timed_old = _task_cur->timed;
				_task_cur->timed = 0;
				r2 = mutex_lock(wtask->mutex);
				_task_cur->timed = task_timed_old;
				if (0 != r2) {
					logme("fail at mutex_lock()");
					r = -4;
				}
			}

			wtask->msg = NULL;
			wtask->mutex = NULL;

			if (SIGOBJ_SIGTYPE__TIMEOUT == wtask->sigtype) {
				r = SIGOBJ_SIGTYPE__TIMEOUT;
			}
		}
	}
	else {
		for (i=0; i<count; i++) {
			wtask = &_task_cur->wtask_p[i];
			sigtype_p[i] = wtask->sigtype;
		}
	}

end0:
	return r;
}

int task_waitforsigobjs_timed(void ** _sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt, unsigned int tick) {
	int r;

	logmfv("sigobj_p 0x%x, sigtype_p 0x%x, param_p 0x%x, count %d, waitopt 0x%x, tick %d",
			_sigobj_p, sigtype_p, param_p, count, waitopt, tick);

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

	r = task_waitforsigobjs(_sigobj_p, sigtype_p, param_p, count, waitopt);

	_task_cur->timed		= 0;

end0:
	return r;
}

int task_waitforsigobjs_timedms(void ** _sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt, unsigned int timems) {
	logmfv("sigobj_p 0x%x, sigtype_p 0x%x, param_p 0x%x, count %d, waitopt 0x%x, timems %d",
			_sigobj_p, sigtype_p, param_p, count, waitopt, timems);

	return task_waitforsigobjs_timed(_sigobj_p, sigtype_p, param_p, count, waitopt, ubik_timemstotick(timems));
}

#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
int task_join(task_pt * task_p, int * result_p, int count) {
	int r;
	int r2;
	_task_pt task;
	void ** sigobj_p = NULL;
	int * resultbuf = NULL;
	int i;
	int maxwaitcount_old;
	unsigned int task_sysflag01_old;

	logmfv("task_p 0x%x, result_p 0x%x, count %d", task_p, result_p, count);

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

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
		goto end0;
	}

	if (0 != _ubik_tasklockcount) {
		logme("task is locked");
		r = -1;
		goto end0;
	}

	if (NULL == task_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	if (0 >= count) {
		logme("parameter 3 is wrong");
		r = -4;
		goto end0;
	}

	maxwaitcount_old = task_getmaxwaitsigobj(NULL);
	if (maxwaitcount_old < count) {
		r = task_setmaxwaitsigobj(NULL, count);
		if (0 != r) {
			logme("fail at task_setmaxwaitsigobj()");
			r = -1;
			goto end0;
		}
	}

	sigobj_p = malloc(sizeof(void *) * count);
	if (NULL == sigobj_p) {
		logme("fail at malloc()");
		r = -1;
		goto end1;
	}

	if (NULL == result_p) {
		resultbuf = malloc(sizeof(int) * count);
		if (NULL == resultbuf) {
			logme("fail at malloc()");
			free(sigobj_p);
			r = -1;
			goto end1;
		}
		result_p = resultbuf;
	}

	ubik_entercrit();

	for (i=0; i<count; i++) {
		task = (_task_pt) (task_p[i]);
		if (NULL == task || 0 == task->valid || OBJTYPE__UBIK_TASK != task->type) {
			sigobj_p[i] = NULL;
		}
		else {
			sigobj_p[i] = task->monitor;
		}
	}

	task_sysflag01_old = _task_cur->sysflag01;
	_task_cur->sysflag01 = 1;
	r = task_waitforsigobjs(sigobj_p, result_p, NULL, count, TASK_WAITOPT__ALL | TASK_WAITOPT__IGNOREINVALID);
	_task_cur->sysflag01 = task_sysflag01_old;

	ubik_exitcrit();

	r = 0;
	for (i=0; i<count; i++) {
		if (	SIGOBJ_SIGTYPE__TERMINATED 	!= result_p[i] &&
				SIGOBJ_SIGTYPE__TIMEOUT 	!= result_p[i] &&
				-2 							!= result_p[i]		)
		{
			r = -1;
			break;
		}
	}

	free(sigobj_p);
	if (NULL != resultbuf) {
		free(resultbuf);
	}

end1:
	if (maxwaitcount_old < count) {
		r2 = task_setmaxwaitsigobj(NULL, maxwaitcount_old);
		if (0 != r2) {
			logme("fail at task_setmaxwaitsigobj()");
			r = -1;
		}
	}

end0:
	return r;
}

int task_join_timed(task_pt * task_p, int * result_p, int count, unsigned int tick) {
	int r;

	logmfv("task_p 0x%x, result_p 0x%x, count %d tick %d", task_p, result_p, count, tick);

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
	_task_cur->waittick 	= tick;

	r = task_join(task_p, result_p, count);

	_task_cur->timed		= 0;

end0:
	return r;
}

int task_join_timedms(task_pt * task_p, int * result_p, int count, unsigned int timems) {
	logmfv("task_p 0x%x, result_p 0x%x, count %d timems %d", task_p, result_p, count, timems);

	return task_join_timed(task_p, result_p, count, ubik_timemstotick(timems));
}

int task_join_and_delete(task_pt * task_p, int * result_p, int count) {
    int r = 0;

    do {
        r = task_join(task_p, result_p, count);
        if (r != 0) {
            logmfe("fail at task_join(), err=%d", r);
            break;
        }

        for (int i = 0; i < count; i++) {
            r = task_delete(&task_p[i]);
            if (r != 0) {
                logmfe("fail at task_delete(0x%x), err=%d", task_p[i], r);
                break;
            }
        }

        break;
    } while (1);

    return r;
}

int task_join_and_delete_timed(task_pt * task_p, int * result_p, int count, unsigned int tick) {
    int r;

    logmfv("task_p 0x%x, result_p 0x%x, count %d tick %d", task_p, result_p, count, tick);

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

    _task_cur->timed        = 1;
    _task_cur->waittick     = tick;

    r = task_join_and_delete(task_p, result_p, count);

    _task_cur->timed        = 0;

end0:
    return r;
}

int task_join_and_delete_timedms(task_pt * task_p, int * result_p, int count, unsigned int timems) {
    logmfv("task_p 0x%x, result_p 0x%x, count %d timems %d", task_p, result_p, count, timems);

    return task_join_and_delete_timed(task_p, result_p, count, ubik_timemstotick(timems));
}
#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

task_pt task_getcur(void) {
	if (0 == _bsp_kernel_active) {
		return NULL;
	}

	return (task_pt) _task_cur;
}

unsigned int task_getremainingtimeout(void) {
	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		return 0;
	}

	return _task_cur->waittick;
}

unsigned int task_getremainingtimeoutms(void) {
	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		return 0;
	}

	return ubik_ticktotimems(_task_cur->waittick);
}

unsigned int task_getminstackdepth(void) {
	return UBINOS__UBIK__TASK_STACK_DEPTH_MIN;
}

unsigned int task_getdefaultstackdepth(void) {
	return UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT;
}

void _task_init(_task_pt task) {
	task->type 					= OBJTYPE__UBIK_TASK;
	task->valid					= 0;
	task->reserved1				= 0;

	task->reserved2				= 0;

	task->stacktop				= NULL;
	task->stacktop_max			= NULL;
	task->stacklimit			= NULL;
	task->stack					= NULL;
	task->stacksize				= 0;

	edlist_link_init(&task->tasklist_link);

	edlist_link_init(&task->wtask.link);
	task->wtask.task			= task;
	task->wtask.sigtype			= SIGOBJ_SIGTYPE__FAIL;
	task->wtask.msg				= NULL;
	task->wtask.mutex			= NULL;

	task->wtask_p				= &task->wtask;
	task->wtask_max				= 1;
	task->wtask_count			= 0;
	task->wtask_waitcount		= 0;
	task->wtask_recvcount		= 0;

	edlist_init(&task->osigobjlist);
	task->osigobjlist.data		= task;

	task->state					= TASK_STATE__READY;
	task->suspended				= 0;
	task->timed					= 0;
	task->sysflag01				= 0;
	task->waitall				= 0;
	task->noautodel		= 0;
	task->reserved3				= 0;

	task->priority				= 0;
	task->priority_ori			= 0;

	task->reserved4				= 0;

	task->waittick				= 0;
	task->wakeuptick			= 0;

	task->func					= NULL;

	#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
	task->monitor				= NULL;
	#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

	task->name[0]				= 0;
	task->name[UBINOS__UBIK__TASK_NAME_SIZE_MAX] = 0;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	edlist_link_init(&task->kernel_monitor_tasklist_link);
	#endif
}

void _task_idlefunc(void * arg) {
	for (;;) {

#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_GARBAGECOLLECT == 1)
		ubik_collectgarbage();
#endif

#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
		if (NULL != _task_idletaskhookfunc_func) {
			_task_idletaskhookfunc_func(_task_idletaskhookfunc_arg);
			if (0 != (_task_idletaskhookfunc_option & IDLEHOOKFUNC_OPT__ONCE)) {
				ubik_entercrit();
				_task_idletaskhookfunc_func		= NULL;
				_task_idletaskhookfunc_arg		= NULL;
				_task_idletaskhookfunc_option	= 0;
				_task_idletaskhookfunc_name[0]	= 0;
				ubik_exitcrit();
			}
		}
#endif

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#if (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1)
		_ubik_idle_cpu_sleep();
#endif /* (UBINOS__UBIK__TICK_RTC_SLEEP_IDLE == 1) */
#endif /* (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC) */
	}
}

void _task_rootfunc(void * arg) {
	(*_task_cur->func)(arg);
	task_delete(NULL);
	bsp_abortsystem(); // unreachable control flow
}

void _task_changelist(_task_pt task) {
	edlist_pt tasklist;
	_task_pt ref;

	if (0 == task->suspended) {
		switch(task->state) {
		case TASK_STATE__READY:
		case TASK_STATE__RUNNING:
			tasklist = &_task_list_ready_a[task->priority];
			if (_task_list_ready_index < task->priority) {
				_task_list_ready_index = task->priority;
				_task_list_ready_cur = &_task_list_ready_a[_task_list_ready_index];
			}
			break;
		case TASK_STATE__BLOCKED:
			if (OBJTYPE__UBIK_IDLETASK  == task->type) {
				logme("idle task is blocked");
				bsp_abortsystem();
			}
			if (0 == task->timed) {
				tasklist = &_task_list_blocked;
			}
			else {
				if (_ubik_tickcount >= task->wakeuptick) {
					tasklist = _task_list_blocked_timed_next;
				}
				else {
					tasklist = _task_list_blocked_timed_cur;
				}
			}
			break;
		case TASK_STATE__TERMINATED:
			if (OBJTYPE__UBIK_IDLETASK  == task->type) {
				logme("idle task is terminated");
				bsp_abortsystem();
			}
			if (task->noautodel == 1) {
				tasklist = &_task_list_terminated_noautodel;
			}
			else {
				tasklist = &_task_list_terminated;
			}

			break;
		default:
			if (OBJTYPE__UBIK_IDLETASK  == task->type) {
				logme("idle task is unknown state");
				bsp_abortsystem();
			}

			return;
		}
	}
	else {
		if (OBJTYPE__UBIK_IDLETASK  == task->type) {
			logme("idle task is suspended");
			bsp_abortsystem();
		}

		tasklist = &_task_list_suspended;
	}

	if (_tasklist_list(task) != tasklist) {
		_tasklist_remove(task);
		if (TASK_STATE__BLOCKED == task->state && 0 != task->timed) {
			ref =  _tasklist_head(tasklist);
			for (;;) {
				if (NULL == ref || task->wakeuptick < ref->wakeuptick) {
					break;
				}
				ref = _tasklist_next(ref);
			}
		}
		else {
			ref = (_task_pt) tasklist->cur;
		}
		_tasklist_insertprev(tasklist, ref,  task);

		for (;;) {
			if (0 != _task_list_ready_cur->count) {
				break;
			}
			_task_list_ready_index--;
			_task_list_ready_cur = &_task_list_ready_a[_task_list_ready_index];
		}
	}
}

void _task_applypriority(_task_pt task) {
	int priority = 0;
	#if !(UBINOS__UBIK__EXCLUDE_TASK_PRIORITY_INHERITANCE == 1)
	_sigobj_pt osigobj;

	if (0 == task->suspended) {
		for (osigobj = _task_osigobjlist_head(task); NULL != osigobj; osigobj = _task_osigobjlist_next(osigobj)) {
			if (priority < osigobj->wtasklist_maxpriority) {
				priority = osigobj->wtasklist_maxpriority;
			}
		}

		if (priority < task->priority_ori) {
			priority = task->priority_ori;
		}

		if (task->priority != priority) {
			task->priority = priority;
			_task_sigobj_notifywtaskchange(task);
			_task_changelist(task);
		}
	}
	#else
	if (0 == task->suspended) {
		priority = task->priority_ori;

		if (task->priority != priority) {
			task->priority = priority;
			_task_sigobj_notifywtaskchange(task);
			_task_changelist(task);
		}
	}
	#endif
}

void _task_collectgarbage(void) {
	_task_pt 	task;

	if (0 < _task_list_terminated.count) {
		for (;;) {
			ubik_entercrit();

			task = _tasklist_head(&_task_list_terminated);
			if (NULL == task) {
				ubik_exitcrit();
				break;
			}
			_tasklist_remove(task);

			if (_task_prev == task) {
				_task_prev = NULL;
			}

			ubik_exitcrit();

			#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
			if (NULL != task->monitor) {
				signal_delete(&task->monitor);
			}
			#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

			if (&task->wtask != task->wtask_p && NULL != task->wtask_p) {
				free(task->wtask_p);
			}
			free(task->stack);
			free(task);
		}
	}
}

void _task_sigobj_removewtask(_task_pt task) {
	int i;
	_wtask_pt wtask;
	_sigobj_pt sigobj;

	for (i=0; i<task->wtask_count; i++) {
		wtask = &task->wtask_p[i];
		sigobj = _sigobj_wtasklist_owner(wtask);
		if (NULL != sigobj) {
			_sigobj_wtasklist_remove(wtask);
			_sigobj_wtasklist_notifychange(sigobj);
		}
	}
}

#if !(UBINOS__UBIK__EXCLUDE_TASK_PRIORITY_INHERITANCE == 1)
void _task_sigobj_notifywtaskchange(_task_pt task) {
	int i;
	_wtask_pt wtask;
	_sigobj_pt sigobj;

	for (i=0; i<task->wtask_count; i++) {
		wtask = &task->wtask_p[i];
		sigobj = _sigobj_wtasklist_owner(wtask);
		if (NULL != sigobj) {
			_sigobj_wtasklist_notifychange(sigobj);
		}
	}
}
#endif

#endif /* (INCLUDE__UBINOS__UBIK == 1) */
