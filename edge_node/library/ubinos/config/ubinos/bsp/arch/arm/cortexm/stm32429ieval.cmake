#
# Copyright (c) 2020 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__CPU_TYPE "CORTEX_M4" STRING)
set_cache(UBINOS__BSP__CPU_ARMTHUMBSTATE "THUMB" STRING)
set_cache(UBINOS__BSP__CPU_MODEL "STM32F429NI" STRING)

set_cache(UBINOS__BSP__BOARD_MODEL "STM32429IEVAL" STRING)

set_cache(UBINOS__BSP__LINK_MEMMAP_TYPE "FLASH" STRING)

set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_ORIGIN 0x08000000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_LENGTH 0x00200000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_ORIGIN   0x20000000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_LENGTH   0x00030000 STRING)

set_cache(UBINOS__BSP__USE_ICACHE TRUE BOOL)
set_cache(UBINOS__BSP__USE_DCACHE TRUE BOOL)

set_cache(UBINOS__BSP__USE_RELOCATED_ISR_VECTOR TRUE BOOL)

set_cache(UBINOS__BSP__CLOCK_FREQ_MAIN 168000 STRING)
set_cache(UBINOS__BSP__CLOCK_FREQ_SLOW 32 STRING)
set_cache(UBINOS__BSP__BUSYWAITCOUNT_PER_MS 59893 STRING)

set_cache(UBINOS__BSP__STACK_SIZE 0x800 STRING)

set_cache(UBINOS__BSP__LINKSCRIPT_FILE "${PROJECT_UBINOS_DIR}/source/ubinos/bsp/arch/arm/cortexm/stm32f429/i/flash.ld" PATH)

set_cache(UBINOS__BSP__OPENOCD_CONFIG_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32429ieval/openocd.cfg" PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_flash_load_openocd.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_flash_reset_openocd.gdb" PATH)

set_cache(UBINOS__BSP__FLASH_WRITER_FILE "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32429ieval/stm32f420.bin" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32429ieval/t32_flash_load.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32429ieval/t32_flash_reset.cmm" PATH)

set_cache(UBINOS__BSP__DEBUG_SERVER_TYPE "OPENOCD" STRING)
set_cache(UBINOS__BSP__DEBUG_SERVER_COMMAND "openocd" STRING)

# maximum size = 192 KiB
set_cache(UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM "GROUP" STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE "MUTEX" STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_M 8 STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT 210 STRING)
set_cache(UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE 32 STRING)

include(${CMAKE_CURRENT_LIST_DIR}/../../arm.cmake)

