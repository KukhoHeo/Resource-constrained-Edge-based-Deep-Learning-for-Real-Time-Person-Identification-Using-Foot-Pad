#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "DHTtester", "build_type": "cmake_ubinos", "app": true}

include(${PROJECT_LIBRARY_DIR}/ArduinoCore-API_wrapper/config/arduinocore_api_nrf52840dk.cmake)
include(${PROJECT_LIBRARY_DIR}/Adafruit_BusIO_wrapper/config/adafruit_busio.cmake)
include(${PROJECT_LIBRARY_DIR}/Adafruit_Sensor_wrapper/config/adafruit_sensor.cmake)
include(${PROJECT_LIBRARY_DIR}/DHT-sensor-library_wrapper/config/dht_sensor_library.cmake)

####

set(INCLUDE__APP TRUE)
set(APP__NAME "DHTtester")

get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/${APP__NAME}" ABSOLUTE)

file(GLOB_RECURSE _tmp_sources
    "${_tmp_source_dir}/*.c"
    "${_tmp_source_dir}/*.cpp"
    "${_tmp_source_dir}/*.S"
    "${_tmp_source_dir}/*.s")

set(PROJECT_APP_SOURCES ${PROJECT_APP_SOURCES} ${_tmp_sources})

