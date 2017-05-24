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
#include <sys/time.h>
#include <stdint.h>
#include <stdarg.h>

extern Serial *serial_ptr;

#define UCL_FLAGS_TIMESTAMP (1 << 0)
#define UCL_FLAGS_LOG_LEVEL (1 << 1)

#define UCL_FLAGS_DEST_ENABLED (1 << 2)

//Legacy function, TODO: remove
#define LOG( args...) \
  serial_ptr->printf(args); \

enum ucl_error_code_e {
	UCL_ERROR,
	UCL_OK,
	UCL_ERROR_INVALID_DEST,
	UCL_ERROR_DEST_DISABLED,
	MAX_ERR_NUM
};

enum ucl_log_level_e {
	UCL_LL_DEBUG,
	UCL_LL_INFO,
	UCL_LL_WARNING,
	UCL_LL_ERROR,
	MAX_LOG_LEVEL_NUM
};

enum ucl_dest_type_e {
	UCL_DEST_FILE,
	UCL_DEST_STDOUT,
	UCL_DEST_STDERR,
	UCL_DEST_UDP,
	MAX_DEST_NUM
};

typedef struct ucl_dest_s* ucl_dest_h;
struct ucl_dest_s {
	enum ucl_log_level_e log_level; //Log messages below this level wil be ignored.
	enum ucl_dest_type_e type;
	union {
		struct {

		} udp;
		struct {
			char *filename;
		} file;
	} conf;

	uint32_t flags;
};

struct ucl_message_s {
	const char *message;
	enum ucl_log_level_e log_level;
	va_list args;
	struct timeval tv;
};

typedef struct ucl_s* ucl_h;
struct ucl_s {
	unsigned log_frequency; // Log every n call

	ucl_dest_h dests; // File descriptors for output destinations
	unsigned num_dests;

	uint32_t flags;
};

uint32_t ucl_init(ucl_h ucl);
uint32_t ucl_log(ucl_h ucl, enum ucl_log_level_e log_level, const char *message, ...);
ucl_dest_h ucl_add_dest(ucl_h ucl, enum ucl_dest_type_e dest_type, ...);
uint32_t ucl_disable_dest(ucl_dest_h dest);
uint32_t ucl_free(ucl_h ucl);
//uint32_t ucl_log_error(ucl_h ucl);

#endif
