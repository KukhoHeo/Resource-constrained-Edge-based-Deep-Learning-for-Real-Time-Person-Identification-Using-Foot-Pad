/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_OBJTYPE_H_
#define UBINOS_OBJTYPE_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file objtype.h
 *
 * @brief ubinos object type
 *
 * ubinos 객체 형을 정의합니다.
 */

#include <ubinos_config.h>

/*! NULL 객체 형 번호 */
#define OBJTYPE__NULL				0x00

/*! ubik 태스크 객체 형 번호 */
#define OBJTYPE__UBIK_TASK			0x10
/*! ubik 아이들 태스크 객체 형 번호 */
#define OBJTYPE__UBIK_IDLETASK		0x18

/*! ubik 시그널 객체 형 번호 */
#define OBJTYPE__UBIK_SIGNAL		0x11
/*! ubik 뮤텍스 객체 형 번호 */
#define OBJTYPE__UBIK_MUTEX			0x12
/*! ubik 세마포어 객체 형 번호 */
#define OBJTYPE__UBIK_SEM			0x13
/*! ubik 큐 객체 형 번호 */
#define OBJTYPE__UBIK_MSGQ			0x14
/*! ubik 컨디션 변수 객체 형 번호 */
#define OBJTYPE__UBIK_CONDV			0x15
/*! ubik 세마포어 타이머 객체 형 번호 */
#define OBJTYPE__UBIK_STIMER		0x16

/*! 힙 객체 형 번호 */
#define OBJTYPE__UBICLIB_HEAP		0x30

/*! 플래시 객체 형 번호 */
#define OBJTYPE__FLASH				0x50

/*! 객체 형 정의 */
typedef struct _obj_t
{
	unsigned int type :8; /*!< Object type */
	unsigned int valid :1; /*!< Indicates whether it is valid */
	unsigned int reserved :7; /*!< Reserved */
	unsigned int reserved2 :16; /*!< Reserved 2 */
} obj_t;

/*! 객체 포인터 형 정의 */
typedef obj_t * obj_pt;

#ifdef	__cplusplus
}
#endif

#endif /* UBINOS_OBJTYPE_H_ */
