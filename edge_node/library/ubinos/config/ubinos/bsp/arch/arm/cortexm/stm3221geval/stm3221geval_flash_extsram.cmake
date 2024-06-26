#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__LINK_MEMMAP_RAM2_ORIGIN 0x64000000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM2_LENGTH 0x00200000 STRING)

set_cache(UBINOS__BSP__LINK_MEMMAP_TYPE "FLASH_EXTSRAM" STRING)
set_cache(UBINOS__BSP__USE_EXTSRAM TRUE BOOL)

set_cache(UBINOS__BSP__LINKSCRIPT_FILE "${PROJECT_UBINOS_DIR}/source/ubinos/bsp/arch/arm/cortexm/stm32f2x7/g/flash_extsram.ld" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/stm3221geval.cmake)

