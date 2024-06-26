#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

if(INCLUDE__UBINOS__BSP)
    get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/ubinos/bsp" ABSOLUTE)
    file(GLOB_RECURSE _tmp_sources
        "${_tmp_source_dir}/*.c"
        "${_tmp_source_dir}/*.cpp"
        "${_tmp_source_dir}/*.S"
        "${_tmp_source_dir}/*.s")
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_sources})
endif(INCLUDE__UBINOS__BSP)

if(INCLUDE__UBINOS__UBICLIB)
    get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/ubinos/ubiclib" ABSOLUTE)
    file(GLOB_RECURSE _tmp_sources
        "${_tmp_source_dir}/*.c"
        "${_tmp_source_dir}/*.cpp"
        "${_tmp_source_dir}/*.S"
        "${_tmp_source_dir}/*.s")
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_sources})
endif(INCLUDE__UBINOS__UBICLIB)

if(INCLUDE__UBINOS__UBICLIB_TEST)
    get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/ubinos/ubiclib_test" ABSOLUTE)
    file(GLOB_RECURSE _tmp_sources
        "${_tmp_source_dir}/*.c"
        "${_tmp_source_dir}/*.cpp"
        "${_tmp_source_dir}/*.S"
        "${_tmp_source_dir}/*.s")
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_sources})
endif(INCLUDE__UBINOS__UBICLIB_TEST)

if(INCLUDE__UBINOS__UBIK)
    get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/ubinos/ubik" ABSOLUTE)
    file(GLOB_RECURSE _tmp_sources
        "${_tmp_source_dir}/*.c"
        "${_tmp_source_dir}/*.cpp"
        "${_tmp_source_dir}/*.S"
        "${_tmp_source_dir}/*.s")
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_sources})
endif(INCLUDE__UBINOS__UBIK)

if(INCLUDE__UBINOS__UBIK_TEST)
    get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/ubinos/ubik_test" ABSOLUTE)
    file(GLOB_RECURSE _tmp_sources
        "${_tmp_source_dir}/*.c"
        "${_tmp_source_dir}/*.cpp"
        "${_tmp_source_dir}/*.S"
        "${_tmp_source_dir}/*.s")
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_sources})
endif(INCLUDE__UBINOS__UBIK_TEST)


