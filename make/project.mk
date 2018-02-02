#
# Main Project Makefile
# This Makefile is included directly from the user project Makefile in order to call the component.mk
# makefiles of all components (in a separate make process) to build all the libraries, then links them
# together into the final file. If so, PWD is the project dir (we assume).
#

#
# This makefile requires the environment variable IDF_PATH to be set to the top-level esp-idf directory
# where this file is located.
#

.PHONY: menuconfig defconfig

help:
	@echo "Welcome to Espressif IDF build system. Some useful make targets:"
	@echo ""
	@echo "make menuconfig - Configure IDF project"
	@echo "make defconfig - Set defaults for all new configuration options"
	@echo ""
	@echo "make all - Build app, bootloader, partition table"
	@echo "make flash - Flash app, bootloader, partition table to a chip"
	@echo "make clean - Remove all build output"
	@echo "make size - Display the static memory footprint of the app"
	@echo "make size-components, size-files - Finer-grained memory footprints"
	@echo "make erase_flash - Erase entire flash contents"
	@echo "make monitor - Run idf_monitor tool to monitor serial output from app"
	@echo "make simple_monitor - Monitor serial output on terminal console"
	@echo "make list-components - List all components in the project"
	@echo ""
	@echo "make app - Build just the app"
	@echo "make app-flash - Flash just the app"
	@echo "make app-clean - Clean just the app"
	@echo "make print_flash_cmd - Print the arguments for esptool when flash"
	@echo ""
	@echo "See also 'make bootloader', 'make bootloader-flash', 'make bootloader-clean', "
	@echo "'make partition_table', etc, etc."

OS ?=

# make IDF_PATH a "real" absolute path
# * works around the case where a shell character is embedded in the environment variable value.
# * changes Windows-style C:/blah/ paths to MSYS style /c/blah
SANITISED_IDF_PATH:=$(realpath $(wildcard $(IDF_PATH)))

export IDF_PATH := $(SANITISED_IDF_PATH)

ifndef IDF_PATH
$(error IDF_PATH variable is not set to a valid directory.)
endif



# Default path to the project: we assume the Makefile including this file
# is in the project directory
ifndef PROJECT_PATH
PROJECT_PATH := $(abspath $(dir $(firstword $(MAKEFILE_LIST))))
export PROJECT_PATH
endif

# A list of the "common" makefiles, to use as a target dependency
COMMON_MAKEFILES := $(abspath make/project.mk $(firstword $(MAKEFILE_LIST)))
export COMMON_MAKEFILES

# The directory where we put all objects/libraries/binaries. The project Makefile can
# configure this if needed.
ifndef BUILD_DIR_BASE
BUILD_DIR_BASE := $(PROJECT_PATH)/build
endif
export BUILD_DIR_BASE



ifdef SRCDIRS
$(warning SRCDIRS variable is deprecated. These paths can be added to EXTRA_COMPONENT_DIRS or COMPONENT_DIRS instead.)
COMPONENT_DIRS += $(abspath $(SRCDIRS))
endif






# Functionality common to both top-level project makefile (project.mk)
# and component makefiles (component_wrapper.mk)
#

# Include project config makefile, if it exists.



#Handling of V=1/VERBOSE=1 flag
#
# if V=1, $(summary) does nothing and $(details) will echo extra details
# if V is unset or not 1, $(summary) echoes a summary and $(details) does nothing
VERBOSE ?=
V ?= $(VERBOSE)
ifeq ("$(V)","1")
summary := @true
details := @echo
else
summary := @echo
details := @true

# disable echoing of commands, directory names
MAKEFLAGS += --silent
endif  # $(V)==1



# General make utilities

# convenience variable for printing an 80 asterisk wide separator line
SEPARATOR:="*******************************************************************************"

# macro to remove quotes from an argument, ie $(call dequote,$(CONFIG_BLAH))
define dequote
$(subst ",,$(1))
endef
# "/) comment kept here to keep syntax highlighting happy


# (bootloader build doesn't need this, config is exported from top-level)
ifndef IS_BOOTLOADER_BUILD
include $(IDF_PATH)/make/project_config.mk
endif

# ELF depends on the library archive files for COMPONENT_LIBRARIES
