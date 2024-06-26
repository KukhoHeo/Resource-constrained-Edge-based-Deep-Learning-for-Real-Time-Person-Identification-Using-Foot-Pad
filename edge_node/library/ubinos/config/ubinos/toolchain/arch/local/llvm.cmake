#
# Copyright (c) 2022 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set_cache(PROJECT_TOOLCHAIN_TYPE "LLVM" STRING)

set_cache_default(PROJECT_TOOLCHAIN_F_NO_UNWIND_TABLES TRUE BOOL "-fno-unwind-tables")

set_cache_default(PROJECT_TOOLCHAIN_C_STD "C99" STRING "C standard version [C99 | GNU11]")

set_cache_default(PROJECT_TOOLCHAIN_CXX_STD "GNU++98" STRING "C++ standard version [GNU++98 | GNU++14]")
set_cache_default(PROJECT_TOOLCHAIN_CXX_F_NO_RTTI FALSE BOOL "-fno-rtti")
set_cache_default(PROJECT_TOOLCHAIN_CXX_F_NO_EXCEPTIONS FALSE BOOL "-fno-exceptions")
set_cache_default(PROJECT_TOOLCHAIN_CXX_F_NO_THREADSAFE_STATICS FALSE BOOL "-fno-threadsafe-statics")
set_cache_default(PROJECT_TOOLCHAIN_CXX_W_NO_CXX14_COMPAT FALSE BOOL "-Wno-c++14-compat")

set_cache(PROJECT_TOOLCHAIN_DATA_SECTION_NAME "__data" STRING)

########

if(PROJECT_TOOLCHAIN_F_NO_UNWIND_TABLES)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fno-unwind-tables")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-unwind-tables")
    set(CMAKE_ASM_FLAGS "${CMAKE_CXX_FLAGS} -fno-unwind-tables")
endif()

if(PROJECT_TOOLCHAIN_C_STD STREQUAL "C99")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=c99")
elseif(PROJECT_TOOLCHAIN_C_STD STREQUAL "GNU11")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu11")
else()
    message(FATAL_ERROR "Unsupported PROJECT_TOOLCHAIN_C_STD")
endif()

if(PROJECT_TOOLCHAIN_CXX_STD STREQUAL "GNU++98")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -std=gnu++98")
elseif(PROJECT_TOOLCHAIN_CXX_STD STREQUAL "GNU++14")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -std=gnu++14")
elseif(PROJECT_TOOLCHAIN_CXX_STD STREQUAL "C++11")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "Unsupported PROJECT_TOOLCHAIN_CXX_STD")
endif()
if(PROJECT_TOOLCHAIN_CXX_F_NO_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()
if(PROJECT_TOOLCHAIN_CXX_F_NO_EXCEPTIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif()
if(PROJECT_TOOLCHAIN_CXX_F_NO_THREADSAFE_STATICS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics")
endif()
if(PROJECT_TOOLCHAIN_CXX_W_NO_CXX14_COMPAT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++14-compat")
endif()

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp -D__ASSEMBLY__")

