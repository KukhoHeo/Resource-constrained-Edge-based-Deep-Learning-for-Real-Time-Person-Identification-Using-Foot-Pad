/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_MSGQTESTSET == 1)

#include <stdio.h>
#include <stdlib.h>

#define TASKWORKINGTIMEMS   500
#define TASKLOOPCOUNT		6
#define MSGSIZE				2
#define MSGCOUNT			3

static msgq_pt 	_g_msgq		= NULL;
int test_result = 0;

static void msgqtest05_task1func(void * arg) {
	int r;
	unsigned int i;
	unsigned char buf[MSGSIZE];
	unsigned int count;

	for (i=0; i<MSGSIZE; i++) {
		buf[i] = i;
	}

	test_result = 0;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		bsp_busywait((unsigned int) rand() % TASKWORKINGTIMEMS * bsp_getbusywaitcountperms());
		printf("task 1 produces one message\n");

		switch (i)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				r = msgq_send(_g_msgq, buf);
				if (r != 0)
				{
					test_result = -1;
				}
				break;
			case 4:
				r = msgq_send(_g_msgq, buf);
				if (r != UBIK_ERR__OVERFLOWED)
				{
					msgq_getcount(_g_msgq, &count);
					printf("message count : %d\n", count);
					test_result = -2;
				}
				break;
			case 5:
				ubik_entercrit();
				r = msgq_send(_g_msgq, buf);
				ubik_exitcrit();
				if (r != UBIK_ERR__OVERFLOWED)
				{
					msgq_getcount(_g_msgq, &count);
					printf("message count : %d\n", count);
					test_result = -2;
				}
			default:
				break;
		}

		if (test_result != 0)
		{
			break;
		}

	}
}

static void msgqtest05_task2func(void * arg) {
	unsigned char buf[MSGSIZE];

	printf("task 2 waits message\n");
	msgq_receive(_g_msgq, buf);

	printf("task 2 consumes one message\n");
	bsp_busywait((unsigned int) rand() % TASKWORKINGTIMEMS * bsp_getbusywaitcountperms());
}

int ubik_test_msgqtest05(void) {
	int r;
	int r2;
	task_pt task_a[2];

	r = msgq_create(&_g_msgq, MSGSIZE, MSGCOUNT);
	if (0 != r) {
		printf("fail at msgq_create(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = task_create_noautodel(&task_a[0], msgqtest05_task1func, NULL, task_getpriority(NULL)-1, 0, "task 1");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	r = task_create_noautodel(&task_a[1], msgqtest05_task2func, NULL, task_getpriority(NULL)-1, 0, "task 2");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end2;
	}

end2:
	r2 = task_join_and_delete(task_a, NULL, 2);
	if (0 != r2) {
		printf("fail at task_join_and_delete(), err=%d\n", r2);
		r = -1;
	}

end1:
	r2 = msgq_delete(&_g_msgq);
	if (0 != r2) {
		printf("fail at msgq_delete(), err=%d\n", r2);
		r = -1;
	}

end0:

	return test_result;
}

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_MSGQTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */
