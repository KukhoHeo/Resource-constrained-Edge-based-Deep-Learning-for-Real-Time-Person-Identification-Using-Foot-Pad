#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "ubinos", "build_type": "cmake_ubinos"}

set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_ORIGIN 0x00027000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_FLASH_LENGTH 0x000D9000 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_ORIGIN 0x20002400 STRING)
set_cache(UBINOS__BSP__LINK_MEMMAP_RAM_LENGTH 0x0003DC00 STRING)

set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_PRESENT TRUE BOOL)
set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_NAME S140 STRING)
set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_BLE_API_VERSION 7 STRING)

set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_FILE "${PROJECT_LIBRARY_DIR}/nrf5sdk_wrapper/source/components/softdevice/s140/hex/s140_nrf52_7.0.1_softdevice.hex" PATH)
set_cache(UBINOS__BSP__NRF52_SOFTDEVICE_INCLUDE_DIR "${PROJECT_LIBRARY_DIR}/nrf5sdk_wrapper/source/components/softdevice/s140/headers" PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/gdb_flash_load_softdevice.gdb" PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52dk/gdb_flash_reset_softdevice.gdb" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_LOAD "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/t32_flash_load_softdevice.cmm" PATH)
set_cache(UBINOS__BSP__T32SCRIPT_FILE_RESET "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/cortexm/nrf52840dk/t32_flash_reset_softdevice.cmm" PATH)

include(${CMAKE_CURRENT_LIST_DIR}/ubinos_nrf52840dk.cmake)

