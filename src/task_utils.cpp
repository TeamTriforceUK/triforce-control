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
 *
 * @file task_utils.cpp
 * @author Cameron A. Craig
 * @date 9 Sep 2017
 * @copyright 2017 Cameron A. Craig
 * @brief These functions are used within tasks.cpp.
 */

#include "thread_args.h"
#include "tmath.h"
#include "comms.h"

void read_recv_pw(thread_args_t *args) {
  int controller, channel;
  float pw, v, min, max;

  for (controller = 0; controller < RC_NUMBER_CONTROLLERS; controller++) {
    for (channel = 0; channel < RC_NUMBER_CHANNELS; channel++) {
      // Read raw pulse width
      pw = args->receiver[controller].channel[channel]->pulsewidth();

      // Get min and max pulsewidths fot this channel
      min = args->channel_limits[controller][channel].min;
      max = args->channel_limits[controller][channel].max;

      // Make sure pw doesn't leave bounds due to imperfect calibration
      pw = clamp(pw, min, max);

      // Convert into float value between 0 and 100, based on max and min
      v = ( (pw - min) / (max - min) ) * 100.0f;

      // Set the control value for other threads to use
      args->mutex.controls->lock();
      args->controls[controller].channel[channel] = v;
      args->mutex.controls->unlock();

      // For debugging purposes
      // args->serial->printf("con %d chan %d: [pw: %.0f, min: %.0f, max: %.0f, v: %.0f]\r\n", controller, channel, pw, min, max, v);
    }
  }
}

void set_output_escs(thread_args_t *args) {
  /* No matter what drive mode we use, ensure outputs
     are within the valid range. */
  args->mutex.outputs->lock();
  args->outputs.wheel_1 = clamp(args->outputs.wheel_1, 0, 100);
  args->outputs.wheel_2 = clamp(args->outputs.wheel_2, 0, 100);
  args->outputs.wheel_3 = clamp(args->outputs.wheel_3, 0, 100);
  args->outputs.weapon_motor_1 = clamp(args->outputs.weapon_motor_1, 0, 100);
  args->outputs.weapon_motor_2 = clamp(args->outputs.weapon_motor_2, 0, 100);
  args->outputs.weapon_motor_3 = clamp(args->outputs.weapon_motor_3, 0, 100);
  args->mutex.outputs->unlock();

  /* Now that we have valid output parameters, we can set the ESCs. */
  args->mutex.outputs->lock();
    switch (args->state) {
      case STATE_FULLY_ARMED:
        args->comms_impl->set_speed(&args->escs.weapon[0], args->outputs.weapon_motor_1);
        args->comms_impl->set_speed(&args->escs.weapon[1], args->outputs.weapon_motor_2);
        args->comms_impl->set_speed(&args->escs.weapon[2], args->outputs.weapon_motor_3);
        args->comms_impl->set_speed(&args->escs.drive[0], args->outputs.wheel_1);
        args->comms_impl->set_speed(&args->escs.drive[1], args->outputs.wheel_2);
        args->comms_impl->set_speed(&args->escs.drive[2], args->outputs.wheel_3);
        break;
      case STATE_DRIVE_ONLY:
        args->comms_impl->set_speed(&args->escs.drive[0], args->outputs.wheel_1);
        args->comms_impl->set_speed(&args->escs.drive[1], args->outputs.wheel_2);
        args->comms_impl->set_speed(&args->escs.drive[2], args->outputs.wheel_3);
        args->comms_impl->stop(&args->escs.weapon[0]);
        args->comms_impl->stop(&args->escs.weapon[1]);
        args->comms_impl->stop(&args->escs.weapon[2]);
        break;
      case STATE_WEAPON_ONLY:
        args->comms_impl->set_speed(&args->escs.weapon[0], args->outputs.weapon_motor_1);
        args->comms_impl->set_speed(&args->escs.weapon[1], args->outputs.weapon_motor_2);
        args->comms_impl->set_speed(&args->escs.weapon[2], args->outputs.weapon_motor_3);
        args->comms_impl->stop(&args->escs.drive[0]);
        args->comms_impl->stop(&args->escs.drive[1]);
        args->comms_impl->stop(&args->escs.drive[2]);
        break;
      case STATE_DISARMED:
        args->comms_impl->stop(&args->escs.drive[0]);
        args->comms_impl->stop(&args->escs.drive[1]);
        args->comms_impl->stop(&args->escs.drive[2]);
        args->comms_impl->stop(&args->escs.weapon[0]);
        args->comms_impl->stop(&args->escs.weapon[1]);
        args->comms_impl->stop(&args->escs.weapon[2]);
    }
  args->mutex.outputs->unlock();
}
