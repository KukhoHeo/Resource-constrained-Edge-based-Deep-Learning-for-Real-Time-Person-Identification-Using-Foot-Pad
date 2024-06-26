/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_MUTEXTESTSET == 1)

#include <stdio.h>
#include <stdlib.h>

#define TASKBUSYWAITCOUNT   1
#define TASKLOOPCOUNT		10000

static mutex_pt _g_mutex	= NULL;
static int		_g_data		= 0;

static void mutextest00_subtest1_task1func(void * arg) {
	unsigned int i;
	int temp;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		temp = _g_data;
		temp = temp + 1;
		bsp_busywait(TASKBUSYWAITCOUNT);
		_g_data = temp;
	}
}

static void mutextest00_subtest1_task2func(void * arg) {
	unsigned int i;
	int temp;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		temp = _g_data;
		temp = temp - 1;
		bsp_busywait(TASKBUSYWAITCOUNT);
		_g_data = temp;
	}
}

static int ubik_test_mutextest00_subtest1(void) {
	int r;
	int r2;
	task_pt task_a[2];

	_g_data 	= 0;

	r = task_create_noautodel(&task_a[0], mutextest00_subtest1_task1func, NULL, task_getpriority(NULL)-1, 0, "task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = task_create_noautodel(&task_a[1], mutextest00_subtest1_task2func, NULL, task_getpriority(NULL)-1, 0, "task 2");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	r = 0;

end1:
	r2 = task_join_and_delete(task_a, NULL, 2);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
		r = -1;
	}

end0:
	printf("data (none mutual exclusive use) : %d\n", _g_data);

	return r;
}

static void mutextest00_subtest2_task1func(void * arg) {
	unsigned int i;
	int temp;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		mutex_lock(_g_mutex);

		temp = _g_data;
		temp = temp + 1;
		bsp_busywait(TASKBUSYWAITCOUNT);
		_g_data = temp;

		mutex_unlock(_g_mutex);
	}
}

static void mutextest00_subtest2_task2func(void * arg) {
	unsigned int i;
	int temp;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		mutex_lock(_g_mutex);

		temp = _g_data;
		temp = temp - 1;
		bsp_busywait(TASKBUSYWAITCOUNT);
		_g_data = temp;

		mutex_unlock(_g_mutex);
	}
}

static int ubik_test_mutextest00_subtest2(void) {
	int r;
	int r2;
	task_pt task_a[2];

	r = mutex_create(&_g_mutex);
	if (0 != r) {
		printf("fail at mutex_create(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	_g_data = 0;

	r = task_create_noautodel(&task_a[0], mutextest00_subtest2_task1func, NULL, task_getpriority(NULL)-1, 0, "task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	r = task_create_noautodel(&task_a[1], mutextest00_subtest2_task2func, NULL, task_getpriority(NULL)-1, 0, "task 2");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end2;
	}

	r = 0;

end2:
	r2 = task_join_and_delete(task_a, NULL, 2);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
		r = -1;
	}

end1:
	r2 = mutex_delete(&_g_mutex);
	if (0 != r2) {
		printf("fail at mutex_delete(), err=%d\n", r2);
		r = -1;
	}

end0:
	printf("data (mutual exclusive use)      : %d\n", _g_data);

	return r;
}

int ubik_test_mutextest00(void) {
	int r;

	r = ubik_test_mutextest00_subtest1();
	if (0 != r) {
		printf("fail at ubik_test_mutextest00_subtest1(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = ubik_test_mutextest00_subtest2();
	if (0 != r) {
		printf("fail at ubik_test_mutextest00_subtest2(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = 0;

end0:
	return r;
}

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_MUTEXTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */

