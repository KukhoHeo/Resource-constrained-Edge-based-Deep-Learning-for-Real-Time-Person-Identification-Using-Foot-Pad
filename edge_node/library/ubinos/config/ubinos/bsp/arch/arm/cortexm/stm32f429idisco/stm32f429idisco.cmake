#
# Copyright (c) 2020 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__CPU_MODEL "STM32F429ZI" STRING)
set_cache(UBINOS__BSP__BOARD_MODEL "STM32F429IDISCO" STRING)
set_cache(UBINOS__BSP__STM32_HSE_VALUE "8000000U" STRING)

set_cache(UBINOS__BSP__OPENOCD_CONFIG_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32f429idisco/openocd.cfg" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/../stm32429ieval/stm32429ieval.cmake)

