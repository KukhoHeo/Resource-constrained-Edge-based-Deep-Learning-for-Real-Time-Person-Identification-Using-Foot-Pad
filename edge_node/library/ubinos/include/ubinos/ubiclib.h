/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_H_
#define UBICLIB_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file ubiclib.h
 *
 * @brief ubiclib (Ubinos C Library) API
 *
 * ubiclib (Ubinos C Library) API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

#if !defined(__cplusplus) || (!defined(ARDUINO) && (INCLUDE__ARDUINOCORE_API != 1) && (INCLUDE__ARDUINOCORE_MBED != 1) && (INCLUDE__ARDUINOCORE_STM32 != 1))

#ifndef min
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif
#ifndef max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#endif /* !defined(__cplusplus) || (!defined(ARDUINO) && (INCLUDE__ARDUINOCORE_API != 1) && (INCLUDE__ARDUINOCORE_MBED != 1) && (INCLUDE__ARDUINOCORE_STM32 != 1)) */

/*!
 * 부호 없는 정수 지수승 연산을 수행하는 함수
 *
 * @param	x		지수
 *
 * @param	y		승수
 *
 * @return	y power of x<br>
 */
unsigned int uipow(unsigned int x, unsigned int y);

/*!
 * 부호 없는 정수 x와 d의 x/d 결과의 올림값을 구하는 함수<br>
 *
 * @param	x		피제수 (dividend)<br>
 *
 * @param	d		제수 (divisor)<br>
 * 					x가 0이면 d가 0이더라고 오류로 처리하지 않음<br>
 *
 * @return	x/d 결과의 올림값<br>
 */
unsigned int uidiv_ceil(register unsigned int x, register unsigned int d);

/*!
 * 부호 없는 정수의 값이 1인 최상위 비트까지 0의 수를 구하는 함수<br>
 * Leading Zero Count<br>
 *
 * @param	x		x 값
 *
 * @return	값이 1인 최상위 비트까지 0의 수<br>
 */
unsigned int uilzc(register unsigned int x);

/*!
 * 부호 없는 정수의 값이 1인 최하위 비트를 구하는 함수<br>
 * Least Significant 1 Bit<br>
 *
 * @param	x		x 값
 *
 * @return	값이 1인 최하위 비트<br>
 */
unsigned int uilsb(register unsigned int x);

/*!
 * 부호 없는 정수 x의 log2의 올림값을 구하는 함수<br>
 * Log2 of an Integer x<br>
 *
 * @param	x		x 값
 *
 * @return	log2(x)의 올림값<br>
 */
unsigned int uilog2_ceil(register unsigned int x);

/*!
 * 부호 없는 정수 x의 log2의 내림값을 구하는 함수<br>
 * Floor of Log2 of an Integer x<br>
 *
 * @param	x		x 값
 *
 * @return	log2(x)의 내림값<br>
 */
unsigned int uilog2_floor(register unsigned int x);

/*!
 * 부호 없는 정수 x 보다 큰 가장 작은 2의 거듭제곱 값을 찾는 함수<br>
 * Next Largest Power of 2<br>
 *
 * @param	x		대상 값
 *
 * @return	x 보다 큰 가장 작은 2의 거듭제곱 값<br>
 */
unsigned int uinlp2(register unsigned int x);

/*!
 * 부호 없는 정수 x 보다 크거나 같은 가장 작은 2의 거듭제곱 값을 찾는 함수<br>
 * Equal or Next Largest Power of 2<br>
 *
 * @param	x		대상 값
 *
 * @return	x 보다 크거나 같은 가장 작은 2의 거듭제곱 값<br>
 */
unsigned int uienlp2(register unsigned int x);

/*!
 * 부호 없는 정수의 값이 1인 최상위 비트를 구하는 함수<br>
 * Most Significant 1 Bit<br>
 *
 * @param	x		대상 값
 *
 * @return	최상위 1 비트 위치<br>
 */
unsigned int uimsb(register unsigned int x);

/*!
 * 부호 없는 정수의 값이 1인 비트 수를 구하는 함수<br>
 * Population Count (Ones Count)<br>
 *
 * @param	x		대상 값
 *
 * @return	값이 1인 비트 수<br>
 */
unsigned int uiones(register unsigned int x);

#include <ubinos/ubiclib/logm.h>
#include <ubinos/ubiclib/bitmap.h>
#include <ubinos/ubiclib/cirbuf.h>
#include <ubinos/ubiclib/cbuf.h>
#include <ubinos/ubiclib/dlist.h>
#include <ubinos/ubiclib/edlist.h>
#include <ubinos/ubiclib/heap.h>
#include <ubinos/ubiclib/list.h>
#include <ubinos/ubiclib/stdlib.h>
#include <ubinos/ubiclib/cli.h>

#ifdef	__cplusplus
}
#endif

#endif /* UBICLIB_H_ */
