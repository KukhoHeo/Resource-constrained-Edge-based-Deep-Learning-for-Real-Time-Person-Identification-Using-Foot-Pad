#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubik_tester", "build_type": "cmake_ubinos", "app": true}

set_cache(UBINOS__BSP__LINKSCRIPT_FILE "${PROJECT_UBINOS_DIR}/source/ubinos/bsp/arch/arm/cortexm/nrf52/xxaa/flash_bdh.ld" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/ubik_tester_nrf52840dk.cmake)

