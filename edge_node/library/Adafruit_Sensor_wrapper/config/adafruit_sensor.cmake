#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__ADAFRUIT_SENSOR TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} Adafruit_Sensor_wrapper)

set_cache_default(ADAFRUIT_SENSOR__BASE_DIR "${PROJECT_LIBRARY_DIR}/Adafruit_Sensor" STRING "Adafruit Sensor project base dir")

