#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubinos", "build_type": "cmake_ubinos"}

set_cache(UBINOS__BSP__BOARD_MODEL "NRF52840DONGLE" STRING)

set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_ORIGIN 0x00001000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_LENGTH 0x000EF000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_ORIGIN   0x20000008 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_LENGTH   0x0003FFF8 STRING)

set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_FILE        "${PROJECT_LIBRARY_DIR}/nrf5sdk_wrapper/source/components/softdevice/mbr/hex/mbr_nrf52_2.4.1_mbr.hex" PATH)
set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_INCLUDE_DIR "${PROJECT_LIBRARY_DIR}/nrf5sdk_wrapper/source/components/softdevice/mbr/headers" PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD  "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/gdb_flash_load_softdevice.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52dk/gdb_flash_reset_softdevice.gdb" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD  "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/t32_flash_load_dfu.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/t32_flash_reset_softdevice.cmm" PATH)

set_cache(UBINOS__BSP__USE_DTTY FALSE BOOL)

include(${CMAKE_CURRENT_LIST_DIR}/ubinos_nrf52840dk.cmake)

