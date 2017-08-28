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
* @file thread_args.h
* @author Cameron A. Craig
* @date 15 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines thread_args_t structure.
*/

#ifndef TC_THREAD_ARGS_H
#define TC_THREAD_ARGS_H

#include "mbed.h"
#include "rtos.h"
#include "types.h"
#include "config.h"
#include "esc.h"
#include "PwmIn.h"
#include "states.h"
#include "bno055.h"
#include "command.h"
#include "task.h"
#include "utilc-logging.h"
#include "drive_mode.h"

typedef struct {

  task_t *tasks;
  Thread *threads;

  state_t state;

  /*! Drive mode in use. */
  drive_mode_t *drive_mode;

  /*! Weapon mode in use */
  weapon_mode_t *weapon_mode;

  /*! Channel values for multiple controllers. */
  rc_controls_t controls[RC_NUMBER_CONTROLLERS];

  /*! Receiver inputs. */
  rc_receiver_t receiver[RC_NUMBER_CONTROLLERS];

  /*! Defines the upper and lower limits of each PWM input from a receiver. */
  channel_limits_t channel_limits[RC_NUMBER_CONTROLLERS][RC_NUMBER_CHANNELS];

  /*! Mail queue for commands from serial comms or RF RX */
  Mail<command_t, COMMAND_QUEUE_LEN> *command_queue;

  /*! USB serial port */
  Serial *serial;

  /*! Serial connection to ESP8266 */
  Serial *esp_serial;
  DigitalIn *esp_ready_pin;

  /*! Logger structure */
  struct ucl_s logger;

  /*! Stores telemetry values */
  /* TODO(camieac): Telemetry parameters should only be accessible
      through thread_args_t. */

  struct rc_outputs outputs;

  struct direction_vector direction;

  struct {
    ESC *drive[3];
    ESC *weapon[3];
  } escs;

  orientation_t orientation_detected;
  orientation_t orientation_override;
  euler_t orientation;
  bool inverted;
  bool active;

  /* These mutexes protect accesses to the fields within thread_args_t.
  */
  struct {
    Mutex *pc_serial;
    Mutex *controls;
    Mutex *outputs;
  } mutex;


  DigitalOut *leds[4];


  bool heading_lock_enabled;
  float heading_lock; // heading to rotate to
  int heading_lock_speed; // percentage
  int heading_lock_deadband; // degrees / 2

} thread_args_t;

void thread_args_init(thread_args_t *args);


#endif //TC_THREAD_ARGS_H
