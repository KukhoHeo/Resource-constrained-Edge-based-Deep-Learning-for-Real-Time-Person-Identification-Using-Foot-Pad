#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__ARDUINO_LSM9DS1 TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} Arduino_LSM9DS1_wrapper)

set_cache_default(ARDUINO_LSM9DS1__BASE_DIR "${PROJECT_LIBRARY_DIR}/Arduino_LSM9DS1" STRING "Arduino LSM9DS1 project base dir")

