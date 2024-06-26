/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_BSP_DTTY_H_
#define UBINOS_BSP_DTTY_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file dtty.h
 *
 * @brief BSP debug terminal API
 *
 * BSP 디버그 터미널 API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*!
 * 디버깅 터미널을 초기화하는 함수
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1: 오류
 */
int dtty_init(void);

/*!
 * 디버깅 터미널을 활성화하는 함수
 *
 * 이 함수는 dtty_init 함수에서 자동적으로 호출됩니다.
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1 : 오류
 * 			-10: 시스템이 이 함수를 지원하지 않음
 */
int dtty_enable(void);

/*!
 * 디버깅 터미널을 비활성화하는 함수
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1 : 오류
 * 			-10: 시스템이 이 함수를 지원하지 않음
 */
int dtty_disable(void);

/*!
 * 디버깅 터미널 오류가 있는지를 검사하는 함수
 *
 * @return	 0: 없음<br>
 * 			 other: 오류 번호<br>
 */
int dtty_geterror(void);

/*!
 * 디버깅 터미널에서 문자를 입력받는 함수 (입력이 들어올 때까지 기다림)
 *
 * @param	ch_p	입력받은 문자를 저장할 변수의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_getc(char * ch_p);

/*!
 * 디버깅 터미널에서 문자를 입력받는 함수 (입력이 없으면 -1을 돌려줌)
 *
 * @param   ch_p    입력받은 문자를 저장할 변수의 주소
 *
 * @return    0: 성공<br>
 *          <br>
 *           -1: 오류<br>
 *           -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_getc_unblocked(char * ch_p);

/*!
 * 디버깅 터미널에 문자를 출력하는 함수
 *
 * @param	ch		출력할 문자
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1: 오류
 */
int dtty_putc(int ch);

/*!
 * 디버깅 터미널 출력 버퍼에 저장된 내용을 모두 출력하는 함수
 *
 * @return   0: 성공<br>
 *          <br>
 *          -1: 오류
 */
int dtty_flush(void);

/*!
 * 디버깅 터미널에 문자열을 출력하는 함수 (NULL도 출력함)
 *
 * @param	str		출력할 문자열
 *
 * @param	len		출력할 문자열의 크기
 *
 * @return	 출력한 문자열의 크기
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_putn(const char * str, int len);

/*!
 * 디버깅 터미널 입력 버퍼에 입력받은 문자가 있는지를 검사하는 함수
 *
 * @return	 1: 있음<br>
 * 			 0: 없음<br>
 */
int dtty_kbhit(void);

/*!
 * 디버깅 터미널에 문자열을 출력하는 함수
 *
 * "\0" 이전까지 또는 출력할 문자열의 최대 크기까지 출력한다.
 * 
 * @param	str		출력할 문자열
 *
 * @param	max		출력할 문자열의 최대 크기
 *
 * @return	 출력한 문자열의 크기
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_puts(const char * str, int max);

/*!
 * 디버깅 터미널에서 문자열을 입력받는 함수
 *
 * "\n", "\r", 또는 "\0" 이 입력되거나 최대 크기보다 작을 때까지 입력받는다.
 * 입력받은 "\n", "\r"은 퍼버에 저장되지 않는다.
 *
 * @param	str		입력받은 문자열을 저장할 버퍼
 *
 * @param	max		입력받을 문자열의 최대 크기
 *
 * @return	 입력받은 문자열의 크기
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_gets(char * str, int max);

/*!
 * 디버깅 터미널 에코 설정 함수
 *
 * @param	echo	0: echo off<br>
 * 					1: echo on<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_setecho(int echo);

/*!
 * 디버깅 터미널 에코 설정을 돌려주는 함수
 *
 * @return    1: echo on<br>
 *            0: echo off<br>
 */
int dtty_getecho();

/*!
 * 디버깅 터미널 자동 캐리지 리턴 설정 함수
 *
 * 디버깅 터미널에 "\n" 출력시 그 앞에 "\r"을 자동 출력하는 기능 활성화 여부를 설정한다.
 *
 * @param	autocr	0: auto carriage return off<br>
 * 					1: auto carriage return on<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_setautocr(int autocr);

/*!
 * 디버깅 터미널 자동 캐리지 리턴 설정을 돌려주는 함수
 *
 * @return    0: auto carriage return off<br>
 *            1: auto carriage return on<br>
 */
int dtty_getautocr();

#ifdef	__cplusplus
}
#endif

#endif /* UBINOS_BSP_DTTY_H_ */
