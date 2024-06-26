/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBIK_TEST_H_
#define UBIK_TEST_H_

/*!
 * @file ubik_test.h
 *
 * @brief ubik test API
 *
 * ubik test API를 정의합니다.
 */

#include <ubinos.h>

///////////////////////////////////////////////////////////////////////////////

/*! 시험에 사용하는 signal type */
#define UBINOS__UBIK_TEST__SIGTYPE	0x01

///////////////////////////////////////////////////////////////////////////////

extern volatile unsigned int _g_ubik_test_result; //!< 시험 결과를 저장하는 전역 변수

extern volatile unsigned int _g_ubik_test_count1; //!< 태스크 별 시험 진행 상황을 저장하는 전역 변수 1
extern volatile unsigned int _g_ubik_test_count2; //!< 태스크 별 시험 진행 상황을 저장하는 전역 변수 2
extern volatile unsigned int _g_ubik_test_count3; //!< 태스크 별 시험 진행 상황을 저장하는 전역 변수 3
extern volatile unsigned int _g_ubik_test_count4; //!< 태스크 별 시험 진행 상황을 저장하는 전역 변수 4

extern mutex_pt _g_ubik_test_mutex; //!< 시험 용 뮤택스
extern mutex_pt _g_ubik_test_mutex2; //!< 시험 용 뮤택스 2
extern signal_pt _g_ubik_test_signal; //!< 시험 용 시그널
extern condv_pt _g_ubik_test_condv; //!< 시험 용 상태변수
extern sem_pt _g_ubik_test_sem; //!< 시험 용 세마포어
extern msgq_pt _g_ubik_test_msgq; //!< 시험 용 메시지큐
extern stimer_pt _g_ubik_test_stimer; //!< 시험 용 세마포어 타이머
extern char * _g_ubik_test_msgbuf; //!< 시험 용 메시지 버퍼

extern task_pt _g_ubik_test_task_a[4]; //!< 시험 용 태스크 배열

///////////////////////////////////////////////////////////////////////////////

int ubik_test_testall(void); //!< Test all tests

int ubik_test_misctest01(void); //!< This function prints system information
int ubik_test_misctest02(void); //!< Test for accuracy of UBINOS__BSP__BUSYWAITCOUNT_PER_MS
int ubik_test_misctest03(void); //!< Test for accuracy of task_sleepms

int ubik_test_tasktest00(void); //!< Simple task example
int ubik_test_tasktest01(void); //!< Test on basic functions of task

int ubik_test_semtest00(void); //!< Simple semaphore example
int ubik_test_semtest01(void); //!< Test on basic functions of semaphore
int ubik_test_semtest02(void); //!< Test for priority inversion situation by semaphore
int ubik_test_semtest03(void); //!< Test on priority inheritance feature of semaphore preventing priority inversion situation
int ubik_test_semtest04(void); //!< Test for memory leak by semaphore

int ubik_test_mutextest00(void); //!< Simple mutex example
int ubik_test_mutextest01(void); //!< Test on basic functions of mutex
int ubik_test_mutextest02(void); //!< Test for priority inversion situation by mutex
int ubik_test_mutextest03(void); //!< Test on priority inheritance feature of mutex preventing priority inversion situation
int ubik_test_mutextest04(void); //!< Test for memory leak by mutex

int ubik_test_msgqtest00(void); //!< Simple message queue example
int ubik_test_msgqtest01(void); //!< Test on basic functions of message queue
int ubik_test_msgqtest02(void); //!< Test for priority inversion situation by message queue
int ubik_test_msgqtest03(void); //!< Test on priority inheritance feature of message queue preventing priority inversion situation
int ubik_test_msgqtest04(void); //!< Test for memory leak by message queue
int ubik_test_msgqtest05(void); //!< Misc tests

int ubik_test_stimertest00(void); //!< Simple semaphore timer example
int ubik_test_stimertest01(void); //!< Test on periodic notification function of semaphore timer
int ubik_test_stimertest02(void); //!< Test on oneshot notification function of semaphore timer
int ubik_test_stimertest03(void); //!< Test for memory leak by semaphore timer
int ubik_test_stimertest04(void); //!< Test on broadcasting notification function of semaphore timer

int ubik_test_tasktest02(void); //!< Test on sleep function of task
int ubik_test_tasktest03(void); //!< Test on suspend and resume function of task
int ubik_test_tasktest04(void); //!< Test on priority-based multi-tasking
int ubik_test_tasktest05(void); //!< Test on set priority function of task
int ubik_test_tasktest06(void); //!< Test for memory leak by task
int ubik_test_tasktest07(void); //!< Test on wait for multiple signal objects function of task
int ubik_test_tasktest08(void); //!< Test on get remaining timeout tick function of task
int ubik_test_tasktest09(void); //!< Test 2 on suspend and resume function of task

int ubik_test_signaltest00(void); //!< Simple signal example
int ubik_test_signaltest01(void); //!< Test on basic functions of signal
int ubik_test_signaltest02(void); //!< Test for priority inversion situation by signal
int ubik_test_signaltest03(void); //!< Test on priority inheritance feature of signal preventing priority inversion situation
int ubik_test_signaltest04(void); //!< Test for memory leak by signal

int ubik_test_condvtest00(void); //!< Simple condition variable example
int ubik_test_condvtest01(void); //!< Test on basic functions of condition variable
int ubik_test_condvtest02(void); //!< Test for priority inversion situation by condition variable
int ubik_test_condvtest03(void); //!< Test on priority inheritance feature of condition variable preventing priority inversion situation
int ubik_test_condvtest04(void); //!< Test for memory leak by condition variable



#endif /* UBIK_TEST_H_ */
