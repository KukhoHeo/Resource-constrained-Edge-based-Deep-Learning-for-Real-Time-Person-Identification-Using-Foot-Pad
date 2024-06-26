#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__ARDUINOCORE_API TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} ArduinoCore-API_wrapper)

set_cache_default(ARDUINOCORE_API__BASE_DIR "${PROJECT_LIBRARY_DIR}/ArduinoCore-API" STRING "ArduinoCore-API project base dir")

set_cache_default(ARDUINOCORE_API__USE_WIRE TRUE BOOL "Use Wire(I2C) API")
set_cache_default(ARDUINOCORE_API__USE_SPI TRUE BOOL "Use SPI API")
set_cache_default(ARDUINOCORE_API__USE_SERVO TRUE BOOL "Use Servo API")
set_cache_default(ARDUINOCORE_API__TENSORFLOWLITE_DIR "${PROJECT_LIBRARY_DIR}/Arduino_TensorFlowLite" STRING "Arduino_TensorFlowLite project base dir")
set_cache_default(ARDUINOCORE_API__INCLUDE_TENSORFLOWLITE FALSE BOOL "Include Arduino_TensorFlowLite")

set_cache_default(ARDUINOCORE_API__VERSION "10813" STRING "Arduino version")

####

get_filename_component(_tmp_source_dir "${ARDUINOCORE_API__BASE_DIR}" ABSOLUTE)

include_directories(${_tmp_source_dir})

get_filename_component(_tmp_wrapper_dir "${PROJECT_LIBRARY_DIR}/ArduinoCore-API_wrapper" ABSOLUTE)
string(TOLOWER ${UBINOS__BSP__BOARD_VARIATION_NAME} _tmp_board_model)

if(ARDUINOCORE_API__USE_WIRE)
    include_directories("${_tmp_wrapper_dir}/include/Wire")
endif(ARDUINOCORE_API__USE_WIRE)
if(ARDUINOCORE_API__USE_SPI)
    include_directories("${_tmp_wrapper_dir}/include/SPI")
endif(ARDUINOCORE_API__USE_SPI)
if(ARDUINOCORE_API__USE_SERVO)
    include_directories("${_tmp_wrapper_dir}/include/Servo")
endif(ARDUINOCORE_API__USE_SERVO)
include_directories("${_tmp_wrapper_dir}/include")

if(UBINOS__BSP__CORTEX_MX)
    include_directories("${_tmp_wrapper_dir}/include/arch/arm/cortexm/${_tmp_board_model}")
    include_directories("${_tmp_wrapper_dir}/source/arduinocore_api_ubinos/arch/arm/cortexm/${_tmp_board_model}")
else()
    message(FATAL_ERROR "Unsupported CPU_ARCH")
endif(UBINOS__BSP__CORTEX_MX)

add_definitions("-DARDUINO=${ARDUINOCORE_API__VERSION}")
add_definitions("-DADAFRUIT_BUSIO_NOT_USE_FAST_PINIO")
add_definitions("-DARDUINOSTL_M_H")
