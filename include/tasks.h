/* Copyright (c) 2017 Cameron A. Craig, Euan W. Mutch, MIT License
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
* @file tasks.h
* @author Cameron A. Craig, Euan W. Mutch
* @date 15 May 2017
* @copyright 2017 Cameron A. Craig, Euan W. Mutch
* @brief Defines struct to share data between threads.
*/

#ifndef INCLUDE_TASKS_H_
#define INCLUDE_TASKS_H_

#include "mbed.h"
#include "task.h"

/* The COUNTER macro is a GCC preprocessor macro that increments after every
   time it is used. We use it here to allow the task IDs to be a sequence
   where each term is one greater than the last.
*/
#define  TASK_READ_SERIAL          __COUNTER__
#define  TASK_PROCESS_COMMANDS     __COUNTER__
#define  TASK_LED_STATE            __COUNTER__
// #define  TASK_READ_RECEIVERS       __COUNTER__
// #define  TASK_ARMING               __COUNTER__
// #define  TASK_FAILSAFE             __COUNTER__
// #define  TASK_SET_ESCS             __COUNTER__
// #define  TASK_CALC_ORIENTATION     __COUNTER__
#define  TASK_COLLECT_TELEMETRY    __COUNTER__
#define  TASK_STREAM_TELEMETRY     __COUNTER__
#define  TASK_CALIBRATE_CHANNELS   __COUNTER__
#define  NUM_TASKS                 __COUNTER__

#ifdef TASK_READ_SERIAL
void task_read_serial(const void *targs);
#endif

#ifdef TASK_PROCESS_COMMANDS
void task_process_commands(const void *targs);
#endif

#ifdef TASK_LED_STATE
void task_state_leds(const void *targs);
#endif

#ifdef TASK_READ_RECEIVERS
void task_read_receiver(const void *targs);
#endif

#ifdef TASK_ARMING
void task_arming(const void *targs);
#endif

#ifdef TASK_FAILSAFE
void task_failsafe(const void *targs);
#endif

#ifdef TASK_SET_ESCS
void task_set_escs(const void *targs);
#endif

#ifdef TASK_CALC_ORIENTATION
// void task_calc_escs(const void *targs);
void task_calc_orientation(const void *targs);
#endif

#ifdef TASK_COLLECT_TELEMETRY
void task_collect_telemetry(const void *targs);
#endif

#ifdef TASK_STREAM_TELEMETRY
void task_stream_telemetry(const void *targs);
#endif

#ifdef TASK_CALIBRATE_CHANNELS
void task_calibrate_channels(const void *targs);
#endif

// Debug tasks
void task_print_channels(const void *targs);

static volatile task_t tasks[] = {
#ifdef TASK_READ_SERIAL
  {.id = TASK_READ_SERIAL,        .name = "Read Serial",        .func = task_read_serial,        .args = NULL, .priority = osPriorityRealtime, .active = true},
#endif
#ifdef TASK_PROCESS_COMMANDS
  {.id = TASK_PROCESS_COMMANDS,   .name = "Process Commands",   .func = task_process_commands,   .args = NULL, .priority = osPriorityHigh,     .active = true},
#endif
#ifdef TASK_LED_STATE
  {.id = TASK_LED_STATE,          .name = "LED State",          .func = task_state_leds,         .args = NULL, .priority = osPriorityNormal,   .active = true},
#endif
#ifdef TASK_READ_RECEIVERS
  {.id = TASK_READ_RECEIVERS,     .name = "Read RX",            .func = task_read_receiver,      .args = NULL, .priority = osPriorityNormal,   .active = true},
#endif
#ifdef TASK_ARMING
  {.id = TASK_ARMING,             .name = "Arming" ,            .func = task_arming,             .args = NULL, .priority = osPriorityNormal,   .active = false},
#endif
#ifdef TASK_FAILSAFE
  {.id = TASK_FAILSAFE,           .name = "Failsafe" ,          .func = task_failsafe,           .args = NULL, .priority = osPriorityNormal,   .active = false},
#endif
#ifdef TASK_SET_ESCS
  {.id = TASK_SET_ESCS,           .name = "Set ESCs",           .func = task_set_escs,           .args = NULL, .priority = osPriorityNormal,   .active = false},
#endif
#ifdef TASK_CALC_ORIENTATION
  {.id = TASK_CALC_ORIENTATION,   .name = "Calc Orientation",   .func = task_calc_orientation,   .args = NULL, .priority = osPriorityNormal,   .active = false},
#endif
#ifdef TASK_COLLECT_TELEMETRY
  {.id = TASK_COLLECT_TELEMETRY,  .name = "Collect Telemetry",  .func = task_collect_telemetry,  .args = NULL, .priority = osPriorityNormal,   .active = true},
#endif
#ifdef TASK_STREAM_TELEMETRY
  {.id = TASK_STREAM_TELEMETRY,   .name = "Stream Telemetry",   .func = task_stream_telemetry,   .args = NULL, .priority = osPriorityNormal,   .active = true},
#endif
#ifdef TASK_CALIBRATE_CHANNELS
  {.id = TASK_CALIBRATE_CHANNELS, .name = "Calibrate Channels", .func = task_calibrate_channels, .args = NULL, .priority = osPriorityNormal,   .active = false}
#endif
};

#endif  // INCLUDE_TASKS_H_
