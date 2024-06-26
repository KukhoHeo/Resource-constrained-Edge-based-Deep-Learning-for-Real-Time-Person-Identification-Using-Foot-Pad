/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_CBUF_H_
#define UBICLIB_CBUF_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * @file cbuf.h
 *
 * @brief ubiclib simple circular buffer API
 *
 * ubiclib 단순환형버퍼 API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*! 단순환형버퍼 형 정의 */
typedef struct _cbuf_t
{
    uint32_t head; /*!< Head index */
    uint32_t tail; /*!< Tail index */
    uint32_t size; /*!< Buffer size */
    uint8_t * buf; /*!< Buffer pointer */
} cbuf_t;

/*! 단순환형버퍼 포인터 형 정의 */
typedef cbuf_t * cbuf_pt;

/*!
 * 정적으로 단순환형버퍼를 선언하고 초기화하는 매크로
 *
 * @param   _name   단순환형버퍼 이름
 *
 * @param   _size   버퍼 크기
 */
#define cbuf_def_init(_name, _size)         \
    uint8_t _name##_cbuf_buf[_size];        \
    cbuf_t _name##_cbuf = {                 \
        .head = 0,                          \
        .tail = 0,                          \
        .size = _size,                      \
        .buf = _name##_cbuf_buf,            \
    };                                      \
    cbuf_pt _name = &_name##_cbuf;

/*!
 * 동적으로 단순환형버퍼를 생성하는 함수
 *
 * @param   cbuf_p  생성한 단순환형버퍼의 주소를 저장할 포인터의 주소
 *
 * @param   size    버퍼 크기
 *
 * @return  오류 번호
 */
ubi_err_t cbuf_create(cbuf_pt * cbuf_p, uint32_t size);

/*!
 * 동적으로 단순환형버퍼를 제거하는 함수
 *
 * @param   cbuf_p  제거할 단순환형버퍼의 주소가 저장된 포인터의 주소<br>
 *                  제거에 성공하면 *cbuf_p는 NULL이 됨
 *
 * @return  오류 번호
 */
ubi_err_t cbuf_delete(cbuf_pt * cbuf_p);

/*!
 * 단순환형버퍼에 자료를 쓰는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @param   buf         쓸 자료가 저장되어 있는 버퍼
 *
 * @param   len         쓸 자료 길이
 *
 * @param   written_p   쓴 자료 길이를 저장할 변수의 주소 (NULL이면 무시)
 *
 * @return  오류 번호
 */
ubi_err_t cbuf_write(cbuf_pt cbuf, const uint8_t * buf, uint32_t len, uint32_t * written_p);

/*!
 * 단순환형버퍼에서 자료를 읽는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @param   buf         읽은 자료를 저장할 버퍼
 *
 * @param   size        읽을 자료 크기
 *
 * @param   read_p      읽은 자료 크기를 저장할 변수의 주소 (NULL이면 무시)
 *
 * @return  오류 번호
 */
ubi_err_t cbuf_read(cbuf_pt cbuf, uint8_t * buf, uint32_t len, uint32_t * read_p);

/*!
 * 단순환형버퍼의 자료를 모두 지우는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  오류 번호
 */
ubi_err_t cbuf_clear(cbuf_pt cbuf);

/*!
 * 단순환형버퍼에 들어있는 자료의 길이를 돌려주는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  사용된 길이
 */
uint32_t cbuf_get_len(cbuf_pt cbuf);

/*!
 * 단순환형버퍼가 가득 차있는지 여부를 돌려주는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  가득 차있는지 여부
 */
uint8_t cbuf_is_full(cbuf_pt cbuf);

/*!
 * 단순환형버퍼의 머리 부분 주소를 돌려주는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  머리 부분 주소
 */
uint8_t * cbuf_get_head_addr(cbuf_pt cbuf);

/*!
 * 단순환형버퍼의 꼬리 부분 주소를 돌려주는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  꼬리 부분 주소
 */
uint8_t * cbuf_get_tail_addr(cbuf_pt cbuf);

/*!
 * 단순환형버퍼에 들어있는 자료 중 연속된 영역의 길이를 돌려주는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  자료 중 연속된 영역의 길이
 */
uint32_t cbuf_get_contig_len(cbuf_pt cbuf);

/*!
 * 단순환형버퍼에 빈 공간 중 연속된 영역의 길이를 돌려주는 함수
 *
 * @param   cbuf        대상 단순환형버퍼 포인터
 *
 * @return  빈 공간 중 연속된 영역의 길이
 */
uint32_t cbuf_get_contig_empty_len(cbuf_pt cbuf);

#ifdef __cplusplus
}
#endif

#endif /* UBICLIB_CBUF_H_ */

