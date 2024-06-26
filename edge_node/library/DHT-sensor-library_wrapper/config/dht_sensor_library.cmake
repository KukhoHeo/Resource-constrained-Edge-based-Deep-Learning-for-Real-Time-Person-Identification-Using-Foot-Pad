#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__DHT_SENSOR_LIBRARY TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} DHT-sensor-library_wrapper)

set_cache_default(DHT_SENSOR_LIBRARY__BASE_DIR "${PROJECT_LIBRARY_DIR}/DHT-sensor-library" STRING "DHT-sensor-library project base dir")

