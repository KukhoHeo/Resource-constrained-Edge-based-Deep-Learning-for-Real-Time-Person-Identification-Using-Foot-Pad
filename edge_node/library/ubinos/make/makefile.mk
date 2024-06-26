#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

help: common-help

include common.mk
-include custom.mk

%: common-% ;

