#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__STM32CUBEF2 TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} stm32cubef2_wrapper)

set_cache_default(STM32CUBEF2__BASE_DIR "${PROJECT_LIBRARY_DIR}/stm32cubef2_v01.09.00_lite" STRING "stm32cubef2 project base dir")

set_cache_default(STM32CUBEF2__USE_HAL_DRIVER TRUE BOOL "Use STM32 Cube F2 HAL Driver")

set_cache_default(STM32CUBEF2__USE_HAL_WITH_UBINOS_TICK TRUE BOOL "Use STM32 Cube F2 HAL Driver with ubinos tick ")

set_cache_default(STM32CUBEF2__USE_FULL_LL_DRIVER FALSE BOOL "Use STM32 Cube F2 Full Low Level Driver")

set_cache_default(STM32CUBEF2__USE_FREERTOS FALSE BOOL "Use STM32 Cube F2 FreeRTOS")

set_cache_default(STM32CUBEF2__USE_LWIP FALSE BOOL "Use STM32 Cube F2 LwIP")

set_cache_default(STM32CUBEF2__USE_LWIP_ETHERNET FALSE BOOL "Use STM32 F2 LwIP Ethernet Interface")

set_cache_default(STM32CUBEF2__USE_LWIP_HTTP FALSE BOOL "Use STM32 Cube F2 HTTP Application Protocol")

set_cache_default(STM32CUBEF2__USE_LCD FALSE BOOL "Use STM32 Cube F2 LCD")

set_cache_default(STM32CUBEF2__USE_LCD_LOG FALSE BOOL "Use STM32 Cube F2 LCD Log")

set_cache_default(STM32CUBEF2__USE_SRAM FALSE BOOL "Use STM32 Cube F2 SRAM")

set_cache_default(STM32CUBEF2__USE_FATFS FALSE BOOL "Use STM32 Cube F2 FatFs")

####

set(_tmp_all_flags "")

if(STM32CUBEF2__USE_HAL_DRIVER)
    if(UBINOS__UBIK__TICK_TYPE STREQUAL "SYSTICK")
        message(FATAL_ERROR "Can not support STM32CUBEF2__USE_HAL_DRIVER with  SYSTICK UBINOS__UBIK__TICK_TYPE")
    endif()
    set(_tmp_all_flags "${_tmp_all_flags} -DUSE_HAL_DRIVER")
endif()

if(STM32CUBEF2__USE_FULL_LL_DRIVER)
    set(_tmp_all_flags "${_tmp_all_flags} -DUSE_FULL_LL_DRIVER")
endif()

set(CMAKE_ASM_FLAGS "${_tmp_all_flags} ${CMAKE_ASM_FLAGS}")
set(CMAKE_C_FLAGS   "${_tmp_all_flags} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${_tmp_all_flags} ${CMAKE_CXX_FLAGS}")

