/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_BITMAP_H_
#define UBICLIB_BITMAP_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file bitmap.h
 *
 * @brief ubiclib bitmap API
 *
 * ubiclib 비트맵 API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*!
 * Max bit size is (INT_SIZE * 8) ^ BITMAP_DEPTH_MAX
 * if BITMAP_DEPTH_MAX is 4 in 32 bit machine, then max bit size is 32 ^  = 1,048,576 bits
 */
#define BITMAP_DEPTH_MAX	4

/*! 비트맵 형 정의 */
typedef struct _bitmap_t
{
	unsigned int bitsize; /*!< bit size */
	unsigned int * map; /*!< map buffer pointer */
	unsigned int map_bytesize; /*!< map byte size */
	unsigned int level_depth; /*!< level depth */
	unsigned int level_offset_a[BITMAP_DEPTH_MAX]; /*!< level offset arrat */
} bitmap_t;

/*! 비트맵 포인터 형 정의 */
typedef bitmap_t * bitmap_pt;

/*! bitmap을 초기화하는 macro */
#define BITMAP_INITVALUE(bitsize)	{	\
	bitsize,							\
	NULL,								\
	0,									\
	0,									\
	{0, }								\
}

/*!
 * 비트맵를 초기화하는 함수<br>
 * 정적으로 선언된 비트맵은 이 함수를 사용해 초기화한 다음 사용해야 한다.<br>
 * bitmap_create 함수를 사용해 동적으로 만들어진 비트맵은 이 함수를 사용해 초기화할 필요가 없다.<br>
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @param	bitsize		비트맵 크기 (비트 단위)
 *
 * @param	buf			비트맵을 저장할 버퍼 포인터
 *
 * @param	bufsize		비트맵을 저장할 버퍼 크기 (바이트 단위)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bitmap_init(bitmap_pt bitmap, unsigned int bitsize, unsigned char * buf,
		unsigned int bufsize);

/*!
 * 비트맵 저장에 필요한 메모리 크기를 돌려주는 함수
 *
 * @param	bitsize			비트맵 크기 (비트 단위)
 *
 * @return	비트맵 저장에 필요한 메모리 크기 (바이트 단위)<br>
 */
unsigned int bitmap_getmemsize(unsigned int bitsize);

/*!
 * 비트맵 저장에 필요한 메모리 크기를 돌려주는 함수
 *
 * @param	bitsize			비트맵 크기 (비트 단위)
 *
 * @return	비트맵 저장에 필요한 메모리 크기 (INT_SIZE 바이트 단위)<br>
 */
unsigned int bitmap_getmapsize(unsigned int bitsize);

/*!
 * 비트맵를 생성하는 함수 (비트 초기 값은 0)
 *
 * @param	bitmap_p		생성한 비트맵의 주소를 저장할 포인터의 주소
 *
 * @param	bitsize			비트맵 크기 (비트 단위)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bitmap_create(bitmap_pt * bitmap_p, unsigned int bitsize);

/*!
 * 비트맵를 제거하는 함수
 *
 * @param	bitmap_p	제거할 비트맵의 주소가 저장된 포인터의 주소<br>
 * 						제거에 성공하면 *bitmap_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bitmap_delete(bitmap_pt * bitmap_p);

/*!
 * 비트 값을 가져오는 함수
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @param	index		대상 비트 인덱스 (LSB 부터 1)
 *
 * @return	  가져온 비트 값<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bitmap_getbit(bitmap_pt bitmap, unsigned int index);

/*!
 * 비트 값을 설정하는 함수
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @param	index		대상 비트 인덱스 (LSB 부터 1)
 *
 * @param	value		설정할 값 (0|1)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bitmap_setbit(bitmap_pt bitmap, unsigned int index, int value);

/*!
 * 모든 비트 값을 설정하는 함수
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @param	value		설정할 값 (0|1)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bitmap_setbitall(bitmap_pt bitmap, int value);

/*!
 * 값이 1인 최하위 비트의 인덱스를 구하는 함수<br>
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @return	값이 1인 최하위 비트의 인덱스 (LSB 부터 1, 값이 1이 비트가 없으면 0)<br>
 */
unsigned int bitmap_getlsb(bitmap_pt bitmap);

/*!
 * 지정한 인덱스 보다 크거나 같은 값이 1인 최하위 비트의 인덱스를 구하는 함수<br>
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @param	index		비트 인덱스 (LSB 부터 1)
 *
 * @return	지정한 인덱스 보다 크거나 같은 값이 1인 최하위 비트의 인덱스 (LSB 부터 1, 값이 1이 비트가 없으면 0)<br>
 */
unsigned int bitmap_getlsb2(bitmap_pt bitmap, unsigned int index);

/*!
 * 값이 1인 최상위 비트의 인덱스를 구하는 함수<br>
 *
 * @param	bitmap		대상 비트맵 포인터
 *
 * @return	값이 1인 최상위 비트의 인덱스 (LSB 부터 1, 값이 1이 비트가 없으면 0)<br>
 */
unsigned int bitmap_getmsb(bitmap_pt bitmap);

#ifdef	__cplusplus
}
#endif

#endif /* UBICLIB_BITMAP_H_ */
