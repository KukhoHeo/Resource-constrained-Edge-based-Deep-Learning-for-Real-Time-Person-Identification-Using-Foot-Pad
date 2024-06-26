/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBIDRV_UART_H_
#define UBIDRV_UART_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * @file uart.h
 *
 * @brief UART device driver API
 *
 * UART 장치 드라이버 API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

#define UBIDRV_UART_FILE_NAME_MAX           64

#define UBIDRV_UART_DATA_BITS_8             0
#define UBIDRV_UART_DATA_BITS_9             1

#define UBIDRV_UART_STOP_BITS_1             0
#define UBIDRV_UART_STOP_BITS_2             1

#define UBIDRV_UART_PARITY_TYPE_NONE        0
#define UBIDRV_UART_PARITY_TYPE_EVEN        1
#define UBIDRV_UART_PARITY_TYPE_ODD         2

#define UBIDRV_UART_HW_FLOW_CTRL_NONE       0
#define UBIDRV_UART_HW_FLOW_CTRL_RTS        1
#define UBIDRV_UART_HW_FLOW_CTRL_CTS        2
#define UBIDRV_UART_HW_FLOW_CTRL_RTS_CTS    3

/*! UART 장치 정의 */
typedef struct _ubidrv_uart_t
{
    char file_name[UBIDRV_UART_FILE_NAME_MAX];
    uint32_t baud_rate;
    unsigned int data_bits :2;
    unsigned int stop_bits :2;
    unsigned int parity_type :2;
    unsigned int hw_flow_ctl :2;

    int fd;
} ubidrv_uart_t;

/*!
 * UART 장치를 여는 함수
 *
 * @param   uart        대상 장치
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_open(ubidrv_uart_t * uart);

/*!
 * UART 장치를 닫는 함수
 *
 * @param   uart        대상 장치
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_close(ubidrv_uart_t * uart);

/*!
 * UART 장치에서 문자를 입력받는 함수 (입력이 들어올 때까지 기다림)
 *
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   ch_p        입력받은 문자를 저장할 변수의 주소
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_getc(int fd, char *ch_p);

/*!
 * UART 장치에서 문자를 입력받는 함수
 *
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   ch_p        입력받은 문자를 저장할 변수의 주소
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_getc_unblocked(int fd, char *ch_p);

/*!
 * UART 장치에서 문자를 출력하는 함수
 *
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   ch          출력할 문자
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_putc(int fd, int ch);

/*!
 * UART 장치 출력 버퍼에 저장된 내용을 모두 출력하는 함수
 *
 * @param   fd          대상 장치 파일 디스크립터
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_flush(int fd);

/*!
 * UART 장치에 문자열을 출력하는 함수 (NULL도 출력함)
 *
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   str         출력할 문자열이 저장된 버퍼
 * 
 * @param   len         출력할 문자열의 크기
 *
 * @return  오류 번호
 */
int ubidrv_uart_putn(int fd, const char *str, int len);

/*!
 * UART 장치 입력 버퍼에 입력받은 문자가 있는지를 검사하는 함수

 * @param   fd          대상 장치 파일 디스크립터
 *
 * @return     1: 있음<br>
 *              0: 없음<br>
 */
int ubidrv_uart_kbhit(int fd);

/*!
 * UART 장치에 문자열을 출력하는 함수
 *
 * "\0" 이전까지 또는 출력할 문자열의 최대 크기까지 출력한다.
 *
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   str         출력할 문자열이 저장된 버퍼
 * 
 * @param   max         출력할 문자열의 최대 크기
 *
 * @return  오류 번호
 */
int ubidrv_uart_puts(int fd, const char *str, int max);

/*!
 * UART 장치에에서 문자열을 입력받는 함수
 *
 * "\n", "\r", 또는 "\0" 이 입력되거나 최대 크기보다 작을 때까지 입력받는다.
 * 입력받은 "\n", "\r"은 퍼버에 저장되지 않는다.
 * 
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   str         입력받은 문자열을 저장할 버퍼
 * 
 * @param   max         입력받을 문자열의 최대 크기
 *
 * @return  오류 번호
 */
int ubidrv_uart_gets(int fd, char *str, int max);

/*!
 * UART 장치 에코 설정 함수
 * 
 * @param   fd          대상 장치 파일 디스크립터
 *
 * @param   echo        0: echo off<br>
 *                      1: echo on<br>
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_setecho(int fd, int echo);

/*!
 * 디버깅 터미널 에코 설정을 돌려주는 함수
 * 
 * @param   fd          대상 장치 파일 디스크립터
 *
 * @return  1: echo on<br>
 *          0: echo off<br>
 */
int ubidrv_uart_getecho(int fd);

/*!
 * UART 장치 자동 캐리지 리턴 설정 함수
 *
 * 디버깅 터미널에 "\n" 출력시 그 앞에 "\r"을 자동 출력하는 기능 활성화 여부를 설정한다.
 * 
 * @param   fd          대상 장치 파일 디스크립터
 * 
 * @param   autocr      0: auto carriage return off<br>
 *                      1: auto carriage return on<br>
 *
 * @return  오류 번호
 */
ubi_err_t ubidrv_uart_setautocr(int fd, int autocr);

/*!
 * 디버깅 터미널 자동 캐리지 리턴 설정을 돌려주는 함수
 * 
 * @param   fd          대상 장치 파일 디스크립터
 *
 * @return  0: auto carriage return off<br>
 *          1: auto carriage return on<br>
 */
int ubidrv_uart_getautocr(int fd);

#ifdef __cplusplus
}
#endif

#endif /* UBIDRV_UART_H_ */
