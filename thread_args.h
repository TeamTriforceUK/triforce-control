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

typedef struct {

  state_t state;


  bool active;
  bool armed;
  bool just_armed;
  bool failsafe;
  bool inverted;

  PwmIn *rc_channel[RC_NUMBER_CHANNELS];

  Serial *serial;

  struct rc_controls controls;

  struct rc_outputs outputs;

  struct direction_vector direction;

  struct {
    ESC *drive[3];
    ESC *weapon[2];
  } escs;

  euler_t orientation;

  struct {
    Mutex *speed;
    Mutex *accel;
    Mutex *engine;
    Mutex *odometer;
  } mutex;


  DigitalOut *leds[4];


  bool heading_lock_enabled;
  float heading_lock; // heading to rotate to
  int heading_lock_speed; // percentage
  int heading_lock_deadband; // degrees / 2

} thread_args_t;


#endif //TC_THREAD_ARGS_H
