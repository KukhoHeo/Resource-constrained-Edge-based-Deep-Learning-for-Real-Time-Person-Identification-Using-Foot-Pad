#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__ADAFRUIT_BUSIO TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} Adafruit_BusIO_wrapper)

set_cache_default(ADAFRUIT_BUSIO__BASE_DIR "${PROJECT_LIBRARY_DIR}/Adafruit_BusIO" STRING "Adafruit BusIO project base dir")

