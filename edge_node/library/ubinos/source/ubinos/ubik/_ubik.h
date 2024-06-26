/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UBIK_H_
#define _UBIK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file _ubik.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 내부 인터페이스
 *
 * ubik 컴포넌트 내부 인터페이스를 정의합니다.
 */

#include <ubinos/bsp.h>
#include <ubinos/bsp/arch.h>
#include <ubinos/ubiclib.h>
#include <ubinos/ubik.h>

/*****************************************************************************/

/**
 * ubik port 컴포넌트를 초기화하는 함수
 *
 * 이 함수는 ubik_comp_init 함수에서 자동적으로 호출됩니다.
 *
 * @param	idle_stackdepth: idle task 스택 깊이(스택 영역 크기는 stackdepth * INT_SIZE 바이트가 됨)<br>
 * 			0: 기본  스택 깊이 값(task_getdefaultstackdepth 함수가 돌려주는 값)<br>
 * 			1 ~ 최저 스택 깊이 값: 최저 스택 깊이 값(task_getminstackdepth 함수가 돌려주는 값)<br>
 * 			<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int _ubik_port_comp_init(unsigned int idle_stackdepth);

/**
 * ubik port 컴포넌트를 시작하는 함수
 */
void _ubik_port_comp_start(void);

/**
 * tick을 초기화 하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int _ubik_inittick(void);

/**
 * 소프트웨어 인터럽트 서비스 루틴
 */
void _ubik_swisr(unsigned int swino);

/*****************************************************************************/

#define 				TASK_PRIORITY__IDLE				0

#define					TASK_STATE__READY				0
#define					TASK_STATE__RUNNING				1
#define					TASK_STATE__BLOCKED				2
#define					TASK_STATE__TERMINATED			3

/** @define 시그널 종류: 성공 */
#define					SIGOBJ_SIGTYPE__SUCCESS			UBIK_ERR__SUCCESS
/** @define 시그널 종류: 실패 */
#define					SIGOBJ_SIGTYPE__FAIL			UBIK_ERR__FAIL
/** @define 시그널 종류: 제한 시간 초과 */
#define					SIGOBJ_SIGTYPE__TIMEOUT			UBIK_ERR__TIMEOUT
/** @define 시그널 종류: 시그널을 보낸 객체가 삭제되었음 */
#define					SIGOBJ_SIGTYPE__TERMINATED		UBIK_ERR__TERMINATED

/**
 * task 컴포넌트를 초기화하는 함수
 *
 * @param	idle_stackdepth: idle task 스택 깊이(스택 영역 크기는 stackdepth * INT_SIZE 바이트가 됨)<br>
 * 			0: 기본  스택 깊이 값(task_getdefaultstackdepth 함수가 돌려주는 값)<br>
 * 			1 ~ 최저 스택 깊이 값: 최저 스택 깊이 값(task_getminstackdepth 함수가 돌려주는 값)<br>
 * 			<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int task_comp_init(unsigned int idle_stackdepth);

/**
 * task 컴포넌트를 시작하는 함수
 */
void task_comp_start(void);

/*****************************************************************************/

/*
 * 시그널을 보낼 수 있는 객체 형 구현
 */
typedef	struct __sigobj_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  7;

	unsigned int		reserved2				:  1;
	unsigned int		nopriorityinheritance	:  1;
	unsigned int		ignoreoverflow			:  1;
	unsigned int		msgsize_1				:  5;
	unsigned int		wtasklist_maxpriority	:  8;

	edlist_t			wtasklist;

	struct {
		struct __sigobj_t *	prev;
		struct __sigobj_t *	next;
		edlist_pt			list;
	}					osigobjlist_link;

	unsigned int		count;
	unsigned int		maxcount;
	cirbuf_pt			msgbuf;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	struct {
		struct __sigobj_t *	prev;
		struct __sigobj_t *	next;
		edlist_pt			list;
	}					kernel_monitor_sigobjlist_link;
	#endif
} _sigobj_t;

typedef _sigobj_t * _sigobj_pt;

/*
 * 태스크 객체 형 구현
 */
