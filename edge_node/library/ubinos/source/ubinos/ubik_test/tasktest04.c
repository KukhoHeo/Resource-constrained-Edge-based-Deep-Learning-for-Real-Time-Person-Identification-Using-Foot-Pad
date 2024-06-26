/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET2 == 1)

#include <stdio.h>
#include <stdlib.h>

extern volatile unsigned int	_g_ubik_test_result;
extern volatile unsigned int	_g_ubik_test_count1;
extern volatile unsigned int	_g_ubik_test_count2;
extern volatile unsigned int	_g_ubik_test_count3;
extern volatile unsigned int	_g_ubik_test_count4;
extern task_pt					_g_ubik_test_task_a[4];

static void tasktest04_task1func(void * arg) {
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms();

	for (i=0; i<UBINOS__UBIK_TEST__TASKLOOPCOUNT * 2; i++) {
		_g_ubik_test_count1++;
		printf("1");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("task 1 ends\n");
}

static void tasktest04_task2func(void * arg) {
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms() / 2;

	for (i=0; i<UBINOS__UBIK_TEST__TASKLOOPCOUNT * 1; i++) {
		_g_ubik_test_count2++;
		printf("2");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("task 2 ends\n");
}

static void tasktest04_task3func(void * arg) {
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms() / 2;

	for (i=0; i<UBINOS__UBIK_TEST__TASKLOOPCOUNT * 1; i++) {
		_g_ubik_test_count3++;
		printf("3");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("task 3 ends\n");
}

static void tasktest04_task4func(void * arg) {
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms();

	for (i=0; i<UBINOS__UBIK_TEST__TASKLOOPCOUNT * 2; i++) {
		_g_ubik_test_count4++;
		printf("4");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("task 4 ends\n");
}

int ubik_test_tasktest04(void) {
	int r;
	int r2;
	unsigned int count1;
	unsigned int count2;
	unsigned int count3;
	unsigned int count4;
	unsigned int sleepvalue = ubik_timemstotick(UBINOS__UBIK_TEST__TASKWAITTIMEMS) * UBINOS__UBIK_TEST__TASKLOOPCOUNT / 3;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_tasktest04</name>\n");
	printf("<description>Test on priority-based multi-tasking</description>\n");

	printf("<message>\n");

	_g_ubik_test_result = 0;

	_g_ubik_test_count1 = 0;
	_g_ubik_test_count2 = 0;
	_g_ubik_test_count3 = 0;
	_g_ubik_test_count4 = 0;

	_g_ubik_test_task_a[0] = NULL;
	_g_ubik_test_task_a[1] = NULL;
	_g_ubik_test_task_a[2] = NULL;
	_g_ubik_test_task_a[3] = NULL;

	printf("create task 4\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[4-1], tasktest04_task4func, NULL, task_getpriority(NULL)-3, 0, "tasktest04 task 4");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	task_sleep(sleepvalue);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	count4 = _g_ubik_test_count4;
	printf("\n");
	printf("checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("check point 01: ");
	if (count1 != _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 != _g_ubik_test_count3 || count4 == _g_ubik_test_count4) {
		printf("fail\n");
		r = -1;
		goto end1;
	}
	printf("pass\n");

	task_sleep(sleepvalue);

	printf("\n");
	printf("create task 1\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[1-1], tasktest04_task1func, NULL, task_getpriority(NULL)-1, 0, "tasktest04 task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	task_sleep(sleepvalue);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	count4 = _g_ubik_test_count4;
	printf("\n");
	printf("checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("check point 02: ");
	if (count1 == _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 != _g_ubik_test_count3 || count4 != _g_ubik_test_count4) {
		printf("fail\n");
		r = -1;
		goto end1;
	}
	printf("pass\n");

	task_sleep(sleepvalue);

	printf("\n");
	printf("create task 2\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[2-1], tasktest04_task2func, NULL, task_getpriority(NULL)-2, 0, "tasktest04 task 2");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	printf("create task 3\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[3-1], tasktest04_task3func, NULL, task_getpriority(NULL)-2, 0, "tasktest04 task 3");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	task_sleep(sleepvalue);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	count4 = _g_ubik_test_count4;
	printf("\n");
	printf("checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("check point 03: ");
	if (count1 == _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 != _g_ubik_test_count3 || count4 != _g_ubik_test_count4) {
		printf("fail\n");
		r = -1;
		goto end1;
	}
	printf("pass\n");

	task_sleep(sleepvalue * 2);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	count4 = _g_ubik_test_count4;
	printf("\n");
	printf("checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("check point 04: ");
	if (count1 != _g_ubik_test_count1 || count2 == _g_ubik_test_count2 || count3 == _g_ubik_test_count3 || count4 != _g_ubik_test_count4) {
		printf("fail\n");
		r = -1;
		goto end1;
	}
	printf("pass\n");

	task_sleep(sleepvalue * 2);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	count4 = _g_ubik_test_count4;
	printf("\n");
	printf("checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("check point 05: ");
	if (count1 != _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 != _g_ubik_test_count3 || count4 == _g_ubik_test_count4) {
		printf("fail\n");
		r = -1;
		goto end1;
	}
	printf("pass\n");

	r = 0;

end1:
	r2 = task_join_and_delete(_g_ubik_test_task_a, NULL, 4);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
		r = -1;
	}

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

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET2 == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */

