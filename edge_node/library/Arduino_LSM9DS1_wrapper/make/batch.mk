#
# Copyright (c) 2021 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

###############################################################################

%:
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_arduinonano33ble
	@echo ""
	@echo ""
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_arduinonano33ble_bleuart
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_adafruitnrf52840e
	@echo ""
	@echo ""
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_adafruitnrf52840e_bleuart
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_nrf52840dk
	@echo ""
	@echo ""
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_nrf52840dk_bleuart
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""
	@echo ""
	make -f makefile.mk $@ CONFIG_DIR=../app CONFIG_NAME=arduino_lsm9ds1_SimpleAccelerometer_nucleof207zg
	@echo ""
	@echo ""


	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo ""

###############################################################################

