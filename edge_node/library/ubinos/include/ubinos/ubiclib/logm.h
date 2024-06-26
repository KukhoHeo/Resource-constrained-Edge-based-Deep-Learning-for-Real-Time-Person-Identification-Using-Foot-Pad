/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBICLIB_LOGM_H_
#define UBICLIB_LOGM_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file	logm.h
 *
 * @brief	ubiclib (Ubinos C Library) 컴포넌트 로그 메세지 인터페이스
 *
 * ubiclib 컴포넌트가  제공하는 로그 메세지 인터페이스를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>


/*! 미분류 카테고리 */
#define LOGM_CATEGORY__NONE			0

/*! 유비노스 카테고리 */
#define LOGM_CATEGORY__UBINOS		1
/*! BSP 카테고리 */
#define LOGM_CATEGORY__BSP			2
/*! 유비노스 커널 카테고리 */
#define LOGM_CATEGORY__UBIK			3
/*! 테스크 카테고리 */
#define LOGM_CATEGORY__TASK			4
/*! 세마포어 카테고리 */
#define LOGM_CATEGORY__SEM			5
/*! 메시지큐 카테고리 */
#define LOGM_CATEGORY__MSGQ			6
/*! 상태 변수 카테고리 */
#define LOGM_CATEGORY__CONDV		7
/*! 시그널 카테고리 */
#define LOGM_CATEGORY__SIGNAL		8
/*! 세마포어 타이머 카테고리 */
#define LOGM_CATEGORY__STIMER		9

/*! 유비노스 C 라이브러리 카테고리 */
#define LOGM_CATEGORY__UBICLIB		10
/*! 힙 카테고리 */
#define LOGM_CATEGORY__HEAP			11

/*! 유비노스 디바이스 드라이버 카테고리 */
#define LOGM_CATEGORY__UBIDRV		12
/*! 비휘발성 메모리 카테고리 */
#define LOGM_CATEGORY__NVMEM		13

/*! 확장을 위한 예약 카테고리 */
#define LOGM_CATEGORY__RESOLVED04	14
#define LOGM_CATEGORY__RESOLVED05	15
#define LOGM_CATEGORY__RESOLVED06	16
#define LOGM_CATEGORY__RESOLVED07	17
#define LOGM_CATEGORY__RESOLVED08	18
#define LOGM_CATEGORY__RESOLVED09	19

/*! 시스템 0 카테고리 */
#define LOGM_CATEGORY__SYS00		20
/*! 시스템 1 카테고리 */
#define LOGM_CATEGORY__SYS01		21
/*! 시스템 2 카테고리 */
#define LOGM_CATEGORY__SYS02		22
/*! 시스템 3 카테고리 */
#define LOGM_CATEGORY__SYS03		23
/*! 시스템 4 카테고리 */
#define LOGM_CATEGORY__SYS04		24
/*! 시스템 5 카테고리 */
#define LOGM_CATEGORY__SYS05		25
/*! 시스템 6 카테고리 */
#define LOGM_CATEGORY__SYS06		26
/*! 시스템 7 카테고리 */
#define LOGM_CATEGORY__SYS07		27
/*! 시스템 8 카테고리 */
#define LOGM_CATEGORY__SYS08		28
/*! 시스템 9 카테고리 */
#define LOGM_CATEGORY__SYS09		29

/*! 사용자 0 카테고리 */
#define LOGM_CATEGORY__USER00		30
/*! 사용자 1 카테고리 */
#define LOGM_CATEGORY__USER01		31
/*! 사용자 2 카테고리 */
#define LOGM_CATEGORY__USER02		32
/*! 사용자 3 카테고리 */
#define LOGM_CATEGORY__USER03		33
/*! 사용자 4 카테고리 */
#define LOGM_CATEGORY__USER04		34
/*! 사용자 5 카테고리 */
#define LOGM_CATEGORY__USER05		35
/*! 사용자 6 카테고리 */
#define LOGM_CATEGORY__USER06		36
/*! 사용자 7 카테고리 */
#define LOGM_CATEGORY__USER07		37
/*! 사용자 8 카테고리 */
#define LOGM_CATEGORY__USER08		38
/*! 사용자 9 카테고리 */
#define LOGM_CATEGORY__USER09		39

/*! 카테고리 끝 */
#define LOGM_CATEGORY__END			40


/*! 어떤 메시지도 출력하지 않음 */
#define LOGM_LEVEL__SILENT			0
/*! 심각한 수준의 문제에 관한 메시지까지만 출력함 */
#define LOGM_LEVEL__FATAL			1
/*! 일반적인 문제에 관한 메시지까지만 출력함 */
#define LOGM_LEVEL__ERROR			2
/*! 경고 메시지까지만 출력함 */
#define LOGM_LEVEL__WARNING			3
/*! 동작 상태 정보까지만 출력함 */
#define LOGM_LEVEL__INFO			4
/*! 디버깅시 필요한 정보까지 모두 출력함 */
#define LOGM_LEVEL__DEBUG			5
/*! 기타 상세 정보까지 모두 출력함 */
#define LOGM_LEVEL__VERBOSE			6
/*! 레벨 끝 */
#define LOGM_LEVEL__END				7


/*! 카테고리 기본 값 */
#define LOGM_CATEGORY				LOGM_CATEGORY__NONE
/*! 레벨 기본 값 */
#define LOGM_LEVEL					LOGM_LEVEL__WARNING
/*! 테그 기본 값 */
#define LOGM_TAG					NULL


/*! 출력되는 틱 카운트 최소 길이 */
#define LOGM_TICK_COUNT_LENGTH		10

/*! 출력되는 테그 최소 길이 */
#define LOGM_TAG_LENGTH				20

/*! 출력되는 함수명의 최소 길이 */
#define LOGM_FUNCTION_NAME_LENGTH	40

