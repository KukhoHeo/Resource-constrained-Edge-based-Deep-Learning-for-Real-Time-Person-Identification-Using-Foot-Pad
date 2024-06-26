#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubinos", "build_type": "cmake_ubinos"}

include(${PROJECT_UBINOS_DIR}/config/ubinos/toolchain/arch/local/llvm.cmake)
include(${PROJECT_UBINOS_DIR}/config/ubinos/toolchain.cmake)
include(${PROJECT_UBINOS_DIR}/config/ubinos/bsp/arch/local/llvm.cmake)
include(${PROJECT_UBINOS_DIR}/config/ubinos/bsp.cmake)

