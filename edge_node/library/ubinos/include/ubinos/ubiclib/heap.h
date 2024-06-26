/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_HEAP_H_
#define UBICLIB_HEAP_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file heap.h
 *
 * @brief ubiclib heap API
 *
 * ubiclib 힙 API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

/*! 상위 경계 영역이 오염되었음 */
#define	HEAP_ERR__POLLUTED_TOP			-21
/*! 하위 경계 영역이 오염되었음 */
#define	HEAP_ERR__POLLUTED_BOTTOM		-22
/*! 헤더 경계 영역이 오염되었음 */
#define	HEAP_ERR__POLLUTED_HEADER		-23
/*! 지원하지 않는 기능임 */
#define	HEAP_ERR__UNSUPPORTED			-24

/*! API용 힙 구조체 정의 */
typedef struct __heap_tip_t
{
	unsigned int type :8; /*!< Object type: OBJTYPE__UBICLIB_HEAP */
	unsigned int valid :1; /*!< Indicates whether it is valid */
	unsigned int reserved :7; /*!< Reserved */
	unsigned int reserved2 :16; /*!< Reserved 2 */
} _heap_tip_t;

/*! API용 힙 포인터 형 정의 */
typedef _heap_tip_t * heap_pt;

/*!
 * 힙 컴포넌트를 초기화하는 함수<br>
 * 
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			HEAP_ERR__UNSUPPORTED: 지원하지 않음<br>
 */
int ubiclib_heap_comp_init(void);

/*!
 * 힙 컴포넌트를 재진입(reenterance)이 가능하도록 초기화하는 함수<br>
 *
 * 이 함수는 ubiclib_heap_comp_init 함수가 호출된 다음에 호출되어야 하며, 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			HEAP_ERR__UNSUPPORTED: 지원하지 않음<br>
 */
int ubiclib_heap_comp_init_reent(void);

/*!
 * 힙을 생성하는 함수<br>
 *
 * @param	heap_p				생성한 힙의 주소를 저장할 포인터의 주소<br>
 *
 * @param	addr				힙으로 사용할 메모리의 주소<br>
 *
 * @param	size				힙으로 사용할 메모리의 크기<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_create(heap_pt * heap_p, unsigned int addr, unsigned int size);

/*!
 * 힙을 생성하는 함수의 확장형<br>
 *
 * @param	heap_p				생성한 힙의 주소를 저장할 포인터의 주소<br>
 *
 * @param	addr				힙으로 사용할 메모리의 주소<br>
 *
 * @param	size				힙으로 사용할 메모리의 크기<br>
 *
 * @param	algorithm0			정방향 힙의 알고리즘 (UBINOS__UBICLIB__HEAP_ALGORITHM__...)<br>
 *
 * @param	locktype0			정방향 힙의 잠금 방식 (UBINOS__UBICLIB__HEAP_LOCK_TYPE__...)<br>
 *
 * @param	m0					정방향 힙의 그룹 시스템 M 값<br>
 *
 * @param	fblcount0			정방향 힙의 사용가능 블록 리스트의 수<br>
 *
 * @param	algorithm1			역방향 힙의 알고리즘 (UBINOS__UBICLIB__HEAP_ALGORITHM__...)<br>
 *
 * @param	locktype1			역방향 힙의 잠금 방식 (UBINOS__UBICLIB__HEAP_LOCK_TYPE__...)<br>
 *
 * @param	m1					역방향 힙의 그룹 시스템 M 값<br>
 *
 * @param	fblcount1			역방향 힙의 사용가능 블록 리스트의 수<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_create_ext(heap_pt * heap_p, unsigned int addr, unsigned int size,
		int algorithm0, int locktype0, unsigned int m0, unsigned int fblcount0,
		int algorithm1, int locktype1, unsigned int m1, unsigned int fblcount1);

/*!
 * 힙을 제거하는 함수<br>
 *
 * @param	heap_p				제거할 힙의 주소가 저장된 포인터의 주소<br>
 * 								제거에 성공하면 *heap_p는 NULL이 됨<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_delete(heap_pt * heap_p);

/*!
 * 메모리 블록을 할당하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size				요청 크기<br>
 *
 * @return	할당된 메모리 블록의 주소<br>
 * 			<br>
 * 			NULL: 오류<br>
 */
void * heap_malloc(heap_pt heap, unsigned int size);

/*!
 * 정방향으로 메모리 블록을 할당하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size				요청 크기<br>
 *
 * @return	할당된 메모리 블록의 주소<br>
 * 			<br>
 * 			NULL: 오류<br>
 */
void * heap_mallocn(heap_pt heap, unsigned int size);

/*!
 * 역방향으로 메모리 블록을 할당하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size				요청 크기<br>
 *
 * @return	할당된 메모리 블록의 주소<br>
 * 			<br>
 * 			NULL: 오류<br>
 */
void * heap_mallocr(heap_pt heap, unsigned int size);

