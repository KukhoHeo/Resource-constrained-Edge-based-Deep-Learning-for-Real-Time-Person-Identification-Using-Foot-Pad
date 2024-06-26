#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

if(INCLUDE__ARDUINOCORE_API)

    get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/arduinocore_api_ubinos" ABSOLUTE)
    get_filename_component(_tmp_api_dir "${ARDUINOCORE_API__BASE_DIR}" ABSOLUTE)

    file(GLOB_RECURSE _tmp_sources
        "${_tmp_source_dir}/*.c"
        "${_tmp_source_dir}/*.cpp"
        "${_tmp_source_dir}/*.S"
        "${_tmp_source_dir}/*.s")
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_sources})

    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_api_dir}/api/Common.cpp)
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_api_dir}/api/Stream.cpp)
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_api_dir}/api/Print.cpp)

endif(INCLUDE__ARDUINOCORE_API)

