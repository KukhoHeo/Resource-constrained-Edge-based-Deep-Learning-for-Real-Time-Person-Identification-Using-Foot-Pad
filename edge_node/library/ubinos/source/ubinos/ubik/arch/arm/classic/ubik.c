/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__ARM926EJ_S)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBIK

int _ubik_port_comp_init(unsigned int idle_stackdepth) {
	int r = 0;

	r = _ubik_inittick();
	if (r != 0) {
		return -1;
	}

	r = task_comp_init(idle_stackdepth);
	if (r != 0) {
		return -1;
	}

	return 0;
}

void _ubik_port_comp_start(void) {
	task_comp_start();
}

int ubik_isactive(void) {
	return _bsp_kernel_active;
}

tickcount_t ubik_gettickcount(void) {
	tickcount_t tc;

	ubik_entercrit();

	tc.high	= _ubik_tickcounth;
	tc.low	= _ubik_tickcount;

	ubik_exitcrit();

	return tc;
}

tickcount_t ubik_gettickdiff(tickcount_t tick1, tickcount_t tick2) {
	tickcount_t diff;

	if (tick1.high > tick2.high) {
		diff.high = UBINOS__UBIK__TICK_COUNT_MAX - tick1.high + 1 + tick2.high;
	}
	else {
		diff.high = tick2.high - tick1.high;
	}

	if (tick1.low > tick2.low) {
		diff.high--;
		diff.low = UBINOS__UBIK__TICK_COUNT_MAX - tick1.low + 1 + tick2.low;
	}
	else {
		diff.low = tick2.low - tick1.low;
	}

	return diff;
}

void _ubik_swisr(unsigned int swino) {
	switch(swino) {
	case SWINO__TASK_YIELD:
		_task_prev = _task_cur;
		if(TASK_STATE__RUNNING == _task_cur->state) {
			_task_cur->state = TASK_STATE__READY;
		}
		_task_cur = _tasklist_getcurnext(_task_list_ready_cur);
		_task_cur->state = TASK_STATE__RUNNING;
		break;
	}
}

void _task_stackinit(_task_pt task, void * arg) {
	unsigned int *	stacktop;
	unsigned int	stacktop_ori;
	unsigned int	stacklimit;

	stacktop 		= (unsigned int *) (((unsigned int) task->stack) + task->stacksize);
	stacktop_ori	= (unsigned int) stacktop;
	stacklimit		= ((unsigned int) task->stack) + UBINOS__BSP__STACK_OVERFLOW_CHECK_MARGIN + ARM_CONTEXT_SIZE;

	*(--stacktop) = ((unsigned int) _task_rootfunc & ~0x00000001);
	*(--stacktop) = ((unsigned int) 0x00000000);					/* r14 (lr)				*/
	*(--stacktop) = ((unsigned int) stacktop_ori);					/* r13 (sp)				*/
	*(--stacktop) = ((unsigned int) 0x12121212);					/* r12					*/
	*(--stacktop) = ((unsigned int) 0x11111111);					/* r11					*/
	*(--stacktop) = ((unsigned int) 0x10101010);					/* r10	                */
	*(--stacktop) = ((unsigned int) 0x09090909);					/* r9					*/
	*(--stacktop) = ((unsigned int) 0x08080808);					/* r8 					*/
	*(--stacktop) = ((unsigned int) 0x07070707);					/* r7 					*/
	*(--stacktop) = ((unsigned int) 0x06060606);					/* r6 					*/
	*(--stacktop) = ((unsigned int) 0x05050505);					/* r5 					*/
	*(--stacktop) = ((unsigned int) 0x04040404);					/* r4 					*/
	*(--stacktop) = ((unsigned int) 0x03030303);					/* r3 					*/
	*(--stacktop) = ((unsigned int) 0x02020202);					/* r2 					*/
	*(--stacktop) = ((unsigned int) 0x01010101);					/* r1 					*/
	*(--stacktop) = ((unsigned int) arg       );					/* r0 					*/
	if ( 0 == ((unsigned int) _task_rootfunc & 0x00000001) ) {
		*(--stacktop) = (unsigned int) ARM_MODE_SYS;			/* spsr = nzcvqift_sys */
	}
	else {
		*(--stacktop) = (unsigned int) ARM_MODE_SYS | T_BIT;	/* spsr = nzcvqifT_sys */
	}
	*(--stacktop) = 0 											/* _bsp_critcount = 0 */;

	task->stacktop = stacktop;
	task->stacktop_max = stacktop;
	task->stacklimit = (unsigned int *) stacklimit;

	return;
}

int task_getstacksize(task_pt _task, unsigned int * stacksize_p) {
	int r;
	_task_pt task = (_task_pt)_task;

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == stacksize_p) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (NULL == task && 0 == _bsp_kernel_active) {
		r = bsp_getstacksize(0, stacksize_p);
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || (OBJTYPE__UBIK_TASK != task->type && OBJTYPE__UBIK_IDLETASK != task->type)) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	*stacksize_p = task->stacksize;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

int task_getmaxstackusage(task_pt _task, unsigned int * maxstackusage_p) {
	if (NULL != maxstackusage_p) {
		*maxstackusage_p = 0;
	}

	return -1;
}

int ubik_iscrit(void) {
	if (	(0 != _bsp_critcount) ||
			(0 != bsp_isintr()) 		) {
		return 1;
	}
	else {
		return 0;
	}
}

int ubik_istask(void) {
	if (	(0 != _bsp_kernel_active && 0 == bsp_isintr()) 	) {
		return 1;
	}
	else {
		return 0;
	}
}

int ubik_isrt(void) {
	if (	(0 != _bsp_critcount) ||
			(0 != bsp_isintr()) ||
			(0 != _ubik_tasklockcount) ||
			(0 != _bsp_kernel_active && _task_cur->priority >= _ubik_rtpriority)	) {
		return 1;
	}
	else {
		return 0;
	}
}

void _task_yield(void) {
	if (0 == bsp_isintr() && 0 == _ubik_tasklockcount && 0 != _bsp_kernel_active) {
		__asm__ __volatile__ (
			/* swi SWINO__TASK_YIELD */
			"swi 0x01													\n\t"
		);
	}
}

void _ubik_task_schedule_irq() {
    if (0 == _ubik_tasklockcount && 0 != _bsp_kernel_active) {
        if(_task_list_ready_index > _task_cur->priority) {
          _task_prev = _task_cur;
          if(TASK_STATE__RUNNING == _task_cur->state) {
              _task_cur->state = TASK_STATE__READY;
          }
          _task_cur = _tasklist_getcurnext(_task_list_ready_cur);
          _task_cur->state = TASK_STATE__RUNNING;
        }
    }
}

#endif /* (UBINOS__BSP__CPU_TYPE == ...) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


