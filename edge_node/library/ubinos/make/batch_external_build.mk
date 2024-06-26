#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

config configd clean cleand:
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	@echo "Config          : ubinos library"
	@echo "Target board    : nucleof207zg"
	@echo "OS Kernel       : ubik"
	@echo "Link memory map : flash"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../config CONFIG_NAME=ubinos_nucleof207zg_rtctick_external_build
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	@echo "Config          : ubik tester application"
	@echo "Target board    : nucleof207zg"
	@echo "OS Kernel       : ubik"
	@echo "Link memory map : flash"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_NAME=ubik_tester_nucleof207zg_rtctick_external_build
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	@echo "Config          : command line interface tester application"
	@echo "Target board    : nucleof207zg"
	@echo "OS Kernel       : ubik"
	@echo "Link memory map : flash"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_NAME=cli_tester_nucleof207zg_rtctick_external_build
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""

%:
	@echo "Nothing to do"

