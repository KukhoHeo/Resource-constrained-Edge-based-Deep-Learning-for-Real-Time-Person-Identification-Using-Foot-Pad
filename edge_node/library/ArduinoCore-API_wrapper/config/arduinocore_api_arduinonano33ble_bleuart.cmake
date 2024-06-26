#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "arduinocore_api", "build_type": "cmake_ubinos"}

set_cache(PROJECT_TOOLCHAIN_C_STD "GNU11" STRING)
set_cache(PROJECT_TOOLCHAIN_CXX_STD "GNU++14" STRING)
set_cache(PROJECT_TOOLCHAIN_CXX_W_NO_CXX14_COMPAT FALSE BOOL)

set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_ORIGIN 0x20004400 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_LENGTH 0x0003BC00 STRING)

set_cache(UBINOS__UBIK__TICK_TYPE "RTC" STRING)
set_cache(UBINOS__UBIK__TICK_PER_SEC 1024 STRING)

set_cache(UBINOS__BSP__USE_DTTY TRUE BOOL)
set_cache(UBINOS__BSP__DTTY_TYPE "EXTERNAL" STRING)
set_cache(NRF5SDK__DTTY_NRF_BLE_UART_ENABLE TRUE BOOL)

set_cache(NRF5SDK__SWI_DISABLE0 TRUE BOOL)
set_cache(NRF5SDK__BLE_STACK_SUPPORT_REQD TRUE BOOL)

set_cache(NRF5SDK__TWI_ENABLED TRUE BOOL)

include(${PROJECT_UBINOS_DIR}/config/ubinos_arduinonano33ble_softdevice.cmake)
include(${PROJECT_LIBRARY_DIR}/seggerrtt_wrapper/config/seggerrtt.cmake)
include(${PROJECT_LIBRARY_DIR}/nrf5sdk_wrapper/config/nrf5sdk.cmake)
include(${PROJECT_LIBRARY_DIR}/nrf5sdk_extension/config/nrf5sdk_extension.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/arduinocore_api.cmake)

####

string(TOLOWER ${UBINOS__BSP__NRF52_SOFTDEVICE_NAME} _temp_softdevice_name)
string(TOLOWER ${UBINOS__BSP__BOARD_VARIATION_NAME} _tmp_board_model)

get_filename_component(_tmp_source_dir "${PROJECT_LIBRARY_DIR}/ArduinoCore-API_wrapper/source/arduinocore_api_ubinos/arch/arm/cortexm/${_tmp_board_model}" ABSOLUTE)

include_directories(${_tmp_source_dir}/${_temp_softdevice_name}/config)

add_definitions("-DARDUINO_ARCH_NRF52")
add_definitions("-DNRF52_SERIES")
add_definitions("-DARDUINO_ARDUINO_NANO33BLE")

