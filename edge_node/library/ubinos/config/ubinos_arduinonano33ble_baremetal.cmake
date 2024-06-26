#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubinos", "build_type": "cmake_ubinos"}

set_cache(UBINOS__BSP__BOARD_MODEL "ARDUINONANO33BLE" STRING)

set_cache(UBINOS__BSP__USE_DTTY FALSE BOOL)

include(${CMAKE_CURRENT_LIST_DIR}/ubinos_nrf52840dk_baremetal.cmake)

