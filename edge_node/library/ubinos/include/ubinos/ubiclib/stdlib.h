/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_STDLIB_H_
#define UBICLIB_STDLIB_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file stdlib.h
 *
 * @brief stdlib (Standard Library) API
 *
 * stdlib API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*!
 * stdlib 포트 컴포넌트를 초기화하는 함수
 *
 * 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분은 수행하지 않습니다.<br>
 * 재진입(reenterance)을 지원하려면 stdlib_port_comp_init_reent 함수를 추가로 호출해 주어야 합니다.<br>
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int stdlib_port_comp_init(void);

/*!
 * stdlib 포트 컴포넌트의 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분을 수행하는 함수
 *
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int stdlib_port_comp_init_reent(void);

#ifdef	__cplusplus
}
#endif

#endif /* UBICLIB_STDLIB_H_ */
