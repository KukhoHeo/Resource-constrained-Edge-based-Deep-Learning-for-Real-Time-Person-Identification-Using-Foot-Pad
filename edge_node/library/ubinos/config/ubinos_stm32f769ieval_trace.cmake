#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubinos", "build_type": "cmake_ubinos"}

set_cache(UBINOS__BSP__CLOCK_FREQ_MAIN 100000 STRING)
set_cache(UBINOS__BSP__BUSYWAITCOUNT_PER_MS 99660 STRING)

set_cache(UBINOS__BSP__STM32_ENABLE_TRACE TRUE BOOL)

set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32f769ieval/t32_flash_load_trace.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/stm32f769ieval/t32_flash_reset_trace.cmm" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/ubinos_stm32f769ieval.cmake)

