#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

if(INCLUDE__DHT_SENSOR_LIBRARY)

get_filename_component(_tmp_source_dir "${DHT_SENSOR_LIBRARY__BASE_DIR}" ABSOLUTE)

include_directories(${_tmp_source_dir})

set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/DHT.cpp)
set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/DHT_U.cpp)

endif(INCLUDE__DHT_SENSOR_LIBRARY)

