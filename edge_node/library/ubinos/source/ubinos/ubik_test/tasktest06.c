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

#define TASKWORKINGTIMEMS	5

extern volatile unsigned int	_g_ubik_test_count1;
extern mutex_pt					_g_ubik_test_mutex;

static void tasktest06_task1func(void * arg) {
	unsigned int waitvalue = TASKWORKINGTIMEMS * bsp_getbusywaitcountperms();

	bsp_busywait(waitvalue);

	mutex_lock(_g_ubik_test_mutex);
	_g_ubik_test_count1--;
	mutex_unlock(_g_ubik_test_mutex);
}

int ubik_test_tasktest06(void) {
	int r;
	int r2;
	unsigned int i;
	unsigned int j;
	task_pt	* task_p;
	unsigned int size_old;
	unsigned int count_old;
	unsigned int size;
	unsigned int count;
	unsigned int sleepvalue = ubik_timemstotick(TASKWORKINGTIMEMS);

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_tasktest06</name>\n");
	printf("<description>Test for memory leak by task</description>\n");

	printf("<message>\n");

	ubik_collectgarbage();

	r = heap_getallocatedsize(NULL, &size_old);
	if (0 != r) {
		printf("fail at heap_getallocatedsize\n");
		goto end0;
	}

	r = heap_getallocatedcount(NULL, &count_old);
	if (0 != r) {
		printf("fail at heap_getallocatedcount\n");
		goto end0;
	}

	r = mutex_create(&_g_ubik_test_mutex);
	if (0 != r) {
		printf("fail at mutex_create(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	task_p = malloc(sizeof(task_pt) * UBINOS__UBIK_TEST__MEMLEAK_TASKCOUNT);
	if (NULL == task_p) {
		printf("fail at malloc()\n");
		r = -1;
		goto end1;
	}

	_g_ubik_test_count1 = 0;

	r = 0;

	printf("|----------|\n ");
	for (i=0; i<UBINOS__UBIK_TEST__MEMLEAK_TESTCOUNT; i++) {
		for (j=0; j<UBINOS__UBIK_TEST__MEMLEAK_TASKCOUNT; j++) {
			r = task_create(&task_p[j], tasktest06_task1func, NULL, task_getpriority(NULL)-1, 0, "tasktest06 task 1");
			if (0 != r) {
				printf("\n");
				printf("test %d, task %d\n", i, j);
				printf("fail at task_create(), err=%d\n", r);
				r = -1;
				break;
			}
			_g_ubik_test_count1++;
		}

		task_sleep(sleepvalue * _g_ubik_test_count1 * 2);

		while (0 < _g_ubik_test_count1) {
			task_sleep(sleepvalue);
		}

		if (0 != r) {
			break;
		}
		if (0 == i%(UBINOS__UBIK_TEST__MEMLEAK_TESTCOUNT/10)) {
			printf("+");
		}
	}

	if (0 == r) {
		printf("\n");
		printf("pass %d tests creating and deleting %d tasks\n", i, j);
	}

	free(task_p);

end1:
	r2 = mutex_delete(&_g_ubik_test_mutex);
	if (0 != r2) {
		printf("fail at mutex_delete(), err=%d\n", r2);
		r = -1;
	}

	if (0 == r) {
		ubik_collectgarbage();

		r2 = heap_getallocatedsize(NULL, &size);
		if (0 != r2) {
			printf("fail at heap_getallocatedsize\n");
			r = -1;
			goto end0;
		}

		r2 = heap_getallocatedcount(NULL, &count);
		if (0 != r2) {
			printf("fail at heap_getallocatedcount\n");
			r = -1;
			goto end0;
		}

		if (size_old != size || count_old != count) {
			printf("memory leak was detected\n");
			r = -1;
			goto end0;
		}
	}

end0:
	if (0 != r) {
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