typedef	struct __task_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  7;

	unsigned int		reserved2				: 16;

	unsigned int * 		stacktop;   /* stacktop offset should be 4 */
	unsigned int * 		stacktop_max;
	unsigned int *		stacklimit;
	unsigned int *		stack;
	unsigned int		stacksize;

	struct {
		struct __task_t *	prev;
		struct __task_t *	next;
		edlist_pt			list;
	} 					tasklist_link;

	struct __wtask_t {
		struct {
			struct __wtask_t *	prev;
			struct __wtask_t *	next;
			edlist_pt			list;
		} link;
		struct __task_t *		task;
		int						sigtype;
		unsigned char *			msg;
		mutex_pt				mutex;
	} 					wtask;				/* 시그널 객체에 기다리는 태스크로 등록하기 위한 링크 */

	struct __wtask_t *	wtask_p;			/* 시그널 객체에 기다리는 태스크로 등록하기 위한 링크 배열 포인터 */
	int					wtask_max;			/* 이 태스크가 기다를 수 있는 시그널 객체 수 최대값 */
	int					wtask_count; 		/* 이 태스크가 기다리는 시그널 객체 수 (기다릴 수 없는 상태의 객체 수 포함) */
	int					wtask_waitcount;	/* 이 태스크가 실제로 기다리는 시그널 객체 수 (기다릴 수 없는 상태의 객체 수 제외) */
	int					wtask_recvcount;	/* 이 태스크가 시그널을 받은 객체 수 */

	edlist_t			osigobjlist;		/* 이 태스크가 소유한 시그널 객체 목록 */

	unsigned int		state					:  2;
	unsigned int		suspended				:  1;
	unsigned int		timed					:  1;
	unsigned int		sysflag01				:  1;
	unsigned int		waitall					:  1;
	unsigned int		noautodel				:  1;
	unsigned int		reserved3				:  9;

	unsigned int		priority				:  8;
	unsigned int		priority_ori			:  8;

	unsigned int		reserved4				: 16;

	unsigned int		waittick;
	unsigned int		wakeuptick;

	taskfunc_ft			func;

	#if !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1)
	signal_pt			monitor;
	#endif /* !(UBINOS__UBIK__EXCLUDE_TASK_MONITORING == 1) */

	char				name[UBINOS__UBIK__TASK_NAME_SIZE_MAX + 1];

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	struct {
		struct __task_t *	prev;
		struct __task_t *	next;
		edlist_pt			list;
	}					kernel_monitor_tasklist_link;
	#endif
} _task_t;

typedef _task_t * _task_pt;

/*
 * 시그널을 기다리는 태스크 객체 형 구현
 *
 * 태스크는 자신이 기다리는 시그널 객체 수 만큼 이 객체를 가진다.
 * 태스크는 자신이 기다리는 시그널 객체에게 이 객체의 포인터를 전달한다.
 * 시그널 객체는 자신을 기다리는 태스크에 이 객체를 사용해 접근한다.
 */
typedef struct __wtask_t _wtask_t;

typedef _wtask_t * _wtask_pt;

/*
 * 세마포어 타이머 객체 형 구현
 */
typedef	struct __stimer_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  7;

	unsigned int		reserved2				: 13;
	unsigned int		broadcast				:  1;
	unsigned int		oneshot					:  1;
	unsigned int		running					:  1;

	struct {
		struct __stimer_t *	prev;
		struct __stimer_t *	next;
		edlist_pt			list;
	}					stimerlist_link;

	unsigned int		waittick;
	unsigned int		wakeuptick;

	_sigobj_pt			sigobj;
	int					sigtype;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	struct {
		struct __stimer_t *	prev;
		struct __stimer_t *	next;
		edlist_pt			list;
	}					kernel_monitor_stimerlist_link;
	#endif
} _stimer_t;

typedef _stimer_t * _stimer_pt;

/*****************************************************************************/

#define _tasklist_insertprev(tasklist, ref, task)	edlist_insertprev(	_task_pt, tasklist_link, tasklist, ref, task)
#define _tasklist_insertnext(tasklist, ref, task)	edlist_insertnext(	_task_pt, tasklist_link, tasklist, ref, task)
#define _tasklist_remove(task)						edlist_remove(		_task_pt, tasklist_link, task)
#define _tasklist_getcur(tasklist)					edlist_getcur(		_task_pt, tasklist_link, tasklist)
#define _tasklist_getcurnext(tasklist)				edlist_getcurnext(	_task_pt, tasklist_link, tasklist)
#define _tasklist_head(tasklist)					edlist_head(		_task_pt, tasklist_link, tasklist)
#define _tasklist_tail(tasklist)					edlist_tail(		_task_pt, tasklist_link, tasklist)
#define _tasklist_next(task)						edlist_next(		_task_pt, tasklist_link, task)
#define _tasklist_prev(task)						edlist_prev(		_task_pt, tasklist_link, task)
#define _tasklist_list(task)						edlist_list(		_task_pt, tasklist_link, task)

