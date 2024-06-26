/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_H_
#define UBINOS_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file ubinos.h
 *
 * @brief ubinos API
 *
 * ubinos API를 정의합니다.
 */

/*!
 * \mainpage ubinos API
 *
 * ubinos API 버전 03.00.00
 * <br>
 *
 *
 * \section intro_sec 유비노스 소개
 *
 * 유비노스는 내장형 컴퓨터를 위한 운영체제이다. 유비노스는 우선순위 기반 선점형 멀티태스킹과 세마포어, 뮤텍스, 메시지큐, 시그널, 조건변수condition variable 등의 풍부한 태스크간 통신 기능을 지원하면서도, 수K 램RAM과 수십K 롬ROM만을 가진 컴퓨터에서도 잘 동작한다. 또한 수행할 작업이 없는 기간 동안 스스로 전력을 차단함으로써 전력 소비량을 줄일 수 있다. 때문에 유비노스는 램RAM, 롬ROM, 전력 자원 제약이 매우 심한 초소형 초 저전력 내장형 시스템 개발에 유리하다. 또한 유비노스는 우선순위 기반 라운드 로빈 선점형 멀티태스킹을 지원하며 주요 기능들이 최악 수행 시간을 예측할 수 있도록 만들어져 있기 때문에 실시간 시스템 개발에도 사용될 수 있다.
 * <br>
 *
 *
 * \section task_sec 멀티태스킹 multi-tasking
 *
 * 유비노스는 태스크 여러 개를 동시에 수행할 수 있다. 유비노스의 태스크는 스레드 개념을 구현한 것이며, 동적으로 생성되고 제거될 수 있다. 스레드는 각각이 하나의 독립적인 수행 단위이며, 고유한 스택을 가지지만 그 외의 메모리 공간은 다른 스레드와 공유한다. 유비노스의 태스크는 우선순위를 가지며, 높은 우선순위를 가진 태스크가 낮은 우선순위를 가진 태스크 보다 먼저 수행된다. 같은 우선순위를 가진 태스크들은 동시에 수행된다. 실제로는 라운드 로빈 round robin 방식으로 순서를 정해 일정한 주기로 번갈아 수행되는 것이지만, 그 주기가 매우 짧기 때문에 동시에 수행되는 것과 유사하게 동작한다.
 *
 * <b>태스크 상태전이표 task state transition diagram</b>
 *
 * 다음 그림은 태스크의 상태 변화를 나타내는 상태전이표 state transition diagram 이다.
 *
 * \image html ubik_task_state_transition_diagram.png
 *
 * 태스크는 다음과 같은 상태가 될 수 있다.
 *
 *	* 준비ready 상태
 *		* 수행될 준비가 되었지만 수행될 순서가 되지 않은 상태
 *	* 수행running 상태
 *		* 수행될 순서가 되어 수행되고 있는 상태
 *	* 대기blocked 상태
 *		* 지정한 조건이 만족되기를 기다리는 상태
 *		* 조건은 지정한 시간이 지나감, 지정한 자원이 사용 가능해짐 등이 될 수 있음
 *	* 중지suspended 상태
 *		* 조건 없이 기다리는 상태
 *		* 다른 태스크가 재시작resume 시켜 주어야 이 상태에서 빠져 나올 수 있음
 *
 * 태스크의 최초 상태는 준비ready 상태이다. 준비ready 상태인 태스크는 수행될 순서가 되었을 때 수행running 상태가 된다. 수행running 상태인 태스크는 한 수행 주기 time slice 만큼의 시간 동안 수행되었을 때 또는 자신의 우선순위보다 높은 우선순위를 가진 태스크가 준비ready 상태가 되었을 때 수행을 중지하고 준비ready 상태가 된다. 또는 스스로 어떤 조건이 만족될 때까지 기다리기를 원할 때 대기blocked 상태가 된다. 기다리는 조건은 지정한 시간이 지나는 것(task_sleep 함수를 호출했을 경우 이것이 조건이 된다), 지정한 자원이 사용 가능해지는 것(sem_take, mutex_lock 함수 등을 호출했을 경우 이것이 조건이 된다) 등이 될 수 있다. 대기blocked 상태인 태스크는 지정한 조건이 만족되었을 때 준비ready 상태가 된다.
 *
 * 준비ready 상태, 수행running 상태, 대기blocked 상태인 태스크는 스스로 또는 다른 태스크가 중지suspend하기를 원할 때(task_suspend 함수를 호출 할 때) 중지suspended 상태가 된다. 중지suspended 상태인 태스크는 다른 태스크가 재시작resume 시켜 줄 때(task_resume 함수를 호출 할 때)만 중지suspended 상태에서 벗어날 수 있다. 대기blocked 상태에서 중지suspended 상태가 된 태스크는 지정한 조건이 만족되더라도 준비ready 상태가 되지 않는다. 그리고 만족되기를 기다리는 조건이 지정한 시간이 지나는 것일 경우, 중지suspended 상태에 머무른 시간은 지나간 시간으로 인정되지 않는다. 중지suspended 상태인 태스크는 재시작resume되면 중지suspended 상태가 되기 직전 상태로 돌아간다. 좀 더 정확히 말하자면, 직전 상태가 준비ready 상태와 수행 running 상태였던 태스크는 준비ready 상태가 된다. 그리고 직전 상태가 대기blocked 상태였던 태스크는 재시작resume된 시점에 지정한 조건이 만족될 경우 준비ready 상태가, 그렇지 않을 경우 대기blocked 상태가 된다.
 *
 *
 * <b>선점형 스케줄링 preemptive scheduling</b>
 *
 * 현재 태스크보다 높은 우선순위를 가진 태스크가 수행될 준비가 되었거나, 한 수행 주기 time slice 가 끝나서 같은 우선순위를 가진 다음 순서의 태스크를 수행할 시점이 되었을 때, 수행 태스크 전환은 유비노스 커널에 의해 자동적이고 강제적으로 이루어진다. 이러한 태스크 전환 방식은 선점형 방식이라 불린다.
 *
 * 다음 그림은 현재 태스크보다 높은 우선순위를 가진 태스크가 수행될 준비가 되었을 때 수행 태스크가 전환되는 과정을 보여준다.
 *
 * \image html ubik_task_preemption_diagram_01.png
 *
 * 더 높은 우선순위를 가진 태스크 1이 수행될 준비가 되는 시점 1과 2에 수행 태스크는 곧바로 태스크 1로 전환된다. 시점 3과 4에서의 수행 태스크 전환은 태스크 1이 스스로 필요한 이벤트를 기다리기 위해 수행 권한을 양보하기 때문에 발생한다.
 *
 * 다음 그림은 한 수행 주기 time slice 가 끝나서 같은 우선순위를 가진 다음 순서의 태스크를 수행할 시점이 되었을 때 수행 태스크가 전환되는 과정을 보여준다.
 *
 * \image html ubik_task_preemption_diagram_02.png
 *
 * 시스템 틱 하드웨어 타이머에 의해 한 수행 주기 time slice 가 끝났음을 알리는 인터럽트가 주기적으로 시점 1, 2, 3, 4에서 발생하며, 이 인터럽트를 처리하기 위한 핸들러에 의해 수행 태스크는 같은 우선순위를 가진 다음 순서의 태스크로 전환된다.
 *
 *	* <a href="task_8h.html">태스크 인터페이스</a>
 *
 *	* \subpage task_example
 * <br>
 *
 *
 * \section sem_sec 세마포어
 *
 * 세마포어를 사용하면 다양한 태스크 간 동기화 문제를 해결할 수 있다. 태스크간 동기화란 태스크들이 서로 일 수행 시점을 맞추는 것이다. 예를 들어, 다른 태스크가 어떤 일을 완료해야 자신의 일을 시작할 수 있는 태스크가 있을 수 있다. 이런 태스크는 다른 태스크가 그 어떤 일 수행을 완료할 때까지 기다리다가 완료하는 순간 자신의 일 수행을 시작해야 한다. 다시 말해 자신의 일 수행 시작 시점을 다른 태스크의 일 수행 완료 시점에 맞추어야 한다. 세마포어는 그런 태스크를 구현할 때 사용된다.
 *
 * 세마포어는 상호 배제적mutual exclusive 사용 구현에도 사용될 수 있다. 그러나 상호 배제적mutual exclusive 사용 구현에는 뮤텍스를 사용하는 것이 더 낫다. 상호 배제적mutual exclusive 사용과, 그것의 구현에 뮤텍스가 왜 더 적합한지는 뮤텍스 부분에서 자세히 설명한다.
 *
 * 세마포어는 엣져 위베 다익스트라Edsger Wybe Dijkstra가 제안한 특수 명령으로만 접근할 수 있는 보호된 변수이다. 형태는 부호 없는 정수 또는 한자리 이진수가 될 수 있다. 형태가 부호 없는 정수인 세마포어는 카운팅counting 세마포어로, 한자리 이진수인 세마포어는 이진binary 세마포어로 불린다. 세마포어는 원자적atomic 명령인 P 명령(sem_take 함수)과 V 명령(sem_give 함수)을 통한 접근만을 허용한다. 원자적atomic이란 수행 도중 중단될 수 없음을 의미한다. 이 명령들을 의사코드pseudocode로 표현하면 다음과 같다.
 *
 *     P (semaphore S) {
 *         wait until S > 0, then S = S - 1;
 *     }
 *
 *     V (semaphore S) {
 *        S = S + 1;
 *     }
 *
 * 위의 의사코드pseudocode는 운영체제의 도움 없이 구현할 수 있다. 그러나 바쁜 기다림busy waiting을 한다. 운영체제가 제공하는 기능을 활용하는 방식으로 다시 표현하면 아래와 같다. 아래의 의사코드pseudocode는 바쁜 기다림busy waiting을 하지 않는다.
 *
 *     P (semaphore S) {
 *         if S > 0, then
 *             S = S - 1;
 *         else
 *             insert current task into waiting task queue and sleep current task;
 *     }
 *
 *     V (semaphore S) {
 *         if waiting task queue is not empty, then
 *             get a task from waiting task queue and wake the task up ;
 *         else
 *             S = S + 1;
 *     }
 *
 *	* <a href="sem_8h.html">세마포어 인터페이스</a>
 *
 *	* \subpage sem_example
 * <br>
 *
 *
 * \section mutex_sec 뮤텍스
 *
 * 자원 중에는 태스크 여러 개가 동시에 사용할 경우 문제가 생기는 것이 있다. 그런 자원은 상호 배제적으로mutual exclusively 사용해야 한다. 상호 배제적mutual exclusive 사용이란 한 태스크가 어떤 자원을 다른 태스크들이 사용하는 것을 금지하고 사용하는 것을 의미한다. 뮤텍스를 사용하면 이 상호 배제적mutual exclusive 사용을 쉽게 구현할 수 있다. 뮤텍스를 하나 생성한 다음, 자원 사용을 시작하기 전에 그 뮤텍스에 잠그기 명령(mutex_lock 함수)을, 자원 사용을 끝마친 후에 그 뮤텍스에 잠금 해제 명령(mutex_unlock 함수)을 수행하기만 하면 된다.
 *
 * 뮤텍스는 상호 배제적mutual exclusive 사용 구현에 특화된 세마포어이다. 초기값이 1이며, 일반 세마포어에는 없는 특성을 두 가지 가진다. 물론 일반 세마포어로도 상호 배제적mutual exclusive 사용을 구현할 수 있다. 초기값이 1인 세마포어를 생성한 다음, 자원 사용을 시작하기 전에 그 세마포어에 P 명령(sem_take 함수)을, 자원 사용을 끝마친 후에 그 세마포어에 V 명령(sem_give 함수)을 수행하면 된다. 그러나 일반 세마포어는 가지지 않는, 뮤텍스만이 가지는 특성 두 가지 때문에 상호 배제적mutual exclusive 사용 구현에는 뮤텍스를 사용하는 것이 더 낫다. 그 특성들은 다음과 같다. (유비노스의 뮤텍스는 이 특성 두 가지를 모두 가진다. 그러나 다른 모든 운영체제의 뮤텍스가 그러한 것은 아니다.)
 *
 * 첫 번째 특성은 '뮤텍스는 자신을 잠근 태스크에 한해서 다시 잠그는 것을 허용한다'이다. 이를 '재귀적recursive 잠금을 허용한다'라고 표현하기도 한다. 다음과 같은 함수가 있을 수 있다. 이 함수는 어떤 자원을 상호 배제적으로mutual exclusively 사용해야 한다. 그리고 이 함수는 역시 그 어떤 자원을 상호 배제적으로mutual exclusively 사용해야 하는 또 다른 함수를 호출해야 한다. 만일 세마포어를 사용해 상호 배제적mutual exclusive 사용을 구현하면 이 함수를 호출하는 태스크는 교착deadlock 상태에 빠진다. 이 함수 시작 시점에서 한번, 이 함수가 호출하는 또 다른 함수 시작 시점에서 다시 한번 P 명령(sem_take 함수)을 수행하기 때문이다. 반면 뮤텍스를 사용해 구현하면 태스크가 교착deadlock 상태에 빠지는 것을 방지할 수 있다. 뮤텍스는 자신을 잠근 태스크에 한해서 다시 잠그는 것을 허용하기 때문이다.
 *
 * 두 번째 특성은 '뮤텍스 잠그기를 시도한 태스크의 우선순위가 뮤텍스를 잠금 해제할(뮤텍스를 잠근) 태스크의 우선순위 보다 높을 때, 뮤텍스를 잠금 해제할(뮤텍스를 잠근) 태스크가 그 높은 우선순위를 상속받는다'이다. 이것은 우선순위 상속priority inheritance 특성이라고 불린다. 이 우선순위 상속priority inheritance 특성은 상호 배제적mutual exclusive 사용 때문에 발생할 수 있는 우선순위 역전priority inversion 상황을 방지해준다. 우선순위 역전priority inversion 상황과, 우선순위 상속priority inheritance 특성이 어떻게 이 상황을 방지해주는지는 뮤텍스 예제 부분에서 자세히 설명한다.
 *
 *	* <a href="mutex_8h.html">뮤텍스 인터페이스</a>
 *
 *	* \subpage mutex_example
 * <br>
 *
 *
 * \section msgq_sec 메시지큐
 *
 * 태스크들은 메시지큐를 사용해 서로 메시지(작은 크기의 데이터)를 주고 받을 수 있다. 메시지큐는 메시지를 큐 방식으로 전달한다. 받은 순서대로 전달하며 전달하지 못한 데이터는 내부에 쌓아둔다.
 *
 * 메시지큐는 메시지 전달 기능이 추가된 세마포어로 볼 수 있다. 메시지를 전달할 수 있다는 점만 빼면 세마포어와 동일하다. 따라서 메시지를 전달함과 동시에 태스크 간 동기화 문제도 해결할 수 있다. 메시지큐의 메시지 수신 명령(msgq_receive 함수)과 송신 명령(msgq_send 함수)이 각각 세마포어의 P 명령(sem_take 함수)과 V 명령(sem_give 함수)의 역할을 대신할 수 있다.
 *
 *	* <a href="msgq_8h.html">메시지큐 인터페이스</a>
 *
 *	* \subpage msgq_example
 * <br>
 *
 *
 * \section stimer_sec 세마포어 타이머
 *
 * 세마포어 타이머를 사용하면 일정한 주기로 반복적으로 주어진 일을 처리하는 태스크를 쉽게 구현할 수 있다. 세마포어 타이머는 설정된 주기마다 지정한 세마포어에 V 명령(sem_give 함수)을 수행한다. 따라서 태스크를 그 지정한 세마포어에 동기되어 반복적으로 주어진 일을 처리하도록 구현하면, 그 태스크는 일정한 주기로 반복적으로 주어진 일을 처리하게 된다.
 *
 *	* <a href="stimer_8h.html">세마포어 타이머  인터페이스</a>
 *
 *	* \subpage stimer_example
 * <br>
 *
 *
 * \section ref_sec 참고 자료
 *
 *	* <a href="http://www.ubinos.org">유비노스 홈페이지</a>
 * <br>
 *
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>
#include <ubinos/bsp.h>
#include <ubinos/ubiclib.h>
#include <ubinos/ubik.h>

#ifdef	__cplusplus
}
#endif

#endif /* UBINOS_H_ */

