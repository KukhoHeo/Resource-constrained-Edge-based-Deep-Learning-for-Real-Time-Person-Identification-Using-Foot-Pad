#
# Copyright (c) 2022 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

# ubinos_config_info {"name_base": "arduino_adafruit_bme280_bme280test", "build_type": "cmake_ubinos", "app": true}

include(${PROJECT_LIBRARY_DIR}/ArduinoCore-API_wrapper/config/arduinocore_api_adafruitnrf52840e.cmake)
include(${PROJECT_LIBRARY_DIR}/Adafruit_BusIO_wrapper/config/adafruit_busio.cmake)
include(${PROJECT_LIBRARY_DIR}/Adafruit_Sensor_wrapper/config/adafruit_sensor.cmake)
include(${PROJECT_LIBRARY_DIR}/Adafruit_BME280_Library_wrapper/config/adafruit_bme280_library.cmake)

####

set(INCLUDE__APP TRUE)
set(APP__NAME "arduino_adafruit_bme280_bme280test")

get_filename_component(_tmp_source_dir "${CMAKE_CURRENT_LIST_DIR}/${APP__NAME}" ABSOLUTE)

file(GLOB_RECURSE _tmp_sources
    "${_tmp_source_dir}/*.c"
    "${_tmp_source_dir}/*.cpp"
    "${_tmp_source_dir}/*.cc"
    "${_tmp_source_dir}/*.S"
    "${_tmp_source_dir}/*.s")

set(PROJECT_APP_SOURCES ${PROJECT_APP_SOURCES} ${_tmp_sources})