/*! 출력되는 줄 번호의 최소 길이 */
#define LOGM_LINE_NUMBER_LENGTH		6


#if !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1)


/*!
 * 기록할 메시지 수준을 설정하는 함수
 *
 * @param	category 메시지 분류 번호 (-1은 모든 분류)
 *
 * @param	level    기록할 메시지 수준 (설정한 수준 이상(같거나 작은 값)의 메시지만 기록됨)
 *
 * @return	설정한 기록할 메시지 수준<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int logm_setlevel(int category, int level);

/*!
 * 설정되어 있는 기록할 메시지 수준을 돌려주는 함수
 *
 * @param	category 메시지 분류 번호
 *
 * @return	설정되어 있는 기록할 메시지 수준<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int logm_getlevel(int category);

/*!
 * 메시지를 기록하는 함수
 *
 * @param category	메시지 분류 번호
 *
 * @param level		메시지 수준
 *
 * @param tag		메시지 테그
 *
 * @param func		메시지를 출력하는 함수 이름
 *
 * @param line		메시지를 출력하는 줄 수
 *
 * @param msg		메시지
 *
 * @return			기록한 메시지 길이<br>
 * 					<br>
 *                 -1: 오류<br>
 */
int logm_println(int category, int level, const char * tag, const char * func, int line, const char * msg);

/*!
 * 메시지를 기록하는 함수
 *
 * @param category	메시지 분류 번호
 *
 * @param level		메시지 수준
 *
 * @param tag		메시지 테그
 *
 * @param func		메시지를 출력하는 함수 이름
 *
 * @param line		메시지를 출력하는 줄 수
 *
 * @param format	메시지 형식
 *
 * @param ...		메시지에 포함될 데이터들
 *
 * @return			기록한 메시지 길이<br>
 * 					<br>
 *                 -1: 오류<br>
 */
int logm_printfln(int category, int level, const char * tag, const char * func, int line, const char * format, ...);


/*! 메시지를 기록하는 매크로 (LOGM_LEVEL로 정의된 기본 수준) */
#define logm (msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL,          LOGM_TAG, __FUNCTION__, __LINE__, msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__FATAL 수준) */
#define logmt(msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL__FATAL,   LOGM_TAG, __FUNCTION__, __LINE__, msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__ERROR 수준) */
#define logme(msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL__ERROR,   LOGM_TAG, __FUNCTION__, __LINE__, msg)
/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__WARNING 수준) */

#define logmw(msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL__WARNING, LOGM_TAG, __FUNCTION__, __LINE__, msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__INFO 수준) */
#define logmi(msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL__INFO,    LOGM_TAG, __FUNCTION__, __LINE__, msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__DEBUG 수준) */
#define logmd(msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL__DEBUG,   LOGM_TAG, __FUNCTION__, __LINE__, msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__VERBOSE 수준) */
#define logmv(msg) logm_println(LOGM_CATEGORY, LOGM_LEVEL__VERBOSE, LOGM_TAG, __FUNCTION__, __LINE__, msg)


/*! 메시지를 기록하는 매크로 (LOGM_LEVEL로 정의된 기본 수준) */
#define logmf (format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL,            LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__FATAL 수준) */
#define logmft(format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL__FATAL,     LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__ERROR 수준) */
#define logmfe(format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL__ERROR,     LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__WARNING 수준) */
#define logmfw(format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL__WARNING,   LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__INFO 수준) */
#define logmfi(format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL__INFO,      LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__DEBUG 수준) */
#define logmfd(format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL__DEBUG,     LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__VERBOSE 수준) */
#define logmfv(format, args...) logm_printfln(LOGM_CATEGORY, LOGM_LEVEL__VERBOSE,   LOGM_TAG, __FUNCTION__, __LINE__, format, ## args)


#else /* !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1) */


/*!
 * 기록할 메시지 수준을 설정하는 함수
 *
 * @param	category 메시지 분류 번호 (-1은 모든 분류)
 *
 * @param	level    기록할 메시지 수준 (설정한 수준 이상(같거나 작은 값)의 메시지만 기록됨)
 *
 * @return	설정한 기록할 메시지 수준<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
#define logm_setlevel(category, level) ((void) (LOGM_LEVEL__SILENT))

/*!
 * 설정되어 있는 기록할 메시지 수준을 돌려주는 함수
 *
 * @param	category 메시지 분류 번호
 *
 * @return	설정되어 있는 기록할 메시지 수준<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
#define logm_getlevel(category) (LOGM_LEVEL__SILENT)


/*! 메시지를 기록하는 매크로 (LOGM_LEVEL 설정 수준) */
#define logm (msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__FATAL 수준) */
#define logmt(msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__ERROR 수준) */
#define logme(msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__WARNING 수준) */
#define logmw(msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__INFO 수준) */
#define logmi(msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__DEBUG 수준) */
#define logmd(msg)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__VERBOSE 수준) */
#define logmv(msg)


/*! 메시지를 기록하는 매크로 (LOGM_LEVEL 설정 수준) */
#define logmf (format, args...)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__FATAL 수준) */
#define logmft(format, args...)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__ERROR 수준) */
#define logmfe(format, args...)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__WARNING 수준) */
#define logmfw(format, args...)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__INFO 수준) */
#define logmfi(format, args...)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__DEBUG 수준) */
#define logmfd(format, args...)

/*! 메시지를 기록하는 매크로 (LOGM_LEVEL__VERBOSE 수준) */
#define logmfv(format, args...)


#endif /* !(UBINOS__UBICLIB__EXCLUDE_LOGM == 1) */

#ifdef	__cplusplus
}
#endif

#endif /* UBICLIB_LOGM_H_ */