/*****************************************************************************/

#define _sigobj_wtasklist_insertprev(sigobj, ref, wtask)	\
													edlist_insertprev(	_wtask_pt, link, &(sigobj)->wtasklist, ref, wtask)
#define _sigobj_wtasklist_insertnext(sigobj, ref, wtask)	\
													edlist_insertnext(	_wtask_pt, link, &(sigobj)->wtasklist, ref, wtask)
#define _sigobj_wtasklist_remove(wtask)				edlist_remove(		_wtask_pt, link, wtask)
#define _sigobj_wtasklist_getcur(sigobj)			edlist_getcur(		_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_getcurnext(sigobj)		edlist_getcurnext(	_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_head(sigobj)				edlist_head(		_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_tail(sigobj)				edlist_tail(		_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_next(wtask)				edlist_next(		_wtask_pt, link, wtask)
#define _sigobj_wtasklist_prev(wtask)				edlist_prev(		_wtask_pt, link, wtask)
#define _sigobj_wtasklist_list(wtask)				edlist_list(		_wtask_pt, link, wtask)

#define _sigobj_wtasklist_owner(wtask)				((_sigobj_pt) (NULL != _sigobj_wtasklist_list(wtask) ? _sigobj_wtasklist_list(wtask)->data : NULL))

unsigned int	_sigobj_wtasklist_getmaxpriority(_sigobj_pt sigobj);
_wtask_pt 		_sigobj_wtasklist_gettask(_sigobj_pt sigobj);
#if !(UBINOS__UBIK__EXCLUDE_TASK_PRIORITY_INHERITANCE == 1)
void 			_sigobj_wtasklist_notifychange(_sigobj_pt sigobj);
#else
#define 		_sigobj_wtasklist_notifychange(sigobj)
#endif

/*****************************************************************************/

#define _task_osigobjlist_insertprev(task, ref, sigobj)	\
													edlist_insertprev(	_sigobj_pt, osigobjlist_link, &(task)->osigobjlist, ref, sigobj)
#define _task_osigobjlist_insertnext(task, ref, sigobj)	\
													edlist_insertnext(	_sigobj_pt, osigobjlist_link, &(task)->osigobjlist, ref, sigobj)
#define _task_osigobjlist_remove(sigobj)			edlist_remove(		_sigobj_pt, osigobjlist_link, sigobj)
#define _task_osigobjlist_getcur(task)				edlist_getcur(		_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_getcurnext(task)			edlist_getcurnext(	_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_head(task)				edlist_head(		_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_tail(task)				edlist_tail(		_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_next(sigobj)				edlist_next(		_sigobj_pt, osigobjlist_link, sigobj)
#define _task_osigobjlist_prev(sigobj)				edlist_prev(		_sigobj_pt, osigobjlist_link, sigobj)
#define _task_osigobjlist_list(sigobj)				edlist_list(		_sigobj_pt, osigobjlist_link, sigobj)

#define _task_osigobjlist_owner(sigobj)				((_task_pt) (NULL != _task_osigobjlist_list(sigobj) ? _task_osigobjlist_list(sigobj)->data : NULL))

/*****************************************************************************/

#define _stimerlist_insertprev(stimerlist, ref, stimer)	\
													edlist_insertprev(	_stimer_pt, stimerlist_link, stimerlist, ref, stimer)
#define _stimerlist_insertnext(stimerlist, ref, stimer)	\
													edlist_insertnext(	_stimer_pt, stimerlist_link, stimerlist, ref, stimer)
#define _stimerlist_remove(stimer)					edlist_remove(		_stimer_pt, stimerlist_link, stimer)
#define _stimerlist_getcur(stimerlist)				edlist_getcur(		_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_getcurnext(stimerlist)			edlist_getcurnext(	_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_head(stimerlist)				edlist_head(		_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_tail(stimerlist)				edlist_tail(		_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_next(stimer)					edlist_next(		_stimer_pt, stimerlist_link, stimer)
#define _stimerlist_prev(stimer)					edlist_prev(		_stimer_pt, stimerlist_link, stimer)
#define _stimerlist_list(stimer)					edlist_list(		_stimer_pt, stimerlist_link, stimer)

