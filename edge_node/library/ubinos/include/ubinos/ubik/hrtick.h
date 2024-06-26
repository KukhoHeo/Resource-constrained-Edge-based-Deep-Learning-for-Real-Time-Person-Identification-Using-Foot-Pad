/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBIK_HRTICK_H_
#define UBIK_HRTICK_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file hrtick.h
 *
 * @brief ubik high resolution tick API
 *
 * ubik 고해상도 tick API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*!  고해상도 tick 자료 구조 */
typedef struct _hrtick_t
{
	unsigned int high; /*!< tick high bits */
	unsigned int low; /*!< tick low bits */
	unsigned int hrtick; /*!< tick high high bits */
} hrtick_t;

/*!
 * 고해상도 tick 기능을 활성화하는 함수
 *
 * @param	htimerno	고해상도 tick을 위한 하드웨어 타이머 번호<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_enable(int htimerno);

/*!
 * 고해상도 tick 기능을 비활성화하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_disable(void);

/*!
 * 고해상도 tick counter를 초기화 하는 함수 (hrtick만 초기화 됨)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_reset(void);

/*!
 * 초당 tick 수(high, low)를 돌려주는 함수
 *
 * @return	초당 tick 수(high, low)<br>
 */
unsigned int ubik_hrtick_gettickpersec(void);

/*!
 * 천분의 일초당 고해상도 tick 수(htick)를 돌려주는 함수
 *
 * @return	천분의 일초당 고해상도 tick 수(htick)<br>
 */
unsigned int ubik_hrtick_gethrtickpermsec(void);

/*!
 * 현재까지 누적된 고해상도 tick 수를 돌려주는 함수
 *
 * @param	tick_p	현재까지 누적된 고해상도 tick 수를 저장할 변수의  주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_gettick(hrtick_t * tick_p);

/*!
 * 고해상도 tick 수의 차이(tick2에서 tick1을 뺀 값)를 돌려주는 함수
 *
 * 비교 대상 tick 1이 비교 대상 tick 2보다 클 경우 오버플로우가 일어난 것으로 간주하고 차이를 계산함
 *
 * @param	tick1_p		비교 대상 고해상도 tick 1의 포인터<br>
 * 			<br>
 *
 * @param	tick2_p		비교 대상 고해상도 tick 2의 포인터<br>
 * 			<br>
 *
 * @param	tickdiff_p 	고해상도 tick 수의 차이를 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_gettickdiff(hrtick_t * tick1_p, hrtick_t * tick2_p,
		hrtick_t * tickdiff_p);

/*!
 * 고해상도 tick 수를 시간 단위로 변환하는 함수
 *
 * @param	tick_p	대상 고해상도 tick 수의 포인터<br>
 *
 * @param	nsec_p	변환한 십억분의 일초 단위 값을 저장할 변수의 주소<br>
 * 					NULL이면 무시함<br>
 *
 * @param	usec_p	변환한 백만분의 일초 단위 값을 저장할 변수의 주소<br>
 * 					NULL이면 무시함<br>
 *
 * @param	msec_p	변환한 천분의 일초 단위 값을 저장할 변수의 주소<br>
 * 					NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_hrticktotime(hrtick_t * tick_p, unsigned int * nsec_p,
		unsigned int * usec_p, unsigned int * msec_p);

/*!
 * 고해상도 tick 수를 십억분의 일초 단위로 변환하는 함수
 *
 * @param	tick_p	대상 고해상도 tick 수의 포인터<br>
 *
 * @param	nsec_p	변환한 십억분의 일초 단위 값을 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_hrticktotimens(hrtick_t * tick_p, unsigned int * nsec_p);

/*!
 * 고해상도 tick 수를 백만분의 일초 단위로 변환하는 함수
 *
 * @param	tick_p	대상 고해상도 tick 수의 포인터<br>
 *
 * @param	usec_p	변환한 백만분의 일초 단위 값을 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_hrtick_hrticktotimeus(hrtick_t * tick_p, unsigned int * usec_p);

#ifdef	__cplusplus
}
#endif

#endif /* UBIK_HRTICK_H_ */
