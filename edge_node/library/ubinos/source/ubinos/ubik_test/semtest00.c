/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_SEMTESTSET == 1)

#include <stdio.h>
#include <stdlib.h>

#define TASKWORKINGTIMEMS   500
#define TASKLOOPCOUNT		10

static sem_pt 	_g_sem		= NULL;

static void semtest00_task1func(void * arg) {
	unsigned int i;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		bsp_busywait((unsigned int) rand() % TASKWORKINGTIMEMS * bsp_getbusywaitcountperms());
		printf("task 1 produces one\n");

		sem_give(_g_sem);
	}
}

static void semtest00_task2func(void * arg) {
	unsigned int i;

	for (i=0; i<TASKLOOPCOUNT/2; i++) {
		printf("task 2 waits\n");
		sem_take(_g_sem);

		printf("task 2 consumes one\n");
		bsp_busywait((unsigned int) rand() % TASKWORKINGTIMEMS * bsp_getbusywaitcountperms());
	}
}

static void semtest00_task3func(void * arg) {
	unsigned int i;

	for (i=0; i<TASKLOOPCOUNT/2; i++) {
		printf("task 3 waits\n");
		sem_take(_g_sem);

		printf("task 3 consumes one\n");
		bsp_busywait((unsigned int) rand() % TASKWORKINGTIMEMS * bsp_getbusywaitcountperms());
	}
}

int ubik_test_semtest00(void) {
	int r;
	int r2;
	task_pt task_a[3];

	r = sem_create(&_g_sem);
	if (0 != r) {
		printf("fail at sem_create(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = task_create_noautodel(&task_a[0], semtest00_task1func, NULL, task_getpriority(NULL)-1, 0, "task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	r = task_create_noautodel(&task_a[1], semtest00_task2func, NULL, task_getpriority(NULL)-1, 0, "task 2");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end2;
	}

	task_sleep(10);

	r = task_create_noautodel(&task_a[2], semtest00_task3func, NULL, task_getpriority(NULL)-1, 0, "task 3");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end2;
	}

	r = 0;

end2:
	r2 = task_join_and_delete(task_a, NULL, 3);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
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

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_SEMTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */

