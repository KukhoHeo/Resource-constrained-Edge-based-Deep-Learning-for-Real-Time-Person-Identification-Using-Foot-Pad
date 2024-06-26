#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__CPU_TYPE "CORTEX_M3" STRING)
set_cache(UBINOS__BSP__CPU_ARMTHUMBSTATE "THUMB" STRING)
set_cache(UBINOS__BSP__CPU_MODEL "STM32F217IG" STRING)

set_cache(UBINOS__BSP__BOARD_MODEL "STM3221GEVAL" STRING)

set_cache(UBINOS__BSP__LINK_MEMMAP_TYPE "FLASH" STRING)

set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_ORIGIN 0x08000000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_LENGTH 0x00100000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_ORIGIN   0x20000000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_LENGTH   0x00020000 STRING)

set_cache(UBINOS__BSP__USE_ICACHE TRUE BOOL)
set_cache(UBINOS__BSP__USE_DCACHE TRUE BOOL)

set_cache(UBINOS__BSP__USE_RELOCATED_ISR_VECTOR TRUE BOOL)

set_cache(UBINOS__BSP__CLOCK_FREQ_MAIN 120000 STRING)
set_cache(UBINOS__BSP__CLOCK_FREQ_SLOW 32 STRING)
set_cache(UBINOS__BSP__BUSYWAITCOUNT_PER_MS 39877 STRING)

set_cache(UBINOS__BSP__STACK_SIZE 0x800 STRING)

set_cache(UBINOS__BSP__LINKSCRIPT_FILE "${PROJECT_UBINOS_DIR}/source/ubinos/bsp/arch/arm/cortexm/stm32f2x7/g/flash.ld" PATH)

set_cache(UBINOS__BSP__FLASH_WRITER_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm3221geval/stm32f200.bin" PATH)

set_cache(UBINOS__BSP__OPENOCD_CONFIG_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm3221geval/openocd.cfg" PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_flash_load_openocd.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_flash_reset_openocd.gdb" PATH)

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm3221geval/t32_flash_load.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm3221geval/t32_flash_reset.cmm" PATH)

set_cache(UBINOS__BSP__DEBUG_SERVER_TYPE "OPENOCD" STRING)
set_cache(UBINOS__BSP__DEBUG_SERVER_COMMAND "openocd" STRING)

# maximum size = 128 KiB
set_cache(UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM "GROUP" STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE "MUTEX" STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_M 8 STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT 194 STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE 32 STRING)

include(${CMAKE_CURRENT_LIST_DIR}/../../arm.cmake)

