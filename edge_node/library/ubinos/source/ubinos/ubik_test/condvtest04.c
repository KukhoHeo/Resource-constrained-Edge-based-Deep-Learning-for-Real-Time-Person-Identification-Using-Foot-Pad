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

int ubik_test_condvtest04(void) {
	int r;
	int r2;
	int i;
	int j;
	condv_pt * condv_p;
	unsigned int size_old;
	unsigned int count_old;
	unsigned int size;
	unsigned int count;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_condvtest04</name>\n");
	printf("<description>Memory leak test of condition variable</description>\n");

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

	condv_p = malloc(sizeof(condv_pt) * UBINOS__UBIK_TEST__MEMLEAK_SIGOBJCOUNT);
	if (NULL == condv_p) {
		printf("fail at malloc()\n");
		goto end0;
	}

	r = 0;

	printf("|----------|\n ");
	for (i=0; i<UBINOS__UBIK_TEST__MEMLEAK_TESTCOUNT; i++) {
		for (j=0; j<UBINOS__UBIK_TEST__MEMLEAK_SIGOBJCOUNT; j++) {
			condv_p[j] = NULL;
		}
		for (j=0; j<UBINOS__UBIK_TEST__MEMLEAK_SIGOBJCOUNT; j++) {
			r = condv_create(&condv_p[j]);
			if (0 != r) {
				printf("\n");
				printf("test %d, condv %d\n", i, j);
				printf("fail at condv_create(), err=%d\n", r);
				r = -1;
				break;
			}
		}
		for (j=0; j<UBINOS__UBIK_TEST__MEMLEAK_SIGOBJCOUNT; j++) {
			if (NULL != condv_p[j]) {
				r2 = condv_delete(&condv_p[j]);
				if (0 != r2) {
					printf("\n");
					printf("fail at condv_delete(), err=%d\n", r2);
					r = -1;
					break;
				}
			}
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
		printf("Pass %d tests creating and deleting %d condition variable\n", i, j);
	}

	free(condv_p);

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

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_CONDVTESTSET == 1) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */
