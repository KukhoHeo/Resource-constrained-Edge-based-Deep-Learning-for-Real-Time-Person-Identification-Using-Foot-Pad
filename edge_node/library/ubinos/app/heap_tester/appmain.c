/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ubinos/ubiclib_test.h>

int appmain(int argc, char *argv[])
{
    int r;
    (void) r;
    ubiclib_test_heaptest_param_t params;
    int ci[] = {
            10, 11, 12, // group system
            13, 14, 15, // binary buddy system
            16, 17, 18, // weighted buddy system
            19, 20, 21, // best fit
            22, 23, 24, // first fit
            25, 26, 27, // next fit
            28, 29, 30, // pure group system
    };
    int cilen = sizeof(ci) / sizeof(int);
    unsigned int fblcount;

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("heap tester (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    r = ubik_hrtick_enable(1);
    ubi_assert(r == 0);

    params.test_index = 1;
    params.heapsize = 8 * 1024 * 1024;
    params.data_length = UBICLIB_TEST_HEAPTEST_DATA_LENGTH;
    params.lifetime_max = 10;
    params.stepcount_max = 128;
    params.size_max = params.heapsize / 10;
    params.size_min = max((sizeof(edlist_elmt_t)), 64);
    params.mean_step = (params.heapsize / 1024 - 512) / 128;
    params.mean_min = 512 + params.mean_step;
    params.testcount_max = 100;
    params.critlock = 1;
    params.blockcheck = 0;
    params.skipfirsfull = 0;

    printf("\n");
    printf("begin\n");
    printf("\n");

    for (int i = 0; i < cilen; i++) {
        // group system
        if (ci[i] == 10) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP;
            params.m                 = 64;
            fblcount                 = heap_group_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("GROUP%d, Uniform, Uniform, %u, %u, %u\n", params.m, params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 11) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP;
            params.m                 = 64;
            fblcount                 = heap_group_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("GROUP%d, Uniform, Exponential, %u, %u, %u\n", params.m, params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 12) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP;
            params.m                 = 64;
            fblcount                 = heap_group_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("GROUP%d, Uniform, Normal, %u, %u, %u\n", params.m, params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // binary buddy system
        else if (ci[i] == 13) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY;
            params.m                 = 2;
            fblcount                 = heap_pgroup_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("BBUDDY, Uniform, Uniform, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 14) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY;
            params.m                 = 2;
            fblcount                 = heap_pgroup_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("BBUDDY, Uniform, Exponential, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 15) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY;
            params.m                 = 2;
            fblcount                 = heap_pgroup_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("BBUDDY, Uniform, Normal, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // weighted buddy system
        else if (ci[i] == 16) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY;
            params.m                 = 2;
            fblcount                 = heap_wbuddy_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("WBUDDY, Uniform, Uniform, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 17) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY;
            params.m                 = 2;
            fblcount                 = heap_wbuddy_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("WBUDDY, Uniform, Exponential, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));

        }
        else if (ci[i] == 18) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY;
            params.m                 = 2;
            fblcount                 = heap_wbuddy_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("WBUDDY, Uniform, Normal, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // best fit
        else if (ci[i] == 19) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("BEST, Uniform, Uniform, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 20) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("BEST, Uniform, Exponential, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 21) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("BEST, Uniform, Normal, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // first fit
        else if (ci[i] == 22) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("FIRST, Uniform, Uniform, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 23) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("FIRST, Uniform, Exponential, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 24) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("FIRST, Uniform, Normal, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // next fit
        else if (ci[i] == 25) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("NEXT, Uniform, Uniform, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 26) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("NEXT, Uniform, Exponential, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 27) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT;
            params.m                 = 2;
            fblcount                 = 1;

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("NEXT, Uniform, Normal, %u, %u, %u\n", params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // pure group system
        else if (ci[i] == 28) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP;
            params.m                 = 64;
            fblcount                 = heap_pgroup_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_urx2_a[0];

            printf("\n");
            printf("PGROUP%d, Uniform, Uniform, %u, %u, %u\n", params.m, params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 29) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP;
            params.m                 = 64;
            fblcount                 = heap_pgroup_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_erx2_a[0];

            printf("\n");
            printf("PGROUP%d, Uniform, Exponential, %u, %u, %u\n", params.m, params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }
        else if (ci[i] == 30) {
            params.algorithm_type     = UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP;
            params.m                 = 64;
            fblcount                 = heap_pgroup_calc_fblcount_raw(params.heapsize, params.m);

            params.lifetime_type     = UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM;
            params.size_type         = UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL;
            params.lifetime_p         = &ubiclib_test_heaptest_urx1_a[0];
            params.size_p             = &ubiclib_test_heaptest_nrx2_a[0];

            printf("\n");
            printf("PGROUP%d, Uniform, Normal, %u, %u, %u\n", params.m, params.heapsize, fblcount, bitmap_getmemsize(fblcount));
        }

        // undefined
        else {
            printf("undefined config index\n");
            break;
        }

        ubiclib_test_heaptest(&params);
    }

    printf("\n");
    printf("end\n");
    printf("\n");

    return 0;
}

