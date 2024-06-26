/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_CONDVTESTSET == 1)

#include <stdio.h>
#include <stdlib.h>

static void condvtest03_task1func(void * arg) {
	int r;
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms();

	for (i=0; i<(UBINOS__UBIK_TEST__TASKLOOPCOUNT * 1); i++) {
		_g_ubik_test_count1++;
		printf("1");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("Task 1 waits signal\n");
	mutex_lock(_g_ubik_test_mutex);
	r = condv_wait(_g_ubik_test_condv, _g_ubik_test_mutex);
	mutex_unlock(_g_ubik_test_mutex);
	if (0 != r) {
		printf("Task 1: fail at condv_wait(), err=%d\n", r);
		_g_ubik_test_result = -1;
		goto end0;
	}
	printf("Task 1 receives signal\n");

	for (i=0; i<(UBINOS__UBIK_TEST__TASKLOOPCOUNT * 1); i++) {
		_g_ubik_test_count1++;
		printf("1");
		bsp_busywait(waitvalue);
	}
	printf("\n");

end0:
	printf("Task 1 ends\n");
}

static void condvtest03_task2func(void * arg) {
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms();

	for (i=0; i<(UBINOS__UBIK_TEST__TASKLOOPCOUNT * 1); i++) {
		_g_ubik_test_count2++;
		printf("2");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("Task 2 ends\n");
}

static void condvtest03_task3func(void * arg) {
	int r;
	int i;
	unsigned int waitvalue = UBINOS__UBIK_TEST__TASKWAITTIMEMS * bsp_getbusywaitcountperms();

	for (i=0; i<(UBINOS__UBIK_TEST__TASKLOOPCOUNT * 3); i++) {
		_g_ubik_test_count3++;
		printf("3");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("Task 3 sends signal\n");
	mutex_lock(_g_ubik_test_mutex);
	r = condv_signal(_g_ubik_test_condv);
	mutex_unlock(_g_ubik_test_mutex);
	if (0 != r) {
		printf("Task 3: fail at condv_signal(), err=%d\n", r);
		_g_ubik_test_result = -1;
		goto end0;
	}

	for (i=0; i<(UBINOS__UBIK_TEST__TASKLOOPCOUNT * 1); i++) {
		_g_ubik_test_count3++;
		printf("3");
		bsp_busywait(waitvalue);
	}
	printf("\n");

	printf("Cancels sender setting\n");
	r = condv_setsender(_g_ubik_test_condv, NULL);
	if (0 != r) {
		printf("Task 3: fail at condv_setsender(), err=%d\n", r);
		_g_ubik_test_result = -1;
		goto end0;
	}

end0:
	printf("Task 3 ends\n");
}

int ubik_test_condvtest03(void) {
	int r;
	int r2;
	unsigned int count1;
	unsigned int count2;
	unsigned int count3;
	unsigned int sleepvalue = ubik_timemstotick(UBINOS__UBIK_TEST__TASKWAITTIMEMS) * UBINOS__UBIK_TEST__TASKLOOPCOUNT / 3;
	int pri;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_condvtest03</name>\n");
	printf("<description>Priority inheritance test of condition variable</description>\n");

	printf("<message>\n");

	_g_ubik_test_result = 0;

	_g_ubik_test_count1 = 0;
	_g_ubik_test_count2 = 0;
	_g_ubik_test_count3 = 0;

	_g_ubik_test_task_a[0] = NULL;
	_g_ubik_test_task_a[1] = NULL;
	_g_ubik_test_task_a[2] = NULL;

	r = condv_create(&_g_ubik_test_condv);
	if (0 != r) {
		printf("fail at condv_create(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = mutex_create(&_g_ubik_test_mutex);
	if (0 != r) {
		printf("fail at mutex_create(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	printf("Task 3 begins\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[3-1], condvtest03_task3func, NULL, task_getpriority(NULL)-3, 0, "condvtest03 task 3");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end2;
	}

	printf("Set task 3 as a sender\n");
	r = condv_setsender(_g_ubik_test_condv, _g_ubik_test_task_a[2]);
	if (0 != r) {
		printf("fail at condv_setsender(), err=%d\n", r);
		r = -1;
		goto end3;
	}

	task_sleep(sleepvalue);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 01: ");
	if (count1 != _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 == _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	task_sleep(sleepvalue);

	printf("\n");
	printf("Task 1 begins\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[1-1], condvtest03_task1func, NULL, task_getpriority(NULL)-1, 0, "condvtest03 task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end3;
	}

	task_sleep(sleepvalue);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 02: ");
	if (count1 == _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 != _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	pri = task_getpriority(_g_ubik_test_task_a[0]);
	printf("Priority of task 1 is %d\n", pri);
	pri = task_getpriority(_g_ubik_test_task_a[2]);
	printf("Priority of task 3 is %d\n", pri);

	task_sleep(sleepvalue * 2);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	pri = task_getpriority(_g_ubik_test_task_a[2]);
	printf("Priority of task 3 is %d\n", pri);
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 03: ");
	if (count1 != _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 == _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	task_sleep(sleepvalue);

	printf("\n");
	printf("Task 2 begins\n");
	r = task_create_noautodel(&_g_ubik_test_task_a[2-1], condvtest03_task2func, NULL, task_getpriority(NULL)-2, 0, "condvtest03 task 2");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end3;
	}

	task_sleep(sleepvalue);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 04: ");
	if (count1 != _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 == _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	task_sleep(sleepvalue * 2);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	pri = task_getpriority(_g_ubik_test_task_a[2]);
	printf("Priority of task 3 is %d\n", pri);
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 05: ");
	if (count1 == _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 != _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	task_sleep(sleepvalue * 2);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 06: ");
	if (count1 != _g_ubik_test_count1 || count2 == _g_ubik_test_count2 || count3 != _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	task_sleep(sleepvalue * 2);

	count1 = _g_ubik_test_count1;
	count2 = _g_ubik_test_count2;
	count3 = _g_ubik_test_count3;
	printf("\n");
	printf("Checking\n");
	task_sleep(sleepvalue);
	printf("\n");
	printf("Check point 07: ");
	if (count1 != _g_ubik_test_count1 || count2 != _g_ubik_test_count2 || count3 == _g_ubik_test_count3) {
		printf("fail\n");
		r = -1;
		goto end3;
	}
	printf("pass\n");

	r = 0;

end3:
	r2 = task_join_and_delete(_g_ubik_test_task_a, NULL, 3);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
		r = -1;
	}

end2:
	r2 = mutex_delete(&_g_ubik_test_mutex);
	if (0 != r2) {
		printf("fail at mutex_delete(), err=%d\n", r2);
		r = -1;
	}

end1:
	r2 = condv_delete(&_g_ubik_test_condv);
	if (0 != r2) {
		printf("fail at condv_delete(), err=%d\n", r2);
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

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_CONDVTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */
