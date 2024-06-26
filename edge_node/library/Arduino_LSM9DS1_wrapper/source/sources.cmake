#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

if(INCLUDE__ARDUINO_LSM9DS1)

get_filename_component(_tmp_source_dir "${ARDUINO_LSM9DS1__BASE_DIR}" ABSOLUTE)

include_directories(${_tmp_source_dir}/src)

set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/src/LSM9DS1.cpp)

endif(INCLUDE__ARDUINO_LSM9DS1)

