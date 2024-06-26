/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBIK_TEST == 1)
#if !( UBINOS__UBIK_TEST__EXCLUDE_MISCTESTSET == 1)

#include <stdio.h>
#include <stdlib.h>

int ubik_test_misctest03(void) {
	int r;
	tickcount_t tickcount1;
	tickcount_t tickcount2;
	tickcount_t tickcount_busywait;
	tickcount_t tickcount_task_sleepms;
	unsigned int waitvalue;
	unsigned int errorrate;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_misctest03</name>\n");
	printf("<description>Test for accuracy of task_sleepms</description>\n");

	printf("<message>\n");

	printf("bsp_busywait\n");
	printf("it tasks %d ms\n", UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS);

	waitvalue = bsp_getbusywaitcountperms() * UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS;
	tickcount1 = ubik_gettickcount();
	bsp_busywait(waitvalue);
	tickcount2 = ubik_gettickcount();
	tickcount_busywait = ubik_gettickdiff(tickcount1, tickcount2);

	printf("task_sleepms\n");
	printf("it tasks %d ms\n", UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS);

	tickcount1 = ubik_gettickcount();
	task_sleepms(UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS);
	tickcount2 = ubik_gettickcount();
	tickcount_task_sleepms = ubik_gettickdiff(tickcount1, tickcount2);

	printf("bsp_busywait tick count is %d\n", tickcount_busywait.low);
	printf("task_sleepms tick count is %d\n", tickcount_task_sleepms.low);

	if (tickcount_busywait.low >= tickcount_task_sleepms.low) {
		errorrate = (tickcount_busywait.low - tickcount_task_sleepms.low) * 100 / tickcount_busywait.low;
	}
	else {
		errorrate = (tickcount_task_sleepms.low - tickcount_busywait.low) * 100 / tickcount_busywait.low;
	}
	printf("accuracy is %d percent\n", 100 - errorrate);

	if (errorrate <= 2) {
		r = 0;
	}
	else {
		r = -1;
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

#endif /* !( UBINOS__UBIK_TEST__EXCLUDE_MISCTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */
