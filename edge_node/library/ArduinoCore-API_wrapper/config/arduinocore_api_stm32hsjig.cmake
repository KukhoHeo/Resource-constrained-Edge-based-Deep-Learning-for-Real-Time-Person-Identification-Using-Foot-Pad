#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "arduinocore_api", "build_type": "cmake_ubinos"}

set_cache(PROJECT_TOOLCHAIN_C_STD "GNU11" STRING)
set_cache(PROJECT_TOOLCHAIN_CXX_STD "GNU++14" STRING)
set_cache(PROJECT_TOOLCHAIN_CXX_W_NO_CXX14_COMPAT FALSE BOOL)

set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_ORIGIN 0x08000000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_LENGTH 0x000C0000 STRING)

set_cache(UBINOS__UBIK__TICK_TYPE "RTC" STRING)

set_cache(UBINOS__BSP__DTTY_TYPE "EXTERNAL" STRING)
set_cache(SEGGERRTT__DTTY_ENABLE TRUE BOOL)

set_cache(UBINOS__BSP__BOARD_VARIATION_NAME "STM32HSJIG" STRING)

set_cache(UBINOS__BSP__STM32_RCC_HSE_CONFIG "ON" STRING)
set_cache(UBINOS__BSP__STM32_HSE_VALUE "24000000U" STRING)

set_cache(UBINOS__BSP__DEBUG_SERVER_TYPE "JLINK" STRING)
set_cache(UBINOS__BSP__DEBUG_SERVER_COMMAND "JLinkGDBServer" STRING)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_flash_load.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/gdb_flash_reset.gdb" PATH)

include(${PROJECT_UBINOS_DIR}/config/ubinos_nucleof207zg.cmake)
include(${PROJECT_LIBRARY_DIR}/seggerrtt_wrapper/config/seggerrtt.cmake)
include(${PROJECT_LIBRARY_DIR}/stm32cubef2_wrapper/config/stm32cubef2.cmake)
include(${PROJECT_LIBRARY_DIR}/stm32cubef2_extension/config/stm32cubef2_extension.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/arduinocore_api.cmake)

####

string(TOLOWER ${UBINOS__BSP__BOARD_VARIATION_NAME} _tmp_board_model)

get_filename_component(_tmp_source_dir "${PROJECT_LIBRARY_DIR}/ArduinoCore-API_wrapper/source/arduinocore_api_ubinos/arch/arm/cortexm/${_tmp_board_model}" ABSOLUTE)

include_directories(${_tmp_source_dir}/Inc)

