#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "heap_tester", "build_type": "cmake_ubinos", "app": true}

set_cache(PROJECT_BUILD_TYPE "Release" STRING)

set_cache(UBINOS__UBICLIB__EXCLUDE_CLI FALSE BOOL)

set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_BOUNDARY_CHECK TRUE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK TRUE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_LOGMD TRUE BOOL)

set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT FALSE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT  FALSE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP   FALSE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP    FALSE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY   FALSE BOOL)
set_cache(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY   FALSE BOOL)

set_cache(UBINOS__UBICLIB_TEST__EXCLUDE_HEAPTEST FALSE BOOL)

include(${PROJECT_UBINOS_DIR}/config/ubinos_sam9xe512ek.cmake)
include(${PROJECT_UBINOS_DIR}/config/ubinos/ubiclib_test.cmake)

####

set(INCLUDE__APP TRUE)
set(APP__NAME "heap_tester")

get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/${APP__NAME}" ABSOLUTE)

file(GLOB_RECURSE _tmp_sources
    "${_tmp_source_dir}/*.c"
    "${_tmp_source_dir}/*.cpp"
    "${_tmp_source_dir}/*.cc"
    "${_tmp_source_dir}/*.S"
    "${_tmp_source_dir}/*.s")

set(PROJECT_APP_SOURCES ${PROJECT_APP_SOURCES} ${_tmp_sources})

