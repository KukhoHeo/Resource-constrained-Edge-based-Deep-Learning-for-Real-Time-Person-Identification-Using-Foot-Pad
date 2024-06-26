/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubiclib_test.h>

#if (INCLUDE__UBINOS__UBICLIB_TEST == 1)
#if !(UBINOS__UBICLIB_TEST__EXCLUDE_CBUF_TEST == 1)

#include <stdio.h>
#include <stdlib.h>

ubi_err_t ubiclib_test_test_all()
{
    ubi_err_t ubi_err;
    int r;
    int test_count = 0;
    int fail_count = 0;
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
    unsigned int size_old;
    unsigned int count_old;
    unsigned int size;
    unsigned int count;
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

    do
    {
        printf("\n");
        printf("<testset>\n");
        printf("<name>ubiclib_test</name>\n");
        printf("<description>Ubinos C Library Test</description>\n");
        printf("\n");

#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
        ubik_collectgarbage();

        r = heap_getallocatedsize(NULL, &size_old);
        if (0 != r)
        {
            printf("<message>\n");
            printf("fail at heap_getallocatedsize\n");
            printf("</message>\n");
            ubi_err = UBI_ERR_HEAP;
            break;
        }

        r = heap_getallocatedcount(NULL, &count_old);
        if (0 != r)
        {
            printf("<message>\n");
            printf("fail at heap_getallocatedcount\n");
            printf("</message>\n");
            ubi_err = UBI_ERR_HEAP;
            break;
        }
#endif /* !(UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

#if !(UBINOS__UBICLIB__EXCLUDE_CBUF_TEST == 1)
        printf("<!-- ====================================================================== -->\n");
        test_count++;
        ubi_err = ubik_test_cbuf_test_00();
        if (ubi_err != UBI_ERR_OK)
        {
            fail_count++;
        }

        ubik_collectgarbage();
#endif	/* !(UBINOS__UBICLIB__EXCLUDE_CBUF == 1) */

        printf("<!-- ====================================================================== -->\n");

        printf("\n");
        printf("<message>\n");

        printf("test: %d\n", test_count);
        printf("pass: %d\n", test_count - fail_count);
        printf("fail: %d\n", fail_count);

        printf("</message>\n");
        printf("\n");

#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
        ubik_collectgarbage();

        r = heap_getallocatedsize(NULL, &size);
        if (0 != r)
        {
            printf("<message>\n");
            printf("fail at heap_getallocatedsize\n");
            printf("</message>\n");
            ubi_err = UBI_ERR_HEAP;
            break;
        }

        r = heap_getallocatedcount(NULL, &count);
        if (0 != r)
        {
            printf("<message>\n");
            printf("fail at heap_getallocatedcount\n");
            printf("</message>\n");
            ubi_err = UBI_ERR_HEAP;
            break;
        }

        if (size_old != size || count_old != count)
        {
            printf("<message>\n");
            printf("memory leak was detected\n");
            printf("</message>\n");
            ubi_err = UBI_ERR_HEAP;
            break;
        }
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

        if (0 == fail_count)
        {
            ubi_err = UBI_ERR_OK;
        }
        else
        {
            ubi_err = UBI_ERR_INTERNAL;
        }
    } while (0);

    printf("<result>");
    if (ubi_err == UBI_ERR_OK)
    {
        printf("pass");
    }
    else
    {
        printf("fail");
    }
    printf("</result>\n");
    printf("</testset>\n");
    printf("\n");

    return ubi_err;
}

#endif /* !(UBINOS__UBICLIB_TEST__EXCLUDE_CBUF_TEST == 1) */
#endif /* (INCLUDE__UBINOS__UBICLIB_TEST == 1) */

