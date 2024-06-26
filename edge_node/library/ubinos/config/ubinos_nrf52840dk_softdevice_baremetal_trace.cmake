#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubinos", "build_type": "cmake_ubinos"}

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/t32_flash_load_softdevice_trace.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/t32_flash_reset_softdevice_trace.cmm" PATH)

set_cache(UBINOS__BSP__NRF52_ENABLE_TRACE TRUE BOOL)

set_cache(NRF5SDK__USE_ALT_TRACE_PIN TRUE BOOL)

include(${CMAKE_CURRENT_LIST_DIR}/ubinos_nrf52840dk_softdevice_baremetal.cmake)

