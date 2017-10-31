/**
* @file utilc-logging.h
* @author Cameron A. Craig
* @date 5 Jan 2017
* @version 0.1.0
* @copyright 2017 Cameron A. Craig
* @brief Defines uc_timing_h handle and function prototypes.
* -- RULE_3_2_CD_do_not_use_special_characters_in_filename
* -- RULE_8_1_A_provide_file_info_comment
*/

#ifndef UTILC_LOGGING_H
#define UTILC_LOGGING_H

#include "mbed.h"
#include <stdarg.h>

extern Serial *serial_ptr;

//Legacy function, TODO: remove
#define LOG( args...) \
  serial_ptr->printf(args); \

#endif
