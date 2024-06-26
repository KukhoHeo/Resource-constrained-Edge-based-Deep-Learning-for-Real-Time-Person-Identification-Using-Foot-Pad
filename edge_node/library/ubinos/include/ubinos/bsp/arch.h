/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_BSP_ARCH_H_
#define UBINOS_BSP_ARCH_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*!
 * @file arch.h
 *
 * @brief Architecture definition
 *
 * Architecture 관련 내용을 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>

#if   (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM7X256EK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM7X512EK)
#include "arch/arm/classic/sam7xxxxek.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM9XE512EK)
#include "arch/arm/classic/sam9xexxxek.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE)
#include "arch/arm/cortexm/nrf52dk.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOF207ZG)
#include "arch/arm/cortexm/stm3221geval.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L476GEVAL) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOL476RG) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32L475EIOT01)
#include "arch/arm/cortexm/stm32l476geval.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32429IEVAL) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32F429IDISCO)
#include "arch/arm/cortexm/stm32429ieval.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NETDUINOPLUS2)
#include "arch/arm/cortexm/netduinoplus2.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32F769IEVAL) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32F769IDISCO)
#include "arch/arm/cortexm/stm32f769ieval.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32H747IDISCO)
#include "arch/arm/cortexm/stm32h747idisco.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM32VLDISCO)
#include "arch/arm/cortexm/stm32vldisco.h"
#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__LOCAL)
#else
#error "Unsupported UBINOS__BSP__BOARD_MODEL"
#endif

#ifndef __ASSEMBLY__

extern volatile unsigned int _bsp_kernel_active; /* variable representing whether kernel is active or not */
extern volatile unsigned int _bsp_critcount; /* critical section nesting count */
extern volatile unsigned int _bsp_critcount_in_isr; /* critical section nesting count in interrupt service routine */
extern volatile unsigned int _bsp_aborted;  /* variable representing whether aborted or not */

#endif /* __ASSEMBLY__ */

#ifdef	__cplusplus
}
#endif

#endif /* UBINOS_BSP_ARCH_H_ */
