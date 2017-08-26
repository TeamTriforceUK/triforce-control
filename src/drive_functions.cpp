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
* @file drive_functions.cpp
* @author Cameron A. Craig
* @date 26 Aug 2017
* @copyright 2017 Cameron A. Craig
* @brief Provides various implementations for the different drive and weapon modes.
*/

#include "drive_functions.h"
#include "config.h"
#include "thread_args.h"
#include "tmath.h"

void drive_3_wheel_holonomic(const void * targs) {
  thread_args_t *args = (thread_args_t*) targs;
  args->mutex.controls->lock();
  float x = args->controls[1].channel[RC_1_AILERON] - 50.0f;
  float y = args->controls[1].channel[RC_1_ELEVATION] - 50.0f;
  args->mutex.controls->unlock();

  float theta = (float)atan2((double)x, (double)y);
  float magnitude = (float)sqrt((double)((x*x)+(y*y)));

  if(magnitude > 5.0f) {

      float vx = magnitude * sin(theta);
      float vy = magnitude * cos(theta);
      const float sqrt3o2 = 1.0*sqrt(3.0)/2.0;

      float w0 = -vx;                   // v dot [-1, 0] / 25mm
      float w1 = 0.5*vx - sqrt3o2 * vy; // v dot [1/2, -sqrt(3)/2] / 25mm
      float w2 = 0.5*vx + sqrt3o2 * vy; // v dot [1/2, +sqrt(3)/2] / 25mm
      // #if defined (PC_DEBUGGING) && defined (DEBUG_CONTROLS)
      // pc.printf("Calculated Controls: (%7.2f) \t (%7.2f) \t (%7.2f) \r\n", w0, w1, w2);
      // #endif
      float w0_speed =  map(w0, -70, 70, 0, 100);
      float w1_speed =  map(w1, -70, 70, 0, 100);
      float w2_speed =  map(w2, -70, 70, 0, 100);

      /* Add in rotation */
      // #if defined (PC_DEBUGGING) && defined (DEBUG_CONTROLS)
      // pc.printf("Mapped Controls: (%7.2f) \t (%7.2f) \t (%7.2f) \r\n", w0_speed, w1_speed, w2_speed);
      // #endif
      args->mutex.outputs->lock();
      args->outputs.wheel_1 += w0_speed -50;
      args->outputs.wheel_2 += w1_speed -50;
      args->outputs.wheel_3 += w2_speed -50;
      args->mutex.outputs->unlock();


  } else {
      args->mutex.outputs->lock();
      args->outputs.wheel_1 = 50;
      args->outputs.wheel_2 = 50;
      args->outputs.wheel_3 = 50;
      args->mutex.outputs->unlock();
  }

  args->mutex.controls->lock();
  float rudder_ctrl_val = args->controls[1].channel[RC_1_RUDDER] - 50;
  args->mutex.controls->unlock();

  args->mutex.outputs->lock();
  args->outputs.wheel_1 += rudder_ctrl_val;
  args->outputs.wheel_2 += rudder_ctrl_val;
  args->outputs.wheel_3 += rudder_ctrl_val;
  args->mutex.outputs->unlock();
}
/**
* @brief <brief>
* @param [in] targs Thread args.
* @details Alright, here's the plan:
*    Right stick y(ELEVATION) for throttle.
*    Right stick x(AILERON) for steering.
*    As we have two fixed wheels pointing in the same direction, we steer by
*    reducing the speed of the wheel in the direction you want to turn.
*    With 0 throttle and full steering, we should spin on the spot at max speed.
*    With full throttle and 0 steering, we should drive straight(ish) at max speed.
*/
void drive_2_wheel_differential(const void * targs) {
  thread_args_t *args = (thread_args_t*) targs;
  float throttle, steering, left_wheel, right_wheel;

  args->mutex.controls->lock();
  /* Channel values are between 0 and 100. */
  throttle = args->controls[1].channel[RC_1_ELEVATION] - 50;//args->controls[1].channel[RC_1_ELEVATION];
  /* Get steering value between -50 (full left) and +50 (full right). */
  steering = args->controls[1].channel[RC_1_AILERON] - 50.0f;
  args->mutex.controls->unlock();

  /* Spin on the spot when throttle is ~zero */
  if(BETWEEN(throttle, 49, 51)) {
    left_wheel = steering;
    right_wheel = -steering;

  /*  When throttle is applied, wheels only move in the direction of travel. */
  } else {
    /* Output values to the motors must be between 0 and 100. */
    left_wheel = throttle;
    right_wheel = throttle;
    //TODO: Will need to handle reversing once we have revesible ESCs for testing
    // If we want to turn right
    if(steering > 0) {
      left_wheel -= (abs(steering) / 50.0f) * throttle;
    // If we want to turn left
    } else if (steering < 0) {
      right_wheel -= (abs(steering) / 50.0f) * throttle;
    }
  }

  args->mutex.outputs->lock();
  args->outputs.wheel_1 = left_wheel;
  args->outputs.wheel_2 = right_wheel;
  args->mutex.outputs->unlock();
}

void weapon_manual_throttle(const void * targs) {
  thread_args_t *args = (thread_args_t*) targs;
  float weapon_ctrl_val;
  args->mutex.controls->lock();
  weapon_ctrl_val = args->controls[0].channel[RC_0_THROTTLE];
  args->mutex.controls->unlock();

  args->mutex.outputs->lock();
  args->outputs.weapon_motor_1 = weapon_ctrl_val;
  args->outputs.weapon_motor_2 = weapon_ctrl_val;
  args->outputs.weapon_motor_3 = weapon_ctrl_val;
  args->mutex.outputs->unlock();
}
