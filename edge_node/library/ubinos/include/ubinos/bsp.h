/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_BSP_H_
#define UBINOS_BSP_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file bsp.h
 *
 * @brief ubinos BSP (Board Support Package) API
 *
 * ubinos BSP (Board Support Package) API를 정의합니다.
 */

/*!
 * @example	misctest01.c
 * @example	misctest02.c
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*!
 * 응용 코드 시작 함수
 *
 * 응용 코드의 시작점이 되는 함수 입니다.
 * 응용 개발자가 직접 만들어주어야 하며, 시스템 시작시 BSP 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @param	argc	매개변수 배열 argv의 길이<br>
 *					<br>
 *
 * @param	argv	매개변수 배열 (BSP 컴포넌트 작성자가 임의로 정의함)<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int appmain(int argc, char * argv[]);

/*!
 * 인터럽트 서비스 루틴을 수행 중인지 여부를 알려주는 함수
 *
 * @return	1: 인터럽트 서비스 루틴을 수행 중임<br>
 * 			0: 인터럽트 서비스 루틴을 수행 중임이 아님<br>
 */
int bsp_isintr(void);

/*!
 * CPU 클럭 주파수(Hz)를  돌려주는 함수
 *
 * @param	freq_p	CPU 클럭 주파수(Hz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getcpuclockfreq(unsigned int * freq_p);

/*!
 * CPU 클럭 주파수(KHz)를  돌려주는 함수
 *
 * @param	freqk_p	CPU 클럭 주파수(KHz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getcpuclockfreqk(unsigned int * freqk_p);

/*!
 * 마스터 클럭 주파수(Hz)를  돌려주는 함수
 *
 * @param	freq_p	마스터  클럭 주파수(Hz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmckfreq(unsigned int * freq_p);

/*!
 * 마스터 클럭 주파수(KHz)를  돌려주는 함수
 *
 * @param	freqk_p	마스터  클럭 주파수(KHz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmckfreqk(unsigned int * freqk_p);

/*!
 * CPU ID를 돌려주는 함수
 *
 * @param	buf		CPU ID를 저장할 버퍼<br>
 *					<br>
 *
 * @param	max		CPU ID를 저장할 버퍼의 최대 크기<br>
 *					<br>
 *
 * @return	CPU ID 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getcpuid(unsigned char * buf, int max);

/*!
 * CPU ID의 크기를 돌려주는 함수
 *
 * @return	CPU ID 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int bsp_getcpuidsize(void);

/*!
 * 모든 인터럽트를 활성화하는 함수
 *
 * intr_enable 함수로 활성화된 번호의 인터럽트만 활성화함
 */
void bsp_enableintr(void);

/*!
 * 모든 인터럽트를 비활성화하는 함수
 */
void bsp_disableintr(void);

/*!
 * 시스템을 중지시키는 함수
 */
void bsp_abortsystem(void);

/*!
 * 스택 영역 크기를 돌려주는 함수
 *
 * @param	type	스택 영역 종류<br>
 *					<br>
 * 					0: system
 *					<br>
 *
 * @param	stacksize_p	스택 영역 크기를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getstacksize(int type, unsigned int * stacksize_p);

/*!
 * 스택 사용량 최고 기록을 돌려주는 함수
 *
 * @param	type			스택 영역 종류<br>
 *							<br>
 * 							0: system
 *							<br>
 *
 * @param	maxstackusage_p	스택 사용량 최고 기록을 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmaxstackusage(int type, unsigned int * maxstackusage_p);

/*!
 * CPU를 점유한 채로 기다리는 함수
 *
 * @param	count	기다릴 시간 (루프 횟수)<br>
 *					<br>
 */
void bsp_busywait(unsigned int count);

/*!
 * CPU를 점유한 채로 기다리는 함수 (천분의 일초 단위)
 *
 * @param	timems	기다릴 시간 (천분의 일초)<br>
 *					<br>
 */
void bsp_busywaitms(unsigned int timems);

/*!
 * 천분의 일초를 busy wait 하기 위해 필요한 루프 횟수를 돌려주는 함수
 *
 * @return	 천분의 일초를 busy wait 하기 위해 필요한 루프 횟수<br>
 */
unsigned int bsp_getbusywaitcountperms(void);

/*!
 * 시간을 busy wait count로 변환하는 함수
 *
 * @param	timems	시간(천분의 일초)<br>
 * 					<br>
 *
 * @return	busy wait count<br>
 */
unsigned int bsp_timemstobwc(unsigned int timems);

/*!
 * busy wait count를 시간으로 변환하는 함수
 *
 * @param	count	busy wait count<br>
 * 					<br>
 *
 * @return	시간(천분의 일초)<br>
 */
unsigned int bsp_bwctotimems(unsigned int count);

/*!
 * CPU를 점유한 채로 기다리는 함수 (백만분의 일초 단위)
 *
 * @param	timeus	기다릴 시간 (백만분의 일초)<br>
 *					<br>
 */
void bsp_busywaitus(unsigned int timeus);

#include <ubinos/bsp/dtty.h>
#include <ubinos/bsp/intr.h>

#ifdef	__cplusplus
}
#endif

#endif /* UBINOS_BSP_H_ */