void _stimerlist_add(_stimer_pt stimer);

/*****************************************************************************/
#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)

#define _kernel_monitor_tasklist_insertprev(kernel_monitor_tasklist, ref, task)	\
																			edlist_insertprev(	_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist, ref, task)
#define _kernel_monitor_tasklist_insertnext(kernel_monitor_tasklist, ref, task)	\
																			edlist_insertnext(	_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist, ref, task)
#define _kernel_monitor_tasklist_remove(task)								edlist_remove(		_task_pt, kernel_monitor_tasklist_link, task)
#define _kernel_monitor_tasklist_getcur(kernel_monitor_tasklist)			edlist_getcur(		_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_getcurnext(kernel_monitor_tasklist)		edlist_getcurnext(	_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_head(kernel_monitor_tasklist)				edlist_head(		_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_tail(kernel_monitor_tasklist)				edlist_tail(		_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_next(task)									edlist_next(		_task_pt, kernel_monitor_tasklist_link, task)
#define _kernel_monitor_tasklist_prev(task)									edlist_prev(		_task_pt, kernel_monitor_tasklist_link, task)
#define _kernel_monitor_tasklist_list(task)									edlist_list(		_task_pt, kernel_monitor_tasklist_link, task)

/*****************************************************************************/

#define _kernel_monitor_stimerlist_insertprev(kernel_monitor_stimerlist, ref, stimer)	\
																			edlist_insertprev(	_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist, ref, stimer)
#define _kernel_monitor_stimerlist_insertnext(kernel_monitor_stimerlist, ref, stimer)	\
																			edlist_insertnext(	_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist, ref, stimer)
#define _kernel_monitor_stimerlist_remove(stimer)							edlist_remove(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)
#define _kernel_monitor_stimerlist_getcur(kernel_monitor_stimerlist)		edlist_getcur(		_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_getcurnext(kernel_monitor_stimerlist)	edlist_getcurnext(	_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_head(kernel_monitor_stimerlist)			edlist_head(		_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_tail(kernel_monitor_stimerlist)			edlist_tail(		_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_next(stimer)								edlist_next(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)
#define _kernel_monitor_stimerlist_prev(stimer)								edlist_prev(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)
#define _kernel_monitor_stimerlist_list(stimer)								edlist_list(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)

/*****************************************************************************/

#define _kernel_monitor_sigobjlist_insertprev(kernel_monitor_sigobjlist, ref, sigobj)	\
																			edlist_insertprev(	_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist, ref, sigobj)
#define _kernel_monitor_sigobjlist_insertnext(kernel_monitor_sigobjlist, ref, sigobj)	\
																			edlist_insertnext(	_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist, ref, sigobj)
#define _kernel_monitor_sigobjlist_remove(sigobj)							edlist_remove(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)
#define _kernel_monitor_sigobjlist_getcur(kernel_monitor_sigobjlist)		edlist_getcur(		_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_getcurnext(kernel_monitor_sigobjlist)	edlist_getcurnext(	_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_head(kernel_monitor_sigobjlist)			edlist_head(		_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_tail(kernel_monitor_sigobjlist)			edlist_tail(		_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_next(sigobj)								edlist_next(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)
#define _kernel_monitor_sigobjlist_prev(sigobj)								edlist_prev(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)
#define _kernel_monitor_sigobjlist_list(sigobj)								edlist_list(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)

#endif
/*****************************************************************************/

int _sigobj_create(_sigobj_pt * sigobj_p);
int _sigobj_delete(_sigobj_pt * sigobj_p);
int _sigobj_wait(_sigobj_pt sigobj, _wtask_pt wtask);
int _sigobj_send(_sigobj_pt sigobj, int sigtype);
int _sigobj_broadcast(_sigobj_pt sigobj, int sigtype);
int _sigobj_setsender(_sigobj_pt sigobj, _task_pt task);

/*****************************************************************************/

void _task_init(_task_pt task);
void _task_stackinit(_task_pt task, void * arg);

void _task_idlefunc(void * arg);
void _task_rootfunc(void * arg);


void _ubik_tick_rtcisr_clear(void);
unsigned int _ubik_tick_rtccount_get(void);

void _ubik_idle_cpu_sleep(void);
void _ubik_idle_cpu_wakeup(void);

void _task_changelist(_task_pt task);
void _task_yield(void);
void _task_applypriority(_task_pt task);
void _task_collectgarbage(void);

#define _task_recalculate_waittick(task) {											\
	if (0 != task->timed) {															\
		if (_ubik_tickcount >= task->wakeuptick) {									\
			task->waittick = UBINOS__UBIK__TICK_COUNT_MAX - _ubik_tickcount + task->wakeuptick + 1;	\
		}																			\
		else {																		\
			task->waittick = task->wakeuptick - _ubik_tickcount;					\
		}																			\
	}																				\
}

#define _task_schedule() {															\
	if (_task_list_ready_index > _task_cur->priority) {								\
		_task_yield();																\
	}																				\
}																					\

void 		_task_sigobj_removewtask(_task_pt task);
#if !(UBINOS__UBIK__EXCLUDE_TASK_PRIORITY_INHERITANCE == 1)
void 		_task_sigobj_notifywtaskchange(_task_pt task);
#else
#define 	_task_sigobj_notifywtaskchange(task)
#endif

void _ubik_task_schedule_irq(); /* called by bsp */

/*****************************************************************************/

extern volatile unsigned int	_ubik_tickcount;							/* system tick count */
extern volatile unsigned int	_ubik_tickcounth;							/* system tick count high value */
extern volatile unsigned int	_ubik_tasklockcount;						/* task lock nesting count */
extern volatile unsigned int	_ubik_rtpriority;							/* lowest priority of task considered as a real-time task */

#if (UBINOS__UBIK__TICK_TYPE == UBINOS__UBIK__TICK_TYPE__RTC)
#if (UBINOS__UBIK__TICK_RTC_CHECK == 1)
extern volatile unsigned int	_ubik_tickrtccount;							/* RTC count for tick */
extern volatile int				_ubik_tickrtccount_init;					/* Is RTC count for tick initialized */
#endif
#endif

extern			edlist_t		_task_list_blocked_timed_a[2];				/* timed blocked task list array */
extern volatile edlist_pt		_task_list_blocked_timed_cur;				/* current timed blocked task list pointer */
extern volatile edlist_pt		_task_list_blocked_timed_next;				/* next timed blocked task list pointer */
extern 			edlist_t		_task_list_blocked;							/* blocked forever task list */
extern 			edlist_t		_task_list_ready_a[UBINOS__UBIK__TASK_PRIORITY_MAX+1];	/* ready task list array */
extern volatile unsigned int 	_task_list_ready_index;						/* current ready task list index */
extern volatile edlist_pt		_task_list_ready_cur;						/* current ready task list pointer */
extern 			edlist_t		_task_list_suspended;						/* suspended task list */
extern 			edlist_t		_task_list_terminated;						/* list of terminated task (these tasks are deleted by idle task automotically) */
extern 			edlist_t		_task_list_terminated_noautodel;			/* list of terminated task that should be deleted by user (a task to join must be this type) */
extern volatile _task_pt 		_task_cur;									/* current running task pointer */
extern volatile _task_pt 		_task_prev;									/* previous ran task pointer */

#if !(UBINOS__UBIK__EXCLUDE_STIMER == 1)
extern 			edlist_t		_stimer_list_a[2];							/* stimer list array */
extern volatile edlist_pt		_stimer_list_cur;							/* current stimer list pointer */
extern volatile edlist_pt		_stimer_list_next;							/* next stimer list pointer */
#endif

#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
extern 			mutex_pt		_kernel_monitor_mutex;
extern			edlist_t		_kernel_monitor_tasklist;
extern			edlist_t		_kernel_monitor_stimerlist;
extern			edlist_t		_kernel_monitor_sigobjlist;
#endif

#if !(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC == 1)
extern			idletaskhookfunc_ft	_task_idletaskhookfunc_func;
extern			void * 				_task_idletaskhookfunc_arg;
extern			unsigned int		_task_idletaskhookfunc_option;
extern			char				_task_idletaskhookfunc_name[UBINOS__UBIK__TASK_NAME_SIZE_MAX + 1];
#endif

#if !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1)
extern			tickhookfunc_ft		_ubik_tickhookfunc_func;
#endif /* !(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC == 1) */

#if !(UBINOS__UBIK__EXCLUDE_HRTICK == 1)
extern volatile unsigned int	_ubik_hrtick_active;						/* variable representing whether high resolution tick is active or not */
extern volatile unsigned int	_ubik_hrtick_hrtickpertick;
#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK == 1) */

#ifdef	__cplusplus
}
#endif

#endif /* _UBIK_H_ */
