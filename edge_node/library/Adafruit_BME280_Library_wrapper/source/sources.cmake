#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

if(INCLUDE__ADAFRUIT_BME280_LIBRARY)

get_filename_component(_tmp_source_dir "${ADAFRUIT_BME280_LIBRARY__BASE_DIR}" ABSOLUTE)

include_directories(${_tmp_source_dir})

set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Adafruit_BME280.cpp)

endif(INCLUDE__ADAFRUIT_BME280_LIBRARY)

