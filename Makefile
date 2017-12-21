# File: Makefile
# Date: 21/12/2017
# Author: Cameron A. Craig
# Copyright: 2017 Cameron A. Craig
# Description:
#    Top level makefile for triforce-control.
#    Exposes necessary make targets for typical use.

%: force
	@$(MAKE) -f ci.mk $@
	@$(MAKE) -f build.mk $@
force: ;

help:
	@echo  'Configuration targets:'
	@$(MAKE) -f scripts/kconfig/Makefile help
	@echo  ''
	@echo  'Continuous Integration targets:'
	@$(MAKE) -f ci.mk help

menuconfig:
	@$(MAKE) -f config.mk menuconfig
