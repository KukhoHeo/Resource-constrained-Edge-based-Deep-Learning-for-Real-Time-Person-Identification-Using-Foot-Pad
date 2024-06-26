#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__CPU_MODEL "STM32F207ZG" STRING)

set_cache(UBINOS__BSP__BOARD_MODEL "NUCLEOF207ZG" STRING)

set_cache(UBINOS__BSP__STM32_HSE_VALUE "8000000U" STRING)

set_cache(UBINOS__BSP__OPENOCD_CONFIG_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleof207zg/openocd.cfg" PATH)

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleof207zg/t32_flash_load.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleof207zg/t32_flash_reset.cmm" PATH)

set_cache(UBINOS__BSP__STM32_RCC_HSE_CONFIG "BYPASS" STRING)

include(${CMAKE_CURRENT_LIST_DIR}/../stm3221geval.cmake)

