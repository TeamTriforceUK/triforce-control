/**
* @file utic-timing.c
* @author Cameron A. Craig
* @date 28 Dec 2016
* @version 0.1.0
* @copyright 2016 Cameron A. Craig
* @brief Measure time without thinking about the arithmetic.
* -- RULE_3_2_CD_do_not_use_special_characters_in_filename
* -- RULE_8_1_A_provide_file_info_comment
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "utilc-logging.h"

#define MSG_TIMESTAMP_BYTES 29
#define NULL_TERMINATOR 1

uint32_t uc_template_function(void){
	return EXIT_SUCCESS;
}

static const char * ucl_err_string[] = {
	"Error",
	"Ok",
	"Invalid destination type"
};

const char * ucl_err_to_string(enum ucl_error_code_e err){
	if(err < 0 || (err > MAX_ERR_NUM)){
		return "Error";
	}

	return ucl_err_string[err];
}

static const char * ucl_dest_type_string[] = {
	"UCL_DEST_FILE",
	"UCL_DEST_STDOUT",
	"UCL_DEST_STDERR",
	"UCL_DEST_UDP"
};

const char * ucl_dest_type_to_string(enum ucl_dest_type_e err){
	if(err < 0 || (err > MAX_DEST_NUM)){
		return "Error";
	}
	return ucl_dest_type_string[err];
}

static const char * ucl_log_level_string[] = {
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR"
};

const char * ucl_log_level_to_string(enum ucl_log_level_e err){
	if(err < 0 || (err > MAX_LOG_LEVEL_NUM)){
		return "Error";
	}
	return ucl_log_level_string[err];
}

uint32_t ucl_init(ucl_h ucl){
	ucl->dests = NULL; //NULL indicates no dests
	ucl->num_dests = 0;

	ucl->flags = 0;
	ucl->flags |= UCL_FLAGS_TIMESTAMP;
	ucl->flags |= UCL_FLAGS_LOG_LEVEL;
}

static void tv_to_timestamp(char *m, struct timeval *tv){
	char timestamp[64];
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64];
	nowtime = tv->tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", nowtm);
	snprintf(timestamp, sizeof(timestamp), "%s.%06d", tmbuf, tv->tv_usec);
	strcat(m, "[");
	strcat(m, timestamp);
	strcat(m,"] ");
	printf("testing: %s\n", m);
}

static uint32_t dest_log(ucl_h ucl, ucl_dest_h dest, struct ucl_message_s *message){
	if(!(dest->flags & UCL_FLAGS_DEST_ENABLED)){
		return UCL_ERROR_DEST_DISABLED;
	}

	const char* msg_user = message->message;
	size_t msg_user_size = strlen(msg_user);

	const char *msg_log_level;
	size_t msg_log_level_size = 0U;
	if(ucl->flags & UCL_FLAGS_LOG_LEVEL){
		msg_log_level = ucl_log_level_to_string(message->log_level);
		msg_log_level_size = strlen(msg_log_level);
		printf("Log Level: %s\n", msg_log_level);
	}

	char msg_timestamp[MSG_TIMESTAMP_BYTES];
	memset(msg_timestamp, 0, sizeof(msg_timestamp));
	size_t msg_timestamp_size = 0U;
	if(ucl->flags && UCL_FLAGS_TIMESTAMP){
		tv_to_timestamp(msg_timestamp, &message->tv);
		msg_timestamp_size = MSG_TIMESTAMP_BYTES;
		printf("Timestamp: %s\n", msg_timestamp);
	}

	//Work out the size of c-string we need to print
	char msg_full[msg_user_size + msg_timestamp_size + msg_log_level_size];
	memset(msg_full, 0, sizeof(msg_full));
	printf("Allocated %d\n", msg_user_size + msg_timestamp_size + msg_log_level_size + NULL_TERMINATOR);

	if(ucl->flags && UCL_FLAGS_TIMESTAMP){
		strcat(msg_full, msg_timestamp);
	}

	if(ucl->flags & UCL_FLAGS_LOG_LEVEL){
		strcat(msg_full, msg_log_level);
	}

	strcat(msg_full, ": ");
	strcat(msg_full, msg_user);
	//	printf(m);
	printf("Type: %s\n", ucl_dest_type_to_string(dest->type));
  char * filename;
  FILE *log_file;
	switch(dest->type){
		case UCL_DEST_FILE:
			filename = dest->conf.file.filename;
			printf("filename: %s\n", filename);
			log_file = fopen(filename, "a");
      printf("opened\r\n", filename);
			vfprintf(log_file, msg_full, message->args);
			fclose(log_file);
      printf("done\r\n", filename);
			break;
		case UCL_DEST_STDOUT:
			vfprintf(stdout, msg_full, message->args);
			break;
		case UCL_DEST_STDERR:
			vfprintf(stderr, msg_full, message->args);
			break;
		case UCL_DEST_UDP:
		default:
			return UCL_ERROR_INVALID_DEST;
	}
}

uint32_t ucl_log(ucl_h ucl, enum ucl_log_level_e log_level, const char *message, ...){
	//Create a message struct
	struct ucl_message_s m = {
		.message = message,
		.log_level = log_level
	};

	//Mark timestamp
	if(ucl->flags && UCL_FLAGS_TIMESTAMP){
		gettimeofday(&m.tv, NULL);
	}

	int i;
	for(i = 0; i < ucl->num_dests; i++){
		va_list args;
		va_start(args, message);
		va_copy(m.args, args);
		dest_log(ucl, &ucl->dests[i], &m);
		va_end(args);
		va_end(m.args);
	}
}

ucl_dest_h ucl_add_dest(ucl_h ucl, enum ucl_dest_type_e dest_type, ...){
	va_list args;
	va_start(args, dest_type);
	struct ucl_dest_s new_dest;
  memset(&new_dest, 0x00, sizeof(ucl_dest_s));
  new_dest.log_level = UCL_LL_DEBUG;
	new_dest.type = dest_type;
	new_dest.flags = UCL_FLAGS_DEST_ENABLED;

	switch(dest_type){
		case UCL_DEST_FILE:
			new_dest.conf.file.filename = va_arg(args, char*);
			break;
	}

	struct ucl_dest_s *tmp_ptr = (struct ucl_dest_s *) realloc(ucl->dests, (ucl->num_dests + 1) * sizeof(struct ucl_dest_s));
	if(tmp_ptr == NULL){
		free(ucl->dests);
		return NULL;
	} else {
		ucl->dests = tmp_ptr;
	}

	memcpy(&ucl->dests[ucl->num_dests], &new_dest, sizeof(struct ucl_dest_s));
	ucl->num_dests++;
	va_end(args);

	return &ucl->dests[ucl->num_dests - 1];
}

uint32_t ucl_disable_dest(ucl_dest_h dest){
	dest->flags &= ~UCL_FLAGS_DEST_ENABLED;
	return UCL_OK;
}

uint32_t ucl_free(ucl_h ucl){
	//Free destinations.
	free(ucl->dests);
}
