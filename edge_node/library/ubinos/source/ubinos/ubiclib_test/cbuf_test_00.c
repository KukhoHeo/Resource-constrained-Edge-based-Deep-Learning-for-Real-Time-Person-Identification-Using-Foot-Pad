/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if (INCLUDE__UBINOS__UBICLIB_TEST == 1)
#if !(UBINOS__UBIK_TEST__EXCLUDE_CBUF_TEST == 1)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define UBIK_TEST_CBUF_BUF_SIZE 8

static uint8_t _g_buf[UBIK_TEST_CBUF_BUF_SIZE * 4];

cbuf_def_init(_g_cbuf, UBIK_TEST_CBUF_BUF_SIZE);

static void ubik_test_cbuf_print(cbuf_pt cbuf)
{
    assert(cbuf != NULL);

    printf("    size = %6ld, head = %6ld, tail = %6ld, len = %6ld\n", cbuf->size, cbuf->head, cbuf->tail, cbuf_get_len(cbuf));
    printf("        data :");
    for (uint32_t i = 0; i < UBIK_TEST_CBUF_BUF_SIZE; i++)
    {
        printf("%c", cbuf->buf[i]);
    }
    printf("\n");
}

static ubi_err_t ubik_test_cbuf_test_00_func(cbuf_pt cbuf)
{
    ubi_err_t ubi_err;
    uint8_t *buf;
    uint32_t len;
    uint32_t written, read;

    buf = _g_buf;

    do
    {
        sprintf((char*) buf, "abcd");
        len = strlen((char*) buf);
        ubi_err = cbuf_write(cbuf, buf, len, &written);
        assert(ubi_err == UBI_ERR_OK);
        printf("cbuf: 01: write: %ld\n", written);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 0 || cbuf->tail != 4)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        sprintf((char*) buf, "efg");
        len = strlen((char*) buf);
        ubi_err = cbuf_write(cbuf, buf, len, &written);
        assert(ubi_err == UBI_ERR_OK);
        printf("cbuf: 02: write: %ld\n", written);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 0 || cbuf->tail != 7)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        memset(buf, 0, UBIK_TEST_CBUF_BUF_SIZE);
        len = 5;
        ubi_err = cbuf_read(cbuf, buf, len, &read);
        assert(ubi_err == UBI_ERR_OK);
        buf[read] = 0;
        printf("cbuf: 03: read : %ld: %s\n", read, (char*) buf);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 5 || cbuf->tail != 7)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        sprintf((char*) buf, "hijk");
        len = strlen((char*) buf);
        ubi_err = cbuf_write(cbuf, buf, len, &written);
        assert(ubi_err == UBI_ERR_OK);
        printf("cbuf: 04: write: %ld\n", written);
        ubik_test_cbuf_print(cbuf);
        printf("        buf addr   : 0x%08x\n", (uintptr_t) cbuf->buf);
        printf("        head addr  : 0x%08x\n", (uintptr_t) cbuf_get_head_addr(cbuf));
        printf("        contig. len: %ld\n", cbuf_get_contig_len(cbuf));
        printf("\n");
        if (cbuf->head != 5 || cbuf->tail != 3)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        memset(buf, 0, UBIK_TEST_CBUF_BUF_SIZE);
        len = 4;
        ubi_err = cbuf_read(cbuf, buf, len, &read);
        assert(ubi_err == UBI_ERR_OK);
        buf[read] = 0;
        printf("cbuf: 05: read : %ld: %s\n", read, (char*) buf);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 1 || cbuf->tail != 3)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        memset(buf, 0, UBIK_TEST_CBUF_BUF_SIZE);
        len = 4;
        ubi_err = cbuf_read(cbuf, buf, len, &read);
        assert(ubi_err == UBI_ERR_BUF_EMPTY);
        buf[read] = 0;
        printf("cbuf: 06: read : %ld: %s\n", read, (char*) buf);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 3 || cbuf->tail != 3)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        sprintf((char*) buf, "lmnopqrstuvwxyz");
        len = strlen((char*) buf);
        ubi_err = cbuf_write(cbuf, buf, len, &written);
        assert(ubi_err == UBI_ERR_BUF_FULL);
        printf("cbuf: 07: write: %ld\n", written);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 3 || cbuf->tail != 2)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        memset(buf, 0, UBIK_TEST_CBUF_BUF_SIZE);
        len = 12;
        ubi_err = cbuf_read(cbuf, buf, len, &read);
        assert(ubi_err == UBI_ERR_BUF_EMPTY);
        buf[read] = 0;
        printf("cbuf: 08: read : %ld: %s\n", read, (char*) buf);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 2 || cbuf->tail != 2)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        sprintf((char*) buf, "stuvw");
        len = strlen((char*) buf);
        ubi_err = cbuf_write(cbuf, buf, len, &written);
        assert(ubi_err == UBI_ERR_OK);
        printf("cbuf: 09: write: %ld\n", written);
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 2 || cbuf->tail != 7)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        ubi_err = cbuf_clear(cbuf);
        assert(ubi_err == UBI_ERR_OK);
        printf("cbuf: 10: clear\n");
        ubik_test_cbuf_print(cbuf);
        printf("\n");
        if (cbuf->head != 7 || cbuf->tail != 7)
        {
            printf("fail, ubi_err=%d\n", ubi_err);
            ubi_err = UBI_ERR_INTERNAL;
            break;
        }

        ubi_err = UBI_ERR_OK;
    } while (0);

    return ubi_err;
}

ubi_err_t ubik_test_cbuf_test_00()
{
    ubi_err_t ubi_err;
    cbuf_pt cbuf;

    ubi_err = cbuf_create(&cbuf, UBIK_TEST_CBUF_BUF_SIZE);
    assert(ubi_err == UBI_ERR_OK);
    memset(cbuf->buf, 0, cbuf->size);
    printf("cbuf: create\n");
    ubik_test_cbuf_print(cbuf);
    printf("\n");

    ubi_err = ubik_test_cbuf_test_00_func(cbuf);
    assert(ubi_err == UBI_ERR_OK);

    ubi_err = cbuf_delete(&cbuf);
    assert(ubi_err == UBI_ERR_OK);
    printf("cbuf: delete\n");
    printf("\n");

    printf("\n");
    printf("\n");

    cbuf = _g_cbuf;
    memset(cbuf->buf, 0, cbuf->size);
    printf("cbuf: static\n");
    ubik_test_cbuf_print(cbuf);
    printf("\n");

    ubi_err = ubik_test_cbuf_test_00_func(cbuf);
    assert(ubi_err == UBI_ERR_OK);

    ubi_err = UBI_ERR_OK;

    return ubi_err;
}

#endif /* !(UBINOS__UBIK_TEST__EXCLUDE_CBUF_TEST == 1) */
#endif /* (INCLUDE__UBINOS__UBICLIB_TEST == 1) */

