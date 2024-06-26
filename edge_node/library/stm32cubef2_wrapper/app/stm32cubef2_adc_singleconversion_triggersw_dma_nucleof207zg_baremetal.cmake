#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "stm32cubef2_adc_singleconversion_triggersw_dma", "build_type": "cmake_ubinos", "app": true}

set_cache(UBINOS__UBICLIB__USE_STDIO_RETARGETING FALSE BOOL)
set_cache(UBINOS__BSP__USE_DTTY FALSE BOOL)

set_cache(STM32CUBEF2__USE_HAL_DRIVER FALSE BOOL)
set_cache(STM32CUBEF2__USE_FULL_LL_DRIVER TRUE BOOL)

include(${PROJECT_UBINOS_DIR}/config/ubinos_nucleof207zg_baremetal.cmake)
include(${PROJECT_LIBRARY_DIR}/stm32cubef2_wrapper/config/stm32cubef2.cmake)
include(${PROJECT_LIBRARY_DIR}/stm32cubef2_extension/config/stm32cubef2_extension.cmake)

####

set(INCLUDE__APP TRUE)
set(APP__NAME "stm32cubef2_adc_singleconversion_triggersw_dma")

get_filename_component(_tmp_source_dir "${STM32CUBEF2__BASE_DIR}/Projects/NUCLEO-F207ZG/Examples_LL/ADC/ADC_SingleConversion_TriggerSW_DMA/" ABSOLUTE)

include_directories(${_tmp_source_dir}/Inc)

set(PROJECT_APP_SOURCES ${PROJECT_APP_SOURCES} ${_tmp_source_dir}/Src/main.c)
set(PROJECT_APP_SOURCES ${PROJECT_APP_SOURCES} ${_tmp_source_dir}/Src/stm32f2xx_it.c)

