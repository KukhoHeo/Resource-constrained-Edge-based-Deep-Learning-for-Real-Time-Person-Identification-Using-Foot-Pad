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

#define TASKLOOPCOUNT		10
#define TASKLOOPINTERVALMS	1000

static void tasktest00_taskfunc(void * arg) {
	unsigned int i;

	for (i=0; i<TASKLOOPCOUNT; i++) {
		printf("hello world!\n");

		task_sleep(ubik_timemstotick(TASKLOOPINTERVALMS));
	}
}

int ubik_test_tasktest00(void) {
	int r;
	task_pt task;

	r = task_create_noautodel(&task, tasktest00_taskfunc, NULL, task_getpriority(NULL)-1, 0, "tasktest00 task");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = task_join_and_delete(&task, NULL, 1);
	if (0 != r) {
		printf("fail at task_join_and_delete(), err=%d\n", r);
		r = -1;
		goto end0;
	}

	r = 0;

end0:
	return r;
}

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */

