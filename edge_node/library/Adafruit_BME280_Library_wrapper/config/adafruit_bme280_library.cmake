#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__ADAFRUIT_BME280_LIBRARY TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} Adafruit_BME280_Library_wrapper)

set_cache_default(ADAFRUIT_BME280_LIBRARY__BASE_DIR "${PROJECT_LIBRARY_DIR}/Adafruit_BME280_Library" STRING "Adafruit BME280 Library project base dir")

