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

int ubik_test_misctest01(void) {
	int r;

	printf("\n");
	printf("<test>\n");
	printf("<name>ubik_test_misctest01</name>\n");
	printf("<description>System information</description>\n");

	printf("<message>\n");

	printf("\n");
	printf("----------------------------------------------------------------------------------------------------\n");
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
	r = heap_printheapinfo(NULL);
	if (0 < r) {
		printf("fail at heap_printheapinfo\n");
		r = -1;
		goto end0;
	}
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("\n");
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

	r = ubik_printkernelinfo();
	if (0 != r) {
		printf("fail at ubik_printkernelinfo\n");
		r = -1;
		goto end0;
	}

end0:
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
