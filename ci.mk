# File: triforce.mk
# Date: 27/09/2017
# Author: Cameron A. Craig
# Copyright: 2017 Cameron A. Craig
# Description:
#    Make targets for code style checking and static analysis.
#    Designed for use on continuous integration servers.

STYLE_CHECK_PATH=../nsiqcppstyle/nsiqcppstyle
STYLE_CHECK_FILEFILTER=filefilter.txt
STYLE_CHECK_SRC_DIR=src/ include/
STYLE_CHECK_OUTPUT_PATH=style.xml

STATIC_CHECK_PATH=/usr/bin/cppcheck
STATIC_CHECK_INC_DIR=include/
STATIC_CHECK_SRC_DIR=src/
STATIC_CHECK_REPORT_DIR=static.txt

ci: check_style check_static

check_style:
	@echo "Starting style checker...\r\n"
	$(STYLE_CHECK_PATH) --ci -o $(STYLE_CHECK_OUTPUT_PATH) -f $(STYLE_CHECK_FILEFILTER) $(STYLE_CHECK_SRC_DIR)

check_static:
	@echo "Starting static analysis...\r\n"
	$(STATIC_CHECK_PATH) $(STATIC_CHECK_SRC_DIR) -I $(STATIC_CHECK_INC_DIR) 2> $(STATIC_CHECK_REPORT_DIR) --error-exitcode=1

help:
	@echo "check_style:\tRun style checker"
	@echo "check_static:\tRun static analysis"
	@echo "ci:\t\tRun style checker and static analysis"
