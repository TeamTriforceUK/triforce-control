/* Copyright (c) 2017 Cameron A. Craig, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
* @file commands.h
* @author Cameron A. Craig
* @date 18 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines available commands.
*/

#ifndef TC_COMMANDS_H
#define TC_COMMANDS_H

#include "thread_args.h"
#include "command.h"

static const command_t available_commands[] = {
  {.id = FULLY_DISARM, .name = "disarm"},
  {.id = PARTIAL_DISARM, .name = "decarm"},
  {.id = PARTIAL_ARM, .name = "incarm"},
  {.id = FULLY_ARM, .name = "arm"},
  {.id = STATUS, .name = "status"},
  {.id = GET_PARAM, .name = "get"},
  {.id = SET_PARAM, .name = "set"},
  {.id = CALIBRATE_CHANNELS, .name = "calibrate"}
};

#define NUM_COMMANDS (sizeof(available_commands) / sizeof(command_t))

const char * command_get_str(command_id_t id);
int command_generate(command_t *command, char *buffer);
int command_execute(command_t *command, thread_args_t *targs);

int command_fully_disarm(command_t *command, thread_args_t *targs);
int command_partial_disarm(command_t *command, thread_args_t *targs);
int command_partial_arm(command_t *command, thread_args_t *targs);
int command_fully_arm(command_t *command, thread_args_t *targs);

int command_status(command_t *command, thread_args_t *targs);

int command_get_param(command_t *command, thread_args_t *targs);
int command_set_param(command_t *command, thread_args_t *targs);

int command_calibrate_channels(command_t *command, thread_args_t *targs);

#endif //TC_COMMANDS_H
