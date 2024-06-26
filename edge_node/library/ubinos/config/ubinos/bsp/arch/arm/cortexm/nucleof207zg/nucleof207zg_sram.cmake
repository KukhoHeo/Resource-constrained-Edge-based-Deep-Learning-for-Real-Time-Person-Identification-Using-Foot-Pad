#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__LINK_MEMMAP_TYPE "SRAM" STRING)

set_cache(UBINOS__BSP__USE_ICACHE FALSE BOOL)
set_cache(UBINOS__BSP__USE_DCACHE FALSE BOOL)

set_cache(UBINOS__BSP__BUSYWAITCOUNT_PER_MS 28000 STRING)

set_cache(UBINOS__BSP__LINKSCRIPT_FILE "${PROJECT_UBINOS_DIR}/source/ubinos/bsp/arch/arm/cortexm/stm32f2x7/g/sram.ld" PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_sram_load_openocd.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_sram_reset_openocd.gdb" PATH)

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleof207zg/t32_sram_load.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nucleof207zg/t32_sram_reset.cmm" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/nucleof207zg.cmake)

