#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__LINK_MEMMAP_TYPE "SRAM" STRING)

set_cache(UBINOS__BSP__USE_ICACHE FALSE BOOL)

set_cache(UBINOS__BSP__BUSYWAITCOUNT_PER_MS 49110 STRING)

set_cache(UBINOS__BSP__LINKSCRIPT_FILE "${PROJECT_UBINOS_DIR}/source/ubinos/bsp/arch/arm/classic/sam9xe/512/sram.ld" PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/classic/sam9xexxxek/gdb_sram_load.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/classic/sam9xexxxek/gdb_sram_reset.gdb" PATH)

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/classic/sam9xexxxek/t32_sram_load.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/classic/sam9xexxxek/t32_sram_reset.cmm" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/sam9xe512ek.cmake)

