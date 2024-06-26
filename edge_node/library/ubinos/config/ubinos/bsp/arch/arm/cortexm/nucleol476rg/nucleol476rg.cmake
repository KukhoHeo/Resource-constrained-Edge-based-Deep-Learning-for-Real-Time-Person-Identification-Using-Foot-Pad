#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__CPU_MODEL "STM32L476RG" STRING)

set_cache(UBINOS__BSP__BOARD_MODEL "NUCLEOL476RG" STRING)

set_cache(UBINOS__BSP__OPENOCD_CONFIG_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleol476rg/openocd.cfg" PATH)

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleol476rg/t32_flash_load.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleol476rg/t32_flash_reset.cmm" PATH)

set_cache(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "2" STRING)

include(${CMAKE_CURRENT_LIST_DIR}/../stm32l476geval.cmake)