/*!
 * 메모리 블록을 해제하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	ptr					대상 메모리 블록의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_free(heap_pt heap, void * ptr);

/*!
 * 메모리 블록의 경계 영역이 오염되었는지 여부를 검사하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	ptr					대상 메모리 블록의 주소<br>
 *
 * @return	  0: 정상<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			HEAP_ERR__POLLUTED_TOP		: top boundary 영역이 오염되었음<br>
 * 			HEAP_ERR__POLLUTED_BOTTOM	: bottom boundary 영역이 오염되었음<br>
 * 			HEAP_ERR__POLLUTED_HEADER	: header boundary 영역이 오염되었음<br>
 *
 * 메모리 블럭 구조<br>
 * <pre>
 * \#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_BOUNDARY_CHECK == 1)
 *          -----------------------------------
 *          | header boundary                 |
 *          |        --------                 |
 *          |        tag                      |
 *          |        --------                 |
 *          |        ...                      |
 *          -----------------------------------
 *          | top boundary                    |
 *          -----------------------------------  <---- memory address
 *          | memory                          |
 *          |                                 |
 *          |                                 |
 *          -----------------------------------
 *          | tag                             |
 *          -----------------------------------
 *          | bottom boundary                 |
 *          -----------------------------------
 * \#else
 *          -----------------------------------
 *          | header tag                      |
 *          |        --------                 |
 *          |        ...                      |
 *          -----------------------------------  <---- memory address
 *          | memory                          |
 *          |                                 |
 *          |                                 |
 *          -----------------------------------
 *          | tag                             |
 *          -----------------------------------
 * \#endif
 * </pre>
 */
int heap_checkblockboundary(heap_pt heap, void * ptr);

/*!
 * 힙의 모든 메모리 블록들의 경계 영역이 오염되었는지 여부를 검사하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @return	  0: 정상<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 *
 */
int heap_checkblockboundaryall(heap_pt heap);

/*!
 * 메모리 블록의 크기를 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	ptr					대상 메모리 블록의 주소<br>
 *
 * @param	size_p				크기를 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getblocksize(heap_pt heap, void * ptr, unsigned int * size_p);

/*!
 * 기본 힙에서 정방향으로 메모리 블록을 할당하는 함수<br>
 *
 * @param	size				요청 크기<br>
 *
 * @return	할당된 메모리 블록의 주소<br>
 * 			<br>
 * 			NULL: 오류<br>
 */
void * mallocn(size_t size);

/*!
 * 기본 힙에서 역방향으로 메모리 블록을 할당하는 함수<br>
 *
 * @param	size				요청 크기<br>
 *
 * @return	할당된 메모리 블록의 주소<br>
 * 			<br>
 * 			NULL: 오류<br>
 */
void * mallocr(size_t size);

/*!
 * 힙의 크기를 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				크기를 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getsize(heap_pt heap, unsigned int * size_p);

/*!
 * 힙의 메모리 블록들의 요청 크기의 총 합을 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				요청 크기의 총 합을 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getrequestedsize(heap_pt heap, unsigned int * size_p);

/*!
 * 힙의 메모리 블록들의 요청 크기 총 합을 돌려주는 함수의 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				요청 크기의 총 합을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	nsize_p				정방향으로 할당된 메모리 블록들의 요청 크기의 총 합을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rsize_p				역방향으로 할당된 메모리 블록들의 요청 크기의 총 합을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getrequestedsize_ext(heap_pt heap, unsigned int * size_p,
		unsigned int * nsize_p, unsigned int * rsize_p);

/*!
 * 힙의 메모리 블록들의 요청 크기의 총 합의 최고 기록을 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				요청 크기의 총 합의 최고 기록을 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getrequestedsizemax(heap_pt heap, unsigned int * size_p);

/*!
 * 힙의 메모리 블록들의 요청 크기의 총 합의 최고 기록을 돌려주는 함수의 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				요청 크기의 총 합의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	nsize_p				정방향으로 할당된 메모리 블록들의 요청 크기의 총 합의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rsize_p				역방향으로 할당된 메모리 블록들의 요청 크기의 총 합의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getrequestedsizemax_ext(heap_pt heap, unsigned int * size_p,
		unsigned int * nsize_p, unsigned int * rsize_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 수를 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	count_p				총 수를 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedcount(heap_pt heap, unsigned int * count_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 수를 돌려주는 함수의 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	count_p				총 수를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	ncount_p			정방향으로 할당된 메모리 블록들의  총 수를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rcount_p			역방향으로 할당된 메모리 블록들의  총 수를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedcount_ext(heap_pt heap, unsigned int * count_p,
		unsigned int * ncount_p, unsigned int * rcount_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 수의 최고 기록을 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	count_p				총 수의 최고 기록을 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedcountmax(heap_pt heap, unsigned int * count_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 수의 최고 기록을 돌려주는 함수의 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	count_p				총 수의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	ncount_p			정방향으로 할당된 메모리 블록들의 총 수의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rcount_p			역방향으로 할당된 메모리 블록들의 총 수의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedcountmax_ext(heap_pt heap, unsigned int * count_p,
		unsigned int * ncount_p, unsigned int * rcount_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 크기를 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				힙의 할당된 메모리 블록들의 총 크기를 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedsize(heap_pt heap, unsigned int * size_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 크기를 돌려주는 함수의 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				힙의 할당된 메모리 블록들의 총 크기를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	nsize_p				정방향으로 할당된 메모리 블록들의 총 크기를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rsize_p				역방향으로 할당된 메모리 블록들의 총 크기를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedsize_ext(heap_pt heap, unsigned int * size_p,
		unsigned int * nsize_p, unsigned int * rsize_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 크기의 최고 기록을 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				총 크기의 최고 기록을 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedsizemax(heap_pt heap, unsigned int * size_p);

/*!
 * 힙의 할당된 메모리 블록들의 총 크기의 최고 기록을 돌려주는 함수의 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	size_p				총 크기의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	nsize_p				정방향으로 할당된 메모리 블록들의 총 크기의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rsize_p				역방향으로 할당된 메모리 블록들의 총 크기의 최고 기록을 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocatedsizemax_ext(heap_pt heap, unsigned int * size_p,
		unsigned int * nsize_p, unsigned int * rsize_p);

/*!
 * 힙의 사용가능 블록의 수를 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	count_p				총 사용가능 블록의 수를 저장할 변수의 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getfreeblockcount(heap_pt heap, unsigned int * count_p);

/*!
 * 힙의 사용가능 블록의 수를 돌려주는 확장형<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	count_p				총 사용가능 블록의 수를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	ncount_p			정방향 영역의 사용가능 블록의 수를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @param	rcount_p			역방향 영역의 사용가능 블록의 수를 저장할 변수의 주소<br>
 * 								NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getfreeblockcount_ext(heap_pt heap, unsigned int * count_p,
		unsigned int * ncount_p, unsigned int * rcount_p);

/*!
 * 힙의 메모리 블록 당 부담(overhead)를 돌려주는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @param	overhead_p			메모리 블럭 당 부담(overhead)를 저장할 변수의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getblockoverhead(heap_pt heap, unsigned int * overhead_p);

/*!
 * 힙의 정보를 출력하는 함수<br>
 *
 * @param	heap				대상 힙<br>
 * 								NULL이면 기본 힙<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_printheapinfo(heap_pt heap);

/*!
 * 그룹 시스템의 사용 가능 블록 리스트 수를 개산하는 함수<br>
 *
 * @param	size				최대 멤버 수<br>
 *
 * @param	m					최대 멤버 수<br>
 *
 * @return	사용 가능 블록 리스트 수<br>
 * 			<br>
 * 			  0: 오류<br>
 */
