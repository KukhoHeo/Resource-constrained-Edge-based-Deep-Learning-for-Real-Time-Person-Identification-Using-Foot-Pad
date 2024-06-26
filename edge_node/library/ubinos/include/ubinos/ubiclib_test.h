/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_TEST_H_
#define UBICLIB_TEST_H_

/*!
 * @file ubiclib_test.h
 *
 * @brief ubiclib test API
 *
 * ubiclib test API를 정의합니다.
 */

#include <ubinos.h>

#define UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__UNIFORM        1
#define UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__EXPONENTIAL    2
#define UBICLIB_TEST_HEAPTEST_LIFETIME_TYPE__NORMAL         3

#define UBICLIB_TEST_HEAPTEST_SIZE_TYPE__UNIFORM            1
#define UBICLIB_TEST_HEAPTEST_SIZE_TYPE__EXPONENTIAL        2
#define UBICLIB_TEST_HEAPTEST_SIZE_TYPE__NORMAL             3

#define UBICLIB_TEST_HEAPTEST_DATA_LENGTH                   4000

extern double ubiclib_test_heaptest_urx1_a[UBICLIB_TEST_HEAPTEST_DATA_LENGTH];
extern double ubiclib_test_heaptest_urx2_a[UBICLIB_TEST_HEAPTEST_DATA_LENGTH];
extern double ubiclib_test_heaptest_erx1_a[UBICLIB_TEST_HEAPTEST_DATA_LENGTH];
extern double ubiclib_test_heaptest_erx2_a[UBICLIB_TEST_HEAPTEST_DATA_LENGTH];
extern double ubiclib_test_heaptest_nrx1_a[UBICLIB_TEST_HEAPTEST_DATA_LENGTH];
extern double ubiclib_test_heaptest_nrx2_a[UBICLIB_TEST_HEAPTEST_DATA_LENGTH];

typedef struct _ubiclib_test_heaptest_param_t
{
    int test_index;
    unsigned int heapsize;
    unsigned int data_length;
    unsigned int lifetime_max;
    unsigned int stepcount_max;
    unsigned int size_max;
    unsigned int size_min;
    unsigned int mean_step;
    unsigned int mean_min;
    unsigned int testcount_max;
    unsigned int critlock;
    unsigned int blockcheck;
    unsigned int skipfirsfull;

    unsigned int algorithm_type;
    unsigned int m;
    unsigned int lifetime_type;
    double *lifetime_p;
    unsigned int size_type;
    double *size_p;
} ubiclib_test_heaptest_param_t;

typedef ubiclib_test_heaptest_param_t *ubiclib_test_heaptest_param_pt;

typedef struct _ubiclib_test_heaptest_result_t
{
    double sm;
    double rsm;
    double bc;

    double ac;
    double ata;
    double atd;
    double atb;
    double atw;

    double rc;
    double rta;
    double rtd;
    double rtb;
    double rtw;

    double tfa;
    double tfd;
    double tfb;
    double tfw;

    double ifa;
    double ifd;
    double ifb;
    double ifw;

    double efa;
    double efd;
    double efb;
    double efw;
} ubiclib_test_heaptest_result_t;

typedef ubiclib_test_heaptest_result_t *ubiclib_test_heaptest_result_pt;

void ubiclib_test_heaptest(void *arg);

ubi_err_t ubiclib_test_test_all(void);

ubi_err_t ubik_test_cbuf_test_00(void);

#endif /* UBICLIB_TEST_H_ */

