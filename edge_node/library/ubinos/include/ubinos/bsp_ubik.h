/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_BSP_UBIK_H_
#define UBINOS_BSP_UBIK_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file bsp.h
 *
 * @brief BSP UBIK (Board Support Package for Kernel) API
 *
 * BSP 에서 사용하는 Kernel 인터페이스를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

void bsp_ubik_entercrit(void);
void bsp_ubik_exitcrit(void);

typedef void * bsp_mutex_pt;
int bsp_mutex_create(bsp_mutex_pt * bsp_mutex_p);
int bsp_mutex_delete(bsp_mutex_pt * bsp_mutex_p);
int bsp_mutex_lock(bsp_mutex_pt bsp_mutex);
int bsp_mutex_lock_timed(bsp_mutex_pt bsp_mutex, unsigned int tick);
int bsp_mutex_unlock(bsp_mutex_pt bsp_mutex);

void * bsp_task_getcur();
int bsp_task_sleepms(unsigned int timems);

int bsp_ubik_isrt(void);
int bsp_ubik_iscrit(void);

void bsp_ubik_task_schedule_irq();

void bsp_ubik_swisr(unsigned int swino);

#if (UBINOS__BSP__CORTEX_MX == 1)

void __attribute__((naked)) bsp_ubik_psv_handler(void);
void __attribute__((naked)) bsp_ubik_irq_handler(void);

void bsp_ubik_swi_handler(void);
void bsp_ubik_tick_handler(void);
void bsp_ubik_irq_handler_c(unsigned int exceptiontype, unsigned int * prev_sp);

#endif /* (UBINOS__BSP__CORTEX_MX == 1) */

#ifdef	__cplusplus
}
#endif

#endif /* UBINOS_BSP_UBIK_H_ */
