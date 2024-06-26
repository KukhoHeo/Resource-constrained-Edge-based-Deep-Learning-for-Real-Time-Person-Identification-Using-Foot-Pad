#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set_cache(UBINOS__BSP__CPU_ARCH                                                   "ARM"           STRING)
set_cache(UBINOS__BSP__CPU_ENDIAN                                                 "LITTLE"        STRING)

set_cache(UBINOS__BSP__STACK_OVERFLOW_CHECK_MARGIN                                0x0050          STRING)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_INIT                                        "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/gdb_init.gdb"                              PATH)

set_cache(UBINOS__BSP__GDBSCRIPT_FILE_ATTACH                                      "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/gdb_flash_attach.gdb"                      PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_DEBUG                                       "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/gdb_flash_debug.gdb"                       PATH)
set_cache(UBINOS__BSP__GDBSCRIPT_FILE_RUN                                         "${PROJECT_UBINOS_DIR}/resource/ubinos/bsp/arch/arm/gdb_flash_run.gdb"                         PATH)

