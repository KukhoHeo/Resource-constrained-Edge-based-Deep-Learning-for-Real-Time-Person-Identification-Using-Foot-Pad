#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

###############################################################################

_BASE_DIR                = $(realpath ..)

###############################################################################

ifeq ($(strip $(CONFIG_NAME)),)
_CONFIG_NAME             = default
else
_CONFIG_NAME             = $(CONFIG_NAME)
endif

ifeq ($(strip $(CONFIG_DIR)),)
_CONFIG_DIR              = $(_BASE_DIR)/app
else
_CONFIG_DIR              = $(realpath $(CONFIG_DIR))
endif

ifeq ($(strip $(LIBRARY_DIR)),)
ifeq ($(lastword $(subst /, ,$(_BASE_DIR))),ubinos)
_LIBRARY_DIR             = $(realpath ../..)
else
_LIBRARY_DIR             = $(_BASE_DIR)/library
endif
else
_LIBRARY_DIR             = $(realpath $(LIBRARY_DIR))
endif

ifeq ($(strip $(DEBUG_SERVER_SERIAL)),)
_DEBUG_SERVER_SERIAL     = ""
else
_DEBUG_SERVER_SERIAL     = $(DEBUG_SERVER_SERIAL)
endif

ifeq ($(strip $(DEBUG_SERVER_PORT)),)
_DEBUG_SERVER_PORT       = ""
else
_DEBUG_SERVER_PORT       = $(DEBUG_SERVER_PORT)
endif

###############################################################################

ifeq ($(strip $(UBINOS_DIR)),)
ifeq ($(lastword $(subst /, ,$(_BASE_DIR))),ubinos)
_UBINOS_DIR             = $(_BASE_DIR)
else
_UBINOS_DIR             = $(_LIBRARY_DIR)/ubinos
endif
else
_UBINOS_DIR             = $(realpath $(UBINOS_DIR))
endif

ifeq ($(strip $(TOOLBOX)),)
_TOOLBOX                = $(_UBINOS_DIR)/make/toolbox.py
else
_TOOLBOX                = $(realpath $(TOOLBOX))
endif

###############################################################################

_SOURCE_DIR             = $(_BASE_DIR)/source

ifeq ($(strip $(OUTPUT_DIR)),)
ifeq ($(strip $(OUTPUT_BASE_DIR)),)
_OUTPUT_DIR             = $(_BASE_DIR)/output/$(_CONFIG_NAME)
else
_OUTPUT_DIR             = $(realpath $(OUTPUT_BASE_DIR))/output/$(_CONFIG_NAME)
endif
else
_OUTPUT_DIR             = $(OUTPUT_DIR)
endif

###############################################################################

ifeq ($(strip $(RESOURCE_DIR)),)
_RESOURCE_DIR           = $(_BASE_DIR)/resource
else
_RESOURCE_DIR           = $(realpath $(RESOURCE_DIR))
endif

ifeq ($(strip $(MAKE_DIR)),)
_MAKE_DIR               = $(_BASE_DIR)/make
else
_MAKE_DIR               = $(realpath $(MAKE_DIR))
endif

###############################################################################

ifeq ($(strip $(LIBRARY_UPGRADE_LIST)),)
_LIBRARY_UPGRADE_LIST   = ubinos
else
_LIBRARY_UPGRADE_LIST   = $(LIBRARY_UPGRADE_LIST)
endif

###############################################################################

ifeq ($(strip $(SYSTEM_NAME)),)
_SYSTEM_NAME            = $(shell python "$(_TOOLBOX)" system_name)
else
_SYSTEM_NAME            = $(SYSTEM_NAME)
endif
ifeq ($(strip $(JOBS)),)
_JOBS                   = $(shell python "$(_TOOLBOX)" cpu_count)
else
_JOBS                   = $(JOBS)
endif
ifeq ($(strip $(PRECMD)),)
_PRECMD                 = :
else
_PRECMD                 = $(PRECMD)
endif
ifeq ($(strip $(COMPILER_LAUNCHER)),)
_COMPILER_LAUNCHER     = ""
else
_COMPILER_LAUNCHER     = $(COMPILER_LAUNCHER)
endif

###############################################################################

_CMAKE_OPTION           = -G "Unix Makefiles"
_CMAKE_OPTION          += -D PROJECT_CONFIG_NAME=$(_CONFIG_NAME)
_CMAKE_OPTION          += -D PROJECT_CONFIG_DIR="$(_CONFIG_DIR)"
_CMAKE_OPTION          += -D PROJECT_LIBRARY_DIR="$(_LIBRARY_DIR)"
_CMAKE_OPTION          += -D UBINOS__BSP__DEBUG_SERVER_SERIAL="$(_DEBUG_SERVER_SERIAL)"
_CMAKE_OPTION          += -D UBINOS__BSP__DEBUG_SERVER_PORT="$(_DEBUG_SERVER_PORT)"

ifeq ($(strip $(_COMPILER_LAUNCHER)),)
else
_CMAKE_OPTION          += -D CMAKE_C_COMPILER_LAUNCHER=$(_COMPILER_LAUNCHER)
_CMAKE_OPTION          += -D CMAKE_CXX_COMPILER_LAUNCHER=$(_COMPILER_LAUNCHER)
endif

###############################################################################

define begin_message
	@echo ""
	@echo "###############################################################################"
	@echo "# [Begin $@ - $(_CONFIG_NAME)]"
	@echo ""
endef

define end_message
	@echo ""
	@echo "# [End   $@ - $(_CONFIG_NAME)]"
	@echo "###############################################################################"
	@echo ""
endef

###############################################################################

common-help:
	$(call begin_message)
	@echo "CONFIG_NAME                  = $(_CONFIG_NAME)"
	@echo "CONFIG_DIR                   = $(_CONFIG_DIR)"
	@echo "LIBRARY_DIR                  = $(_LIBRARY_DIR)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "UBINOS_DIR                   = $(_UBINOS_DIR)"
	@echo "TOOLBOX                      = $(_TOOLBOX)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "BASE_DIR                     = $(_BASE_DIR)"
	@echo "SOURCE_DIR                   = $(_SOURCE_DIR)"
	@echo "OUTPUT_DIR (BINARY_DIR)      = $(_OUTPUT_DIR)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "CMAKE_OPTION                 = $(_CMAKE_OPTION)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "RESOURCE_DIR                 = $(_RESOURCE_DIR)"
	@echo "MAKE_DIR                     = $(_MAKE_DIR)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "LIBRARY_UPGRADE_LIST         = $(_LIBRARY_UPGRADE_LIST)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "SYSTEM_NAME                  = $(_SYSTEM_NAME)"
	@echo "JOBS                         = $(_JOBS)"
	@echo "PRECMD                       = $(_PRECMD)"
	@echo "COMPILER_LAUNCHER            = $(COMPILER_LAUNCHER)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "MAKEFILE_LIST                = $(MAKEFILE_LIST)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "make help                    (show help)"
	@echo ""
	@echo "make xsel                    (run config selector with GUI or new window)"
	@echo ""
	@echo "make all                     (config and build)"
	@echo ""
	@echo "make config                  (configure)"
	@echo "make configd                 (cleand and config)"
	@echo ""
	@echo "make build                   (build)"
	@echo ""
	@echo "make clean                   (clean)"
	@echo "make cleand                  (clean and remove output directory)"
	@echo ""
	@echo "make rebuild                 (clean and all)"
	@echo "make rebuildd                (cleand and all)"
	@echo ""
	@echo "make dserver                 (run debug server)"
	@echo "make xdserver                (run debug server with GUI or new window)"
	@echo ""
	@echo "make load                    (load execution binary file into target)"
	@echo "make reset                   (reset target)"
	@echo ""
	@echo "make run                     (run execution binary file on target)"
	@echo "make xrun                    (run execution binary file on target with GUI or new window)"
	@echo ""
	@echo "make debug                   (run execution binary file on target and attach debug CLI)"
	@echo "make xdebug                  (run execution binary file on target and attach debug CLI with GUI or new window)"
	@echo ""
	@echo "make attach                  (attach debug CLI)"
	@echo "make xattach                 (attach debug CLI with GUI or new window)"
	@echo ""
	@echo "make xconfig                 (run config GUI using cmake-gui)"
	@echo "make menuconfig              (run config TUI using ccmake)"
	@echo ""
	@echo "make upgrade                 (upgrade libraries listed in LIBRARY_UPGRADE_LIST)"
	@echo "make upgradef                (upgrade libraries listed in LIBRARY_UPGRADE_LIST with force option)"
	@echo "make upgradea                (upgrade all libraries)"
	@echo "make upgradeaf               (upgrade all libraries with force option)"
	@echo ""
	@echo "-------------------------------------------------------------------------------"
	@echo ""
	@echo "make zbatch-<make target>    (do <make target> with all <config name> listed in batch.mk)"
	$(call end_message)

###############################################################################

common-all: config build

common-config:
	$(call begin_message)
	$(_PRECMD) && mkdir -p "$(_OUTPUT_DIR)"
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && cmake $(_CMAKE_OPTION) "$(_SOURCE_DIR)"
	$(_PRECMD) && mkdir -p "$(_OUTPUT_DIR)/../Default"
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && python "$(_TOOLBOX)" copy_file compile_commands.json "$(_OUTPUT_DIR)/../Default/"
	$(call end_message)

common-configd: cleand config

common-build:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make -j $(_JOBS)
	$(call end_message)

common-clean:
	$(call begin_message)
ifeq ("$(shell python "$(_TOOLBOX)" is_existing_path "$(_OUTPUT_DIR)/Makefile")", "1")
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make clean
endif
	$(call end_message)

common-cleand:
	$(call begin_message)
ifeq ("$(shell python "$(_TOOLBOX)" is_removable_dir "$(_OUTPUT_DIR)")", "1")
	$(_PRECMD) && rm -rf "$(_OUTPUT_DIR)" || true
endif
	$(call end_message)

common-rebuild: clean all

common-rebuildd: cleand all

common-dserver:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make dserver
	$(call end_message)

common-xdserver:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make xdserver
	$(call end_message)

common-load:
	$(call begin_message)
	@echo ""
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make reset
	@echo ""
	@echo ""
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make load
	@echo ""
	@echo ""
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make reset
	@echo ""
	$(call end_message)

common-run:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make run
	$(call end_message)

common-xrun:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make xrun
	$(call end_message)

common-reset:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make reset
	$(call end_message)

common-debug:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make debug
	$(call end_message)

common-xdebug:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make xdebug
	$(call end_message)

common-attach:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make attach
	$(call end_message)

common-xattach:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && make xattach
	$(call end_message)

common-xconfig:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && cmake-gui .
	$(call end_message)

common-menuconfig:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && ccmake .
	$(call end_message)

common-xsel:
	$(call begin_message)
ifeq ("$(shell python "$(_TOOLBOX)" is_python3)", "1")
	$(_PRECMD) && cd $(dir $(firstword $(MAKEFILE_LIST))) && python  "$(_UBINOS_DIR)/make/confsel.py" --lib-absolute .. $(_LIBRARY_DIR)
else
	$(_PRECMD) && cd $(dir $(firstword $(MAKEFILE_LIST))) && python3 "$(_UBINOS_DIR)/make/confsel.py" --lib-absolute .. $(_LIBRARY_DIR)
endif
	$(call end_message)

###############################################################################

common-upgrade:
	$(call begin_message)
	$(_PRECMD) && cd "$(_LIBRARY_DIR)" && git submodule update --remote --init $(_LIBRARY_UPGRADE_LIST)
	$(call end_message)

common-upgradef:
	$(call begin_message)
	$(_PRECMD) && cd "$(_LIBRARY_DIR)" && git submodule update --remote --init -f $(_LIBRARY_UPGRADE_LIST)
	$(call end_message)

common-upgradea:
	$(call begin_message)
	$(_PRECMD) && cd "$(_LIBRARY_DIR)" && git submodule update --remote --init
	$(call end_message)

common-upgradeaf:
	$(call begin_message)
	$(_PRECMD) && cd "$(_LIBRARY_DIR)" && git submodule update --remote --init -f
	$(call end_message)

###############################################################################

common-env:
	$(call begin_message)
	$(call end_message)

common-cleanenv:
	$(call begin_message)
	$(call end_message)

common-doc:
	$(call begin_message)
	$(_PRECMD) && cd "$(_OUTPUT_DIR)" && doxygen
	$(call end_message)

common-test:
	$(call begin_message)
	$(call end_message)

###############################################################################

common-zbatch-all:
	make -f batch.mk SYSTEM_NAME=$(_SYSTEM_NAME) all
ifeq ("$(shell python "$(_TOOLBOX)" is_existing_path "batch_external_build.mk")", "1")
	make -f batch_external_build.mk SYSTEM_NAME=$(_SYSTEM_NAME) config
endif

common-zbatch-rebuild:
	make -f batch.mk SYSTEM_NAME=$(_SYSTEM_NAME) rebuild
ifeq ("$(shell python "$(_TOOLBOX)" is_existing_path "batch_external_build.mk")", "1")
	make -f batch_external_build.mk SYSTEM_NAME=$(_SYSTEM_NAME) clean config
endif

common-zbatch-rebuildd:
	make -f batch.mk SYSTEM_NAME=$(_SYSTEM_NAME) rebuildd
ifeq ("$(shell python "$(_TOOLBOX)" is_existing_path "batch_external_build.mk")", "1")
	make -f batch_external_build.mk SYSTEM_NAME=$(_SYSTEM_NAME) cleand config
endif

common-zbatch-load:
	$(call begin_message)
	$(call end_message)

common-zbatch-run:
	$(call begin_message)
	$(call end_message)

common-zbatch-reset:
	$(call begin_message)
	$(call end_message)

common-zbatch-debug:
	$(call begin_message)
	$(call end_message)

common-zbatch-dserver:
	$(call begin_message)
	$(call end_message)

common-zbatch-xdserver:
	$(call begin_message)
	$(call end_message)

common-zbatch-attach:
	$(call begin_message)
	$(call end_message)

common-zbatch-env:
	$(call begin_message)
	$(call end_message)

common-zbatch-cleanenv:
	$(call begin_message)
	$(call end_message)

common-zbatch-xconfig:
	$(call begin_message)
	$(call end_message)

common-zbatch-menuconfig:
	$(call begin_message)
	$(call end_message)

common-zbatch-test:
	$(call begin_message)
	$(call end_message)

common-zbatch-%:
	make -f batch.mk SYSTEM_NAME=$(_SYSTEM_NAME) $*
ifeq ("$(shell python "$(_TOOLBOX)" is_existing_path "batch_external_build.mk")", "1")
	make -f batch_external_build.mk SYSTEM_NAME=$(_SYSTEM_NAME) $*
endif

###############################################################################