unsigned int heap_group_calc_fblcount(unsigned int size, unsigned int m);

/*!
 * 그룹 시스템의 사용 가능 블록 리스트 수를 개산하는 함수 (최소 크기 offset을 반영하지 않고 계산)<br>
 *
 * @param	size				최대 멤버 수<br>
 *
 * @param	m					최대 멤버 수<br>
 *
 * @return	사용 가능 블록 리스트 수<br>
 * 			<br>
 * 			  0: 오류<br>
 */
unsigned int heap_group_calc_fblcount_raw(unsigned int size, unsigned int m);

/*!
 * 가중치 버디 시스템의 사용 가능 블록 리스트 수를 개산하는 함수<br>
 *
 * @param	size				최대 멤버 수<br>
 *
 * @param	m					의미 없음<br>
 *
 * @return	사용 가능 블록 리스트 수<br>
 * 			<br>
 * 			  0: 오류<br>
 */
unsigned int heap_wbuddy_calc_fblcount(unsigned int size, unsigned int m);

/*!
 * 가중치 버디 시스템의 사용 가능 블록 리스트 수를 개산하는 함수 (최소 크기 offset을 반영하지 않고 계산)<br>
 *
 * @param	size				최대 멤버 수<br>
 *
 * @param	m					의미 없음<br>
 *
 * @return	사용 가능 블록 리스트 수<br>
 * 			<br>
 * 			  0: 오류<br>
 */
unsigned int heap_wbuddy_calc_fblcount_raw(unsigned int size, unsigned int m);

/*!
 * 테일링을 하지 않는 순수 그룹 시스템의 사용 가능 블록 리스트 수를 개산하는 함수<br>
 *
 * @param	size				최대 멤버 수<br>
 *
 * @param	m					최대 멤버 수<br>
 *
 * @return	사용 가능 블록 리스트 수<br>
 * 			<br>
 * 			  0: 오류<br>
 */
unsigned int heap_pgroup_calc_fblcount(unsigned int size, unsigned int m);

/*!
 * 테일링을 하지 않는 순수 그룹 시스템의 사용 가능 블록 리스트 수를 개산하는 함수 (최소 크기 offset을 반영하지 않고 계산)<br>
 *
 * @param	size				최대 멤버 수<br>
 *
 * @param	m					최대 멤버 수<br>
 *
 * @return	사용 가능 블록 리스트 수<br>
 * 			<br>
 * 			  0: 오류<br>
 */
unsigned int heap_pgroup_calc_fblcount_raw(unsigned int size, unsigned int m);

#ifdef	__cplusplus
}
#endif

#endif /* UBICLIB_HEAP_H_ */
