/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET == 1)

#include <stdio.h>
#include <stdlib.h>

#define TASKARG				((void *) -3)
#define TASKWORKINGTIMEMS   1000

extern volatile unsigned int	_g_ubik_test_result;
extern task_pt					_g_ubik_test_task_a[4];

static void tasktest01_task1func(void * arg) {
	unsigned int waitvalue = TASKWORKINGTIMEMS * bsp_getbusywaitcountperms();

	if (TASKARG == arg) {
		printf("task 1 is running\n");

		bsp_busywait(waitvalue);

		_g_ubik_test_result = 0;
	}
	else {
		printf("task 1 received wrong argument\n");
	}

	printf("task 1 ends\n");
}

int ubik_test_tasktest01(void) {
	int r;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_tasktest01</name>\n");
	printf("<description>Test on basic functions of task</description>\n");

	printf("<message>\n");

	_g_ubik_test_result = -1;

	printf("create task 1\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[0], tasktest01_task1func, TASKARG, task_getpriority(NULL)-1, 0, "tasktest01 task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = task_join_and_delete(_g_ubik_test_task_a, NULL, 1);
	if (0 != r) {
		printf("fail at task_join_and_delete(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = 0;

end0:
	if (0 != r || 0 > _g_ubik_test_result) {
		r = -1;
	}
	else {
		r = 0;
	}

	printf("</message>\n");

	printf("<result>");
	if (0 == r) {
		printf("pass");
	}
	else {
		printf("fail");
	}
	printf("</result>\n");
	printf("</test>\n");
	printf("\n");
	return r;
}

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */

