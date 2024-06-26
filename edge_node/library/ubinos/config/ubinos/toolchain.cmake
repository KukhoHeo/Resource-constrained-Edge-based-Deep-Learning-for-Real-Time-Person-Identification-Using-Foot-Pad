#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set_cache_default(PROJECT_BUILD_TYPE "Debug" STRING "Build type [None | Debug | Release | MinSizeRel]")

set_cache_default(PROJECT_TOOLCHAIN_TYPE "" STRING "Toolchain type [GCC | LLVM]")
set_cache_default(PROJECT_TOOLCHAIN_PREFIX "" STRING "Toolchain prefix [arm-none-eabi]")
set_cache_default(PROJECT_TOOLCHAIN_GDB_COMMAND "" STRING "GDB command [arm-none-eabi-gdb]")

set_cache_default(PROJECT_TOOLCHAIN_DATA_SECTION_NAME ".data" STRING "Data section name [.data | __data]")

########

set(PROJECT_EXE_NAME "app")

if(PROJECT_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE "Debug")
elseif(PROJECT_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE "Release")
elseif(PROJECT_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_BUILD_TYPE "MinSizeRel")
else()
    set(CMAKE_BUILD_TYPE "None")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for cmake version 3.16.1
