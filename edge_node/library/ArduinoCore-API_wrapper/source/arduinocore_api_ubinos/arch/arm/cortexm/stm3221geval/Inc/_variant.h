/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <ubinos.h>

#include <ubinos/bsp/arch.h>

#include "../../stm32xxxx/_variant_stm32xxxx.h"

#define TIMER_TONE                  TIM3
#define TIMER_TONE_CLOCK            (120000000 / 2)
#define TIMER_TONE_IRQn             TIM3_IRQn
#define TIMER_TONE_IRQHandler       TIM3_IRQHandler
#define TIMER_TONE_CHANNEL          TIM_CHANNEL_1
#define TIMER_TONE_CHANNEL_IT       TIM_IT_CC1
#define TIMER_TONE_COUNT_MAX        0xFFFF
#define TIMER_TONE_BASE_CLOCK       1000000

#endif /* _VARIANT_H_ */

