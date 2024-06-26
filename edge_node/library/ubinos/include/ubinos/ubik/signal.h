/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBIK_SIGNAL_H_
#define UBIK_SIGNAL_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file signal.h
 *
 * @brief ubik signal API
 *
 * ubik signal API를 정의합니다.
 */

/*!
 * @example	signaltest01.c
 * @example	signaltest02.c
 * @example	signaltest03.c
 * @example	signaltest04.c
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

#include "task.h"

/*! API용 시그널 형 정의 */
typedef struct __signal_tip_t
{
	unsigned int type :8; /*!< Object type: OBJTYPE__UBIK_SIGNAL */
	unsigned int valid :1; /*!< Indicates whether it is valid */
	unsigned int reserved :7; /*!< Reserved */
	unsigned int reserved2 :16; /*!< Reserved 2 */
} _signal_tip_t;

/*! API용 시그널 포인터 형 정의 */
typedef _signal_tip_t * signal_pt;

/*!
 * 시그널를 생성하는 함수
 *
 * @param	signal_p	생성한 시그널의 주소를 저장할 포인터의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int signal_create(signal_pt * signal_p);

/*!
 * 시그널를 생성하는 함수 (확장형)
 *
 * @param	signal_p	생성한 시그널의 주소를 저장할 포인터의 주소
 *
 * @param	option		옵션 (SIGNAL_OPT__...)<br>
 * 						0: 기본 설정<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int signal_create_ext(signal_pt * signal_p, unsigned int option);

/*!
 * 시그널를 제거하는 함수
 *
 * @param	signal_p		제거할 시그널의 주소가 저장된 포인터의 주소<br>
 * 							제거에 성공하면 *signal_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int signal_delete(signal_pt * signal_p);

/*!
 * 시그널을 기다리는 함수
 *
 * @param	signal		대상 시그널 포인터
 *
 * @return	받은 시그널 종류<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int signal_wait(signal_pt signal);

/*!
 * 제한 시간동안 시그널을 기다리는 함수
 *
 * @param	signal		대상 시그널 포인터
 *
 * @param	tick		제한 시간 (시스템 틱 수)
 *
 * @return	받은 시그널 종류<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int signal_wait_timed(signal_pt signal, unsigned int tick);

/*!
 * 제한 시간동안 시그널을 기다리는 함수 (천분의 일초 단위)
 *
 * @param	signal		대상 시그널 포인터
 *
 * @param	timems		제한 시간 (천분의 일초)
 *
 * @return	받은 시그널 종류<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int signal_wait_timedms(signal_pt signal, unsigned int timems);

/*!
 * 기다리고 있는 태스크들 중 하나에게 시그널을 보내는 함수
 *
 * @param	signal		대상 시그널 포인터
 *
 * @param	sigtype		시그널 종류 (0 이상)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int signal_send(signal_pt signal, int sigtype);

/*!
 * 기다리고 있는 태스크들 모두에게 시그널을 보내는 함수
 *
 * @param	signal		대상 시그널 포인터
 *
 * @param	sigtype		시그널 종류 (0 이상)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int signal_broadcast(signal_pt signal, int sigtype);

/*!
 * 시그널을 보내는(시그널을 기다리는 최상위 우선순위 태스크의우선순위를 상속받을) 태스크를 설정하는 함수
 *
 * @param	signal		대상 시그널 포인터
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 설정 해제<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int signal_setsender(signal_pt signal, task_pt task);

#ifdef	__cplusplus
}
#endif

#endif /* UBIK_SIGNAL_H_ */
