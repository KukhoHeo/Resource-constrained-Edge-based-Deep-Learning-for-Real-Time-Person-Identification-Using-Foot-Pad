#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__BOARD_MODEL "STM32F769IDISCO" STRING)

set_cache(UBINOS__BSP__OPENOCD_CONFIG_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32f769idisco/openocd.cfg" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/../stm32f769ieval/stm32f769ieval_sram.cmake)

