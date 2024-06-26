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

int ubik_test_misctest02(void) {
	int r;
	tickcount_t tickcount1;
	tickcount_t tickcount2;
	unsigned int waitvalue;
	unsigned int rtick;
	unsigned int itick;
	unsigned int rloopcount;
	unsigned int iloopcount;
	unsigned int errorrate;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_misctest02</name>\n");
	printf("<description>Test for accuracy of UBINOS__BSP__BUSYWAITCOUNT_PER_MS</description>\n");

	printf("<message>\n");

	printf("it tasks %d ms\n", UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS);

	waitvalue = bsp_getbusywaitcountperms() * UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS;
	tickcount1 = ubik_gettickcount();
	bsp_busywait(waitvalue);
	tickcount2 = ubik_gettickcount();
	tickcount1 = ubik_gettickdiff(tickcount1, tickcount2);

	rtick = tickcount1.low;
	itick = ubik_gettickpersec() * UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS / 1000;
	rloopcount = bsp_getbusywaitcountperms();
	iloopcount = rloopcount * itick / rtick;

	printf("current value is %d\n", rloopcount);
	printf("recommended value is %d\n", iloopcount);

	if (rtick >= itick) {
		errorrate = (rtick - itick) * 100 / itick;
	}
	else {
		errorrate = (itick - rtick) * 100 / itick;
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
