/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>
#include <stdio.h>
#include <string.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBIK

#if 	(0 >= UBINOS__UBIK__TASK_NAME_SIZE_MAX)
	#error "UBINOS__UBIK__TASK_NAME_SIZE_MAX should be larger than 0."
#endif

#if 	(9 > UBINOS__UBIK__TASK_PRIORITY_MAX)
	#error "UBINOS__UBIK__TASK_PRIORITY_MAX should be equal or larger than 9."
#endif

#if 	(INT_MAX <= UBINOS__UBIK__TASK_PRIORITY_MAX)
	#error "UBINOS__UBIK__TASK_PRIORITY_MAX should be less than INT_MAX."
#endif

#if     (16 < UBINOS__UBIK__TASK_MAXWAITSIGOBJ_MAX)
    #error "UBINOS__UBIK__TASK_MAXWAITSIGOBJ_MAX should be equal or less than 16."
#endif

#if		(56 > UBINOS__UBIK__TASK_STACK_DEPTH_MIN)
	#error "UBINOS__UBIK__TASK_STACK_DEPTH_MIN should be equal or larger than 56."
#endif

#if		(UBINOS__UBIK__TASK_STACK_DEPTH_MIN > UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT)
	#error "UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT should be equal or larger than UBINOS__UBIK__TASK_STACK_DEPTH_MIN."
#endif

#if		(32 < UBINOS__UBIK__MSGQ_MSGSIZE_MAX)
	#error "UBINOS__UBIK__MSGQ_MSGSIZE_MAX should be equal or less than 32."
#endif

#if     (1 > UBINOS__UBIK__IDLETASK_SPINWAIT_INTERVALTICK)
    #error "UBINOS__UBIK__IDLETASK_SPINWAIT_INTERVALTICK should be equal or larger than 1."
#endif

/*****************************************************************************/

volatile unsigned int	_ubik_tickcount								= 0;		/* system tick count */
volatile unsigned int	_ubik_tickcounth							= 0;		/* system tick count high value */
volatile unsigned int	_ubik_tasklockcount							= 0;		/* task lock nesting count */
volatile unsigned int	_ubik_rtpriority							= 0;		/* lowest priority of task considered as a real-time task */

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#if (UBINOS__UBIK__TICK_RTC_CHECK == 1)
volatile unsigned int	_ubik_tickrtccount							= 0;		/* RTC count for tick */
volatile int			_ubik_tickrtccount_init						= 0;		/* Is RTC count for tick initialized */
#endif
#endif

		 edlist_t		_task_list_blocked_timed_a[2]				= {{0,},};	/* timed blocked task list array */
volatile edlist_pt		_task_list_blocked_timed_cur 				= NULL;		/* current timed blocked task list pointer */
volatile edlist_pt		_task_list_blocked_timed_next 				= NULL;		/* next timed blocked task list pointer */
		 edlist_t		_task_list_blocked							= {0,};		/* blocked forever task list */
		 edlist_t		_task_list_ready_a[UBINOS__UBIK__TASK_PRIORITY_MAX+1]		= {{0,},};	/* ready task list array */
volatile unsigned int 	_task_list_ready_index 						= 0;		/* current ready task list index */
volatile edlist_pt		_task_list_ready_cur 						= NULL;		/* current ready task list pointer */
		 edlist_t		_task_list_suspended						= {0,};		/* suspended task list */
		 edlist_t		_task_list_terminated						= {0,};		/* list of terminated task (these tasks are deleted by idle task automotically) */
		 edlist_t		_task_list_terminated_noautodel			= {0,};		/* list of terminated task that should be deleted by user (a task to join must be this type) */
volatile _task_pt 		_task_cur 									= NULL;		/* current running task pointer */
volatile _task_pt 		_task_prev									= NULL;		/* previous ran task pointer */

#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
		 edlist_t		_stimer_list_a[2]							= {{0,},};	/* stimer list array */
volatile edlist_pt		_stimer_list_cur 							= NULL;		/* current stimer list pointer */
volatile edlist_pt		_stimer_list_next 							= NULL;		/* next stimer list pointer */
#endif

#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
		 mutex_pt		_kernel_monitor_mutex						= NULL;
		 edlist_t		_kernel_monitor_tasklist					= {0,};
		 edlist_t		_kernel_monitor_stimerlist					= {0,};
		 edlist_t		_kernel_monitor_sigobjlist					= {0,};
#endif

#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
		idletaskhookfunc_ft	_task_idletaskhookfunc_func							= NULL;
		void * 				_task_idletaskhookfunc_arg							= NULL;
		unsigned int		_task_idletaskhookfunc_option						= 0;
		char				_task_idletaskhookfunc_name[UBINOS__UBIK__TASK_NAME_SIZE_MAX + 1]		= {0,};
#endif

#if !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1)
		tickhookfunc_ft		_ubik_tickhookfunc_func								= NULL;
#endif /* !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1) */

#if !(UBINOS__UBIK__EXCLUDE_HRTICK == 1)
volatile unsigned int	_ubik_hrtick_active							= 0;		/* variable representing whether high resolution tick is active or not */
volatile unsigned int   _ubik_hrtick_hrtickpertick                  = UINT_MAX;
#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK == 1) */

/*****************************************************************************/

int ubik_comp_init(unsigned int idle_stackdepth) {
	int r;

	_ubik_tasklockcount	= 0;
	_ubik_rtpriority	= INT_MAX;

	_task_list_blocked_timed_cur 	= NULL;
	_task_list_blocked_timed_next 	= NULL;
	_task_list_ready_index 			= 0;
	_task_list_ready_cur 			= NULL;

	_task_cur 						= NULL;
	_task_prev 						= NULL;

	#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
	_stimer_list_cur 				= NULL;
	_stimer_list_next 				= NULL;
	#endif

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	edlist_init(&_kernel_monitor_tasklist);
	edlist_init(&_kernel_monitor_stimerlist);
	edlist_init(&_kernel_monitor_sigobjlist);
	#endif

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_create(&_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		goto end0;
	}
	#endif

	r = _ubik_port_comp_init(idle_stackdepth);
	if (r != 0) {
		logme("fail at _ubik_port_comp_init");
		goto end0;
	}

	#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
	edlist_init(&_stimer_list_a[0]);
	edlist_init(&_stimer_list_a[1]);
	_stimer_list_cur  	= &_stimer_list_a[0];
	_stimer_list_next 	= &_stimer_list_a[1];
	#endif

	#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
	_task_idletaskhookfunc_func		= NULL;
	_task_idletaskhookfunc_arg		= NULL;
	_task_idletaskhookfunc_option	= 0;
	_task_idletaskhookfunc_name[0]	= 0x00;
	#endif

	#if !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1)
	_ubik_tickhookfunc_func			= NULL;
	#endif /* !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1) */

	#if !(UBINOS__UBIK__EXCLUDE_HRTICK == 1)
	_ubik_hrtick_active				= 0;
	#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK == 1) */

	r = 0;

end0:
	return r;
}

int ubik_comp_start(void) {

	_ubik_port_comp_start();

	return 0;
}

int ubik_collectgarbage(void) {
	_task_collectgarbage();

	return 0;
}

int ubik_sprintkernelinfo(char * buf, int max) {
#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r;
	int r2;
	_task_pt 	task;
	_stimer_pt 	stimer;
	_sigobj_pt 	sigobj;
	unsigned int stacksize;
	unsigned int stacksizedepth;
	unsigned int maxstackusage;
	unsigned int maxstackusagedepth;

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

	if (NULL == buf) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	if (max <= 0) {
		r = 0;
		goto end0;
	}

	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}

	r = 0;
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|task    |address    |function   |stack size   |max stack    |priority   |state   |suspended |timed |name               \n");
	if (r < max) r += snprintf(&buf[r], max-r, "|type    |           |address    |(byte(depth))|usage        |(original) |        |          |      |                   \n");
	if (r < max) r += snprintf(&buf[r], max-r, "|        |           |           |             |(byte(depth))|           |        |          |      |                   \n");
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");

	bsp_getstacksize(1, &stacksize);
	stacksizedepth = stacksize/INT_SIZE;
	if (0 != stacksize%INT_SIZE) {
		stacksizedepth++;
	}
	r2 = bsp_getmaxstackusage(1, &maxstackusage);
	maxstackusagedepth = maxstackusage/INT_SIZE;
	if (0 != maxstackusage%INT_SIZE) {
		maxstackusagedepth++;
	}
	if (0 != r2) {
		maxstackusage = 0;
		maxstackusagedepth = 0;
	}
	if (r < max) r += snprintf(&buf[r], max-r, "|svc     |           |           | 0x%08x  | 0x%08x  |           |        |          |      |        \n", stacksize, maxstackusage);
	if (r < max) r += snprintf(&buf[r], max-r, "|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \n",
			stacksizedepth, maxstackusagedepth);

	task = _kernel_monitor_tasklist_head(&_kernel_monitor_tasklist);
	for (; NULL != task; task = _kernel_monitor_tasklist_next(task)) {
		stacksize = task->stacksize;
		stacksizedepth = stacksize/INT_SIZE;
		if (0 != stacksize%INT_SIZE) {
			stacksizedepth++;
		}
		r2 = task_getmaxstackusage((task_pt) task, &maxstackusage);
		maxstackusagedepth = maxstackusage/INT_SIZE;
		if (0 != maxstackusage%INT_SIZE) {
			maxstackusagedepth++;
		}
		if (0 != r2) {
			maxstackusage = 0;
			maxstackusagedepth = 0;
		}
		if (r < max) r += snprintf(&buf[r], max-r, "|task    |0x%08x |0x%08x | 0x%08x  | 0x%08x  |%04d(%04d) |",
				(unsigned int) task, (unsigned int) (task->func), stacksize, maxstackusage, task->priority, task->priority_ori);
		switch (task->state) {
		case TASK_STATE__READY:
			if (r < max) r += snprintf(&buf[r], max-r, "ready  ");
			break;
		case TASK_STATE__RUNNING:
			if (r < max) r += snprintf(&buf[r], max-r, "running");
			break;
		case TASK_STATE__BLOCKED:
			if (r < max) r += snprintf(&buf[r], max-r, "blocked");
			break;
		default:
			if (r < max) r += snprintf(&buf[r], max-r, "unknown");
			break;
		}
		if (r < max) r += snprintf(&buf[r], max-r, " |%d         |%d     |%s",
				task->suspended, task->timed, task->name);
#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
		if (OBJTYPE__UBIK_IDLETASK  == task->type && NULL != _task_idletaskhookfunc_func) {
			if (r < max) r += snprintf(&buf[r], max-r, " (%s)", _task_idletaskhookfunc_name);
		}
#endif
		if (r < max) r += snprintf(&buf[r], max-r, "\n");

		if (r < max) r += snprintf(&buf[r], max-r, "|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \n",
				stacksizedepth, maxstackusagedepth);
	}
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	if (r < max) r += snprintf(&buf[r], max-r, "\n");

	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|timer   |address    |semaphore  |period     |oneshot |running                                      \n");
	if (r < max) r += snprintf(&buf[r], max-r, "|type    |           |address    |(tick)     |        |                                             \n");
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	stimer = _kernel_monitor_stimerlist_head(&_kernel_monitor_stimerlist);
	for (; NULL != stimer; stimer = _kernel_monitor_stimerlist_next(stimer)) {
		if (r < max) r += snprintf(&buf[r], max-r, "|stimer  |0x%08x |0x%08x |0x%08x |%d       |%d\n",
				(unsigned int) stimer, (unsigned int) (stimer->sigobj), stimer->waittick, stimer->oneshot, stimer->running);
	}
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	if (r < max) r += snprintf(&buf[r], max-r, "\n");

	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|sigobj  |address    |waiting |max      |sender     |count      |max        |etc                    \n");
	if (r < max) r += snprintf(&buf[r], max-r, "|type    |           |task    |priority |(owner)    |(size)     |count      |                       \n");
	if (r < max) r += snprintf(&buf[r], max-r, "|        |           |count   |         |address    |           |(size)     |                       \n");
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	sigobj = _kernel_monitor_sigobjlist_head(&_kernel_monitor_sigobjlist);
	for (; NULL != sigobj; sigobj = _kernel_monitor_sigobjlist_next(sigobj)) {
		switch (sigobj->type) {
		case OBJTYPE__UBIK_SIGNAL:
			if (r < max) r += snprintf(&buf[r], max-r, "|signal  |0x%08x |%04d    |%04d     |0x%08x |           |           |\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_CONDV:
			if (r < max) r += snprintf(&buf[r], max-r, "|condv   |0x%08x |%04d    |%04d     |0x%08x |           |           |\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_MUTEX:
			if (r < max) r += snprintf(&buf[r], max-r, "|mutex   |0x%08x |%04d    |%04d     |0x%08x |0x%08x |           |n: %d\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->nopriorityinheritance);
			break;
		case OBJTYPE__UBIK_SEM:
			if (r < max) r += snprintf(&buf[r], max-r, "|sem     |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |i: %d\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->ignoreoverflow);
			break;
		case OBJTYPE__UBIK_MSGQ:
			if (r < max) r += snprintf(&buf[r], max-r, "|msgq    |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |msg size: 0x%04x\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->msgsize_1 + 1);
			break;
		default:
			if (r < max) r += snprintf(&buf[r], max-r, "|unknown |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |type: 0x%04x\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->type);
			break;
		}
	}

	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\n");
	if (r < max) r += snprintf(&buf[r], max-r, "\n");

	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}

end0:
	return r;
#else
	int r;

	if (NULL == buf) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	if (max <= 0) {
		r = 0;
		goto end0;
	}

	buf[0] = 0;

	r = 0;

end0:
	return r;
#endif
}
int ubik_printkernelinfo(void) {
#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	int r;
	int r2;
	_task_pt 	task;
	_stimer_pt 	stimer;
	_sigobj_pt 	sigobj;
	unsigned int stacksize;
	unsigned int stacksizedepth;
	unsigned int maxstackusage;
	unsigned int maxstackusagedepth;

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

	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}

	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("|task    |address    |function   |stack size   |max stack    |priority   |state   |suspended |timed |name               \n");
	printf("|type    |           |address    |(byte(depth))|usage        |(original) |        |          |      |                   \n");
	printf("|        |           |           |             |(byte(depth))|           |        |          |      |                   \n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");

	bsp_getstacksize(1, &stacksize);
	stacksizedepth = stacksize/INT_SIZE;
	if (0 != stacksize%INT_SIZE) {
		stacksizedepth++;
	}
	r2 = bsp_getmaxstackusage(1, &maxstackusage);
	maxstackusagedepth = maxstackusage/INT_SIZE;
	if (0 != maxstackusage%INT_SIZE) {
		maxstackusagedepth++;
	}
	if (0 != r2) {
		maxstackusage = 0;
		maxstackusagedepth = 0;
	}
	printf("|svc     |           |           | 0x%08x  | 0x%08x  |           |        |          |      |        \n", stacksize, maxstackusage);
	printf("|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \n",
			stacksizedepth, maxstackusagedepth);

	task = _kernel_monitor_tasklist_head(&_kernel_monitor_tasklist);
	for (; NULL != task; task = _kernel_monitor_tasklist_next(task)) {
		stacksize = task->stacksize;
		stacksizedepth = stacksize/INT_SIZE;
		if (0 != stacksize%INT_SIZE) {
			stacksizedepth++;
		}
		r2 = task_getmaxstackusage((task_pt) task, &maxstackusage);
		maxstackusagedepth = maxstackusage/INT_SIZE;
		if (0 != maxstackusage%INT_SIZE) {
			maxstackusagedepth++;
		}
		if (0 != r2) {
			maxstackusage = 0;
			maxstackusagedepth = 0;
		}
		printf("|task    |0x%08x |0x%08x | 0x%08x  | 0x%08x  |%04d(%04d) |",
				(unsigned int) task, (unsigned int) (task->func), stacksize, maxstackusage, task->priority, task->priority_ori);
		switch (task->state) {
		case TASK_STATE__READY:
			printf("ready  ");
			break;
		case TASK_STATE__RUNNING:
			printf("running");
			break;
		case TASK_STATE__BLOCKED:
			printf("blocked");
			break;
		default:
			printf("unknown");
			break;
		}
		printf(" |%d         |%d     |%s",
				task->suspended, task->timed, task->name);
#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
		if (OBJTYPE__UBIK_IDLETASK  == task->type && NULL != _task_idletaskhookfunc_func) {
			printf(" (%s)", _task_idletaskhookfunc_name);
		}
#endif
		printf("\n");
		printf("|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \n",
				stacksizedepth, maxstackusagedepth);
	}
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n");

	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("|timer   |address    |semaphore  |period     |oneshot |running                                      \n");
	printf("|type    |           |address    |(tick)     |        |                                             \n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	stimer = _kernel_monitor_stimerlist_head(&_kernel_monitor_stimerlist);
	for (; NULL != stimer; stimer = _kernel_monitor_stimerlist_next(stimer)) {
		printf("|stimer  |0x%08x |0x%08x |0x%08x |%d       |%d\n",
				(unsigned int) stimer, (unsigned int) (stimer->sigobj), stimer->waittick, stimer->oneshot, stimer->running);
	}
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n");

	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("|sigobj  |address    |waiting |max      |sender     |count      |max        |etc                    \n");
	printf("|type    |           |task    |priority |(owner)    |(size)     |count      |                       \n");
	printf("|        |           |count   |         |address    |           |(size)     |                       \n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	sigobj = _kernel_monitor_sigobjlist_head(&_kernel_monitor_sigobjlist);
	for (; NULL != sigobj; sigobj = _kernel_monitor_sigobjlist_next(sigobj)) {
		switch (sigobj->type) {
		case OBJTYPE__UBIK_SIGNAL:
			printf("|signal  |0x%08x |%04d    |%04d     |0x%08x |           |           |\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_CONDV:
			printf("|condv   |0x%08x |%04d    |%04d     |0x%08x |           |           |\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_MUTEX:
			printf("|mutex   |0x%08x |%04d    |%04d     |0x%08x |0x%08x |           |n: %d\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->nopriorityinheritance);
			break;
		case OBJTYPE__UBIK_SEM:
			printf("|sem     |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |i: %d\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->ignoreoverflow);
			break;
		case OBJTYPE__UBIK_MSGQ:
			printf("|msgq    |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |msg size: 0x%04x\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->msgsize_1 + 1);
			break;
		default:
			printf("|unknown |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |type: 0x%04x\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->type);
			break;
		}
	}

	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n");

	r = 0;

	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}

end0:
	return r;
#else
	return 0;
#endif
}

unsigned int _sigobj_wtasklist_getmaxpriority(_sigobj_pt sigobj) {
	_wtask_pt wtask;
	_task_pt task;
	int priority = 0;

	for (wtask = _sigobj_wtasklist_head(sigobj); NULL != wtask; wtask = _sigobj_wtasklist_next(wtask)) {
		task = wtask->task;
		if (0 == task->suspended && priority < task->priority) {
			priority = task->priority;
		}
	}

	return priority;
}

_wtask_pt _sigobj_wtasklist_gettask(_sigobj_pt sigobj) {
	_wtask_pt wtask;
	_task_pt task;
	int priority = 0;
	_wtask_pt mpwtask = NULL;

	for (wtask = _sigobj_wtasklist_head(sigobj); NULL != wtask; wtask = _sigobj_wtasklist_next(wtask)) {
		task = wtask->task;
		if (0 == task->suspended && (mpwtask == NULL || priority < task->priority)) {
			priority = task->priority;
			mpwtask = wtask;
		}
	}

	return mpwtask;
}

#if !(UBINOS__UBIK__EXCLUDE_TASK_PRIORITY_INHERITANCE == 1)
void _sigobj_wtasklist_notifychange(_sigobj_pt sigobj) {
	int priority;
	_task_pt task;

	priority = _sigobj_wtasklist_getmaxpriority(sigobj);
	if (sigobj->wtasklist_maxpriority != priority) {
		sigobj->wtasklist_maxpriority = priority;
		task = _task_osigobjlist_owner(sigobj);
		if (NULL != task) {
			_task_applypriority(task);
		}
	}
}
#endif

#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
void _stimerlist_add(_stimer_pt stimer) {
	edlist_pt stimerlist;
	_stimer_pt ref;

	_stimerlist_remove(stimer);

	stimer->wakeuptick = _ubik_tickcount + stimer->waittick;

	if (_ubik_tickcount >= stimer->wakeuptick) {
		stimerlist = _stimer_list_next;
	}
	else {
		stimerlist = _stimer_list_cur;
	}

	ref =  _stimerlist_head(stimerlist);
	for (;;) {
		if (NULL == ref || stimer->wakeuptick < ref->wakeuptick) {
			break;
		}
		ref = _stimerlist_next(ref);
	}

	_stimerlist_insertprev(stimerlist, ref,  stimer);
}
#endif

#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
int ubik_setidletaskhookfunc(idletaskhookfunc_ft func, void * arg, const char * name, unsigned int option) {
	ubik_entercrit();
	_task_idletaskhookfunc_func		= func;
	_task_idletaskhookfunc_arg		= arg;
	_task_idletaskhookfunc_option	= option;
	_task_idletaskhookfunc_name[0]	= 0x00;
	ubik_exitcrit();

	if (NULL != name) {
		strncpy(_task_idletaskhookfunc_name, name, UBINOS__UBIK__TASK_NAME_SIZE_MAX);
	}

	return 0;
}
#endif

#if !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1)
int ubik_settickhookfunc(tickhookfunc_ft tickhookfunc) {
	ubik_entercrit();
	_ubik_tickhookfunc_func			= tickhookfunc;
	ubik_exitcrit();

	return 0;
}
#endif /* !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1) */

int ubik_setrtpriority(int rtpriority) {
	if (0 >= rtpriority) {
		return -2;
	}
	
	_ubik_rtpriority = rtpriority;

	return 0;
}

int ubik_getrtpriority(void) {
	return _ubik_rtpriority;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */
