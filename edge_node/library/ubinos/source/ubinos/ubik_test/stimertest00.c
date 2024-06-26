/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if (1 != UBINOS__UBIK_TEST__EXCLUDE_STIMERTESTSET)

#include <stdio.h>
#include <stdlib.h>

#define TASKLOOPCOUNT		10
#define TASKLOOPINTERVALMS	1000

static stimer_pt 	_g_stimer		= NULL;
static sem_pt		_g_sem			= NULL;

static void stimertest00_taskfunc(void * arg) {
	unsigned int i;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		sem_take(_g_sem);

		printf("hello world!\n");
		bsp_busywait((unsigned int) rand() % TASKLOOPINTERVALMS / 2 * bsp_getbusywaitcountperms());
	}
}

int ubik_test_stimertest00(void) {
	int r;
	int r2;
	task_pt task;

	r = semb_create(&_g_sem);
	if (0 != r) {
		printf("fail at semb_create(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = stimer_create(&_g_stimer);
	if (0 != r) {
		printf("fail at stimer_create(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	r = task_create_noautodel(&task, stimertest00_taskfunc, NULL, task_getpriority(NULL)-1, 0, "stimertest00 task");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end2;
	}

	r = stimer_set(_g_stimer, ubik_timemstotick(TASKLOOPINTERVALMS), _g_sem, 0);
	if (0 != r) {
		printf("fail at stimer_set(), err=%d\n", r);
		r = -1;
		goto end2;
	}

	r = stimer_start(_g_stimer);
	if (0 != r) {
		printf("fail at stimer_start(), err=%d\n", r);
		r = -1;
		goto end3;
	}

	r = 0;

end3:
	r2 = task_join_and_delete(&task, NULL, 1);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
		r = -1;
	}

	r2 = stimer_stop(_g_stimer);
	if (0 != r2) {
		printf("fail at stimer_stop(), err=%d\n", r2);
		r = -1;
	}

end2:
	r2 = stimer_delete(&_g_stimer);
	if (0 != r2) {
		printf("fail at stimer_delete(), err=%d\n", r2);
		r = -1;
	}

end1:
	r2 = sem_delete(&_g_sem);
	if (0 != r2) {
		printf("fail at sem_delete(), err=%d\n", r2);
		r = -1;
	}

end0:
	return r;
}

#endif /* (1 != UBINOS__UBIK_TEST__EXCLUDE_STIMERTESTSET) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */

