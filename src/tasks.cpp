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
* @file tasks.cpp
* @author Cameron A. Craig, Euan W. Mutch
* @date 15 May 2017
* @copyright 2017 Cameron A. Craig, Euan W. Mutch
* @brief Defines struct to share data between threads.
*/

#include "mbed.h"
#include "tasks.h"
#include "bno055.h"
#include "types.h"
#include "thread_args.h"
#include "tmath.h"
#include "states.h"
#include "utilc-logging.h"
#include "commands.h"
#include "return_codes.h"
#include "tele_params.h"

/**
* @brief Execute commands as they become available on the mail queue.
* @param [in/out] targs Thread arguments.
*/
void task_process_commands(const void *targs){
  LOG("Command processing task started\r\n");
  thread_args_t * args = (thread_args_t *) targs;
  while(args->active){
    osEvent evt;
    while((evt = args->command_queue->get(1)).status == osEventMail){
        command_t *command_q = (command_t*)evt.value.p;
        int err;
        if((err = command_execute(command_q, args)) != RET_OK){
          LOG("\rError: %s\r\n", err_to_str(err));
        } else {
          LOG("\rCommand succesful\r\n");
        }
        args->command_queue->free(command_q);
    }
  }
  LOG("Command processing task ended\r\n");
}

/**
* @brief Creates primative commmand line interface on serial port.
* @param [in/out] targs Thread arguments.
*/
void task_serial_commands_in(const void *targs){
  thread_args_t * args = (thread_args_t *) targs;

  char buffer[100];
  int pos = 0;
  LOG( "$");
  bool readable;
  while(args->active){
    bool readable = args->serial->readable();
    // LOG("test\r\n");
    if(readable){
      buffer[pos] = args->serial->getc();

      // If ENTER key is pressed, execute command
      if(buffer[pos] == '\r'){
        buffer[pos+1] = NULL;
        LOG( "\r\n");
        command_t command;
        //Generate a command structure for the command given
        if(!command_generate(&command, buffer)){
          LOG("\rCommand not recognised!\r\n");
        } else {
          command_t *command_q = args->command_queue->alloc();
          memcpy(command_q, &command, sizeof(command_t));
          args->command_queue->put(command_q);
        };

        pos = -1;
      }
      if(buffer[pos] == '\b'){
        buffer[pos] = NULL;
        pos--;
      }
      buffer[pos+1] = NULL;
      LOG("\r$ %s", buffer);
      pos++;
    }
  }
  LOG("Serial in task ended\r\n");
}


/**
* @brief Set mbed LED0 --> LED3 to represent the arming state.
* @param [in/out] targs Thread arguments.
*/
void task_state_leds(const void *targs){
  LOG("LED task started\r\n");
  thread_args_t * args = (thread_args_t *) targs;
  static state_t previous_state = args->state;
  bool first_time = true;
  bool weapon_only_ripple[4] = {true, false, false, false};
  bool tmp_ripple;
  while(args->active){
    if (args->state != previous_state || first_time){
      LOG( "state change: %s --> %s\r\n", state_to_str(previous_state), state_to_str(args->state));
      switch(args->state){
        case STATE_DISARMED:
          args->leds[0]->write(false);
          args->leds[1]->write(false);
          args->leds[2]->write(false);
          args->leds[3]->write(false);
          break;
        case STATE_DRIVE_ONLY:
          args->leds[0]->write(true);
          args->leds[1]->write(true);
          args->leds[2]->write(false);
          args->leds[3]->write(false);
          break;
        case STATE_WEAPON_ONLY:
          break;
        case STATE_FULLY_ARMED:
          args->leds[0]->write(true);
          args->leds[1]->write(true);
          args->leds[2]->write(true);
          args->leds[3]->write(true);
          break;
      }
    }

    /* The weapon only state is a special case where fancy LED strobing is used
       We need to do this outside the above switch statement, as we require a
       LED change every loop, rather than just on a state change.
    */
    if(args->state == STATE_WEAPON_ONLY){
      args->leds[0]->write(weapon_only_ripple[0]);
      args->leds[1]->write(weapon_only_ripple[1]);
      args->leds[2]->write(weapon_only_ripple[2]);
      args->leds[3]->write(weapon_only_ripple[3]);

      //Move LED along by one
      tmp_ripple = weapon_only_ripple[3];
      memmove(&weapon_only_ripple[1], &weapon_only_ripple[0], sizeof(bool)*3);
      weapon_only_ripple[0] = tmp_ripple;
    }

    previous_state = args->state;
    first_time = false;
    Thread::wait(100);
  }
  LOG("LED task ended\r\n");
}
/**
* @brief Convert receiver signals into commands and add to the command queue.
* @param [in/out] targs Thread arguments.
*/
void task_read_receiver(const void *targs) {
  thread_args_t * args = (thread_args_t *) targs;
  while(args->active){
    int controller, channel;
    for (controller = 0; controller < RC_NUMBER_CONTROLLERS; controller++){
      for (channel = 0; channel < RC_NUMBER_CHANNELS; channel++){
        args->controls[controller].channel[channel] = convert_pulsewidth(args->receiver[controller].channel[channel]->pulsewidth());
      }
    }
  }
}

/**
* @brief Change the arming state based on the arming switch.
* @param [in/out] targs Thread arguments.
*/
void task_arming(const void *targs) {
  thread_args_t * args = (thread_args_t *) targs;
  bool drive_switch, weapon_switch, drive_arm, weapon_arm;

  while (args->active) {
    drive_switch = (args->controls[0].channel[3] > RC_SWITCH_MIDPOINT);
    weapon_switch = (args->controls[1].channel[3] > RC_SWITCH_MIDPOINT);

    drive_arm = drive_switch &&
      BETWEEN(args->controls[0].channel[0], 45, 55) &&
      BETWEEN(args->controls[0].channel[1], 45, 55) &&
      (args->controls[0].channel[2] < RC_ARM_CHANNEL_3) &&
      BETWEEN(args->controls[0].channel[3], 45, 55);

    weapon_arm = weapon_switch &&
      BETWEEN(args->controls[1].channel[0], 45, 55) &&
      BETWEEN(args->controls[1].channel[1], 45, 55) &&
      (args->controls[1].channel[2] < RC_ARM_CHANNEL_3) &&
      BETWEEN(args->controls[1].channel[3], 45, 55);

    switch (args->state) {
      /* From the fully armed state we can only decrease the arm state,
      so we don't need to be concerned with the stick positions. */
      case STATE_FULLY_ARMED:
        if (!drive_switch && !weapon_switch) {
          args->state = STATE_DISARMED;
        } else if (drive_switch && !weapon_switch) {
          args->state = STATE_DRIVE_ONLY;
        } else if (!drive_switch && weapon_switch) {
          args->state = STATE_WEAPON_ONLY;
        }
        break;
      case STATE_DRIVE_ONLY:
        if (!drive_switch) {
          args->state = STATE_DISARMED;
        } else if (weapon_arm) {
          args->state = STATE_FULLY_ARMED;
        }
        break;
      case STATE_WEAPON_ONLY:
        if (!weapon_switch) {
          args->state = STATE_DISARMED;
        } else if (drive_arm) {
          args->state = STATE_FULLY_ARMED;
        }
        break;
      case STATE_DISARMED:
        if (drive_arm && weapon_arm) {
          args->state = STATE_FULLY_ARMED;
        } else if (drive_arm) {
          args->state = STATE_DRIVE_ONLY;
        } else if (weapon_arm) {
          args->state = STATE_WEAPON_ONLY;
        }
    }
  }
}

void task_failsafe(const void *targs) {
  thread_args_t * args = (thread_args_t *) targs;
  bool drive_inactive, weapon_inactive;
  while (args->active) {
    drive_inactive = args->receiver[0].channel[0]->stallTimer.read_ms() > 200;
    weapon_inactive = args->receiver[1].channel[0]->stallTimer.read_ms() > 200;

    switch (args->state) {
      case STATE_FULLY_ARMED:
        if (drive_inactive && weapon_inactive) {
          args->state = STATE_DISARMED;
        } else if (drive_inactive && !weapon_inactive) {
          args->state = STATE_WEAPON_ONLY;
        } else if (!drive_inactive && weapon_inactive) {
          args->state = STATE_DRIVE_ONLY;
        }
        break;
      case STATE_DRIVE_ONLY:
        if (drive_inactive) {
          args->state = STATE_DISARMED;
        }
        break;
      case STATE_WEAPON_ONLY:
        if (weapon_inactive){
          args->state = STATE_DISARMED;
        }
        break;
    }
  }
}

/** Output to OmniMixer and ESCs
 *
 * Sends calculated output values to devices
 */
void task_set_escs(const void *targs) {
    thread_args_t * args = (thread_args_t *) targs;

    switch (args->state) {
      case STATE_FULLY_ARMED:
        args->escs.weapon[0]->setThrottle(args->outputs.weapon_motor_1);
        args->escs.weapon[1]->setThrottle(args->outputs.weapon_motor_2);
      case STATE_DRIVE_ONLY:
        args->escs.drive[0]->setThrottle(args->outputs.wheel_1);
        args->escs.drive[1]->setThrottle(args->outputs.wheel_2);
        args->escs.drive[2]->setThrottle(args->outputs.wheel_3);
        break;
      case STATE_DISARMED:
        args->escs.drive[0]->failsafe();
        args->escs.drive[1]->failsafe();
        args->escs.drive[2]->failsafe();
        args->escs.weapon[0]->failsafe();
        args->escs.weapon[1]->failsafe();
    }
}


void task_calc_orientation(const void *targs) {
  thread_args_t * args = (thread_args_t *) targs;
  while (args->active) {
    /* If there is an error then we maintain the same
     * orientation to stop random control flipping */
    if (!bno055_healthy()) {
        LOG("ERROR: BNO055 has an error/status problem!!!\r\n");
    } else {
        /* Read in the Euler angles */
        args->orientation = bno055_read_euler_angles();

        /* We are upside down in range -30 -> -90
         * the sensor will report -60 when inverted */
        if (args->orientation.roll < -30 && args->orientation.roll > -90){
            args->inverted = true;
        } else {
            args->inverted = false;
        }
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        args->serial->printf("Inverted= %s \t (%7.2f) \r\n", args->inverted ? "true" : "false", orientation.roll);
        #endif
    }
  }
}

void task_collect_telemetry(const void *targs) {
  thread_args_t * args = (thread_args_t *) targs;
  euler_t e;
  int i;
  while (args->active) {
    for (i = 0; i < NUM_TELEM_PARAMS; i++) {
      switch (tele_commands[i].id) {
        case CID_RING_RPM:
          // TODO(camieac): Add support for RPM sensing
          tele_commands[i].param.f = 0.00f;
          break;
        case CID_CON_1_RPM:
          // TODO(camieac): Add support for RPM sensing
          tele_commands[i].param.f = 0.00f;
          break;
        case CID_CON_2_RPM:
          // TODO(camieac): Add support for RPM sensing
          tele_commands[i].param.f = 0.00f;
          break;
        case CID_ACCEL_X:
        case CID_ACCEL_Y:
        case CID_ACCEL_Z:
          e = bno055_read_accel();
          tele_commands[CID_ACCEL_X].param.f = e.x;
          tele_commands[CID_ACCEL_Y].param.f = e.y;
          tele_commands[CID_ACCEL_Z].param.f = e.z;
          // We do x, y and z in one op, so skip 2 once done
          if (i == CID_ACCEL_X) {
            i+=2;
          }
          break;
        case CID_PITCH:
        case CID_ROLL:
        case CID_YAW:
          e = bno055_read_euler_angles();
          tele_commands[CID_PITCH].param.f = e.pitch;
          tele_commands[CID_ROLL].param.f = e.roll;
          tele_commands[CID_YAW].param.f = e.heading;
          // We do x, y and z in one op, so skip 2 once done
          if (i == CID_PITCH) {
            i+=2;
          }
          break;
        case CID_WEAPON_VOLTAGE:
        case CID_DRIVE_VOLTAGE:
          break;
        case CID_AMBIENT_TEMP:
          tele_commands[i].param.i = bno055_read_temp();
          break;
        case CID_ESP_LED:
          //tele_commands[i].param.i = vi;
          break;
        default:
          args->serial->puts("UNSUPPORTED TELE COMMAND\r\n");
      }
    }
    wait(1);
  }
}

void task_stream_telemetry(const void *targs) {
  thread_args_t * args = (thread_args_t *) targs;
  args->serial->printf("Staring streaming task\r\n");

  int i = 0;
  while (args->active) {
    /* The ESP looks for a carriage return character to delimit a command. */

    for (i = 0; i < NUM_TELEM_PARAMS; i++) {
      switch (tele_commands[i].type) {
        case CT_FLOAT:
          args->esp_serial->printf(
            "%s %.2f\r",
            tele_commands[i].name,
            tele_commands[i].param.f);
        break;
        case CT_INT:
          args->esp_serial->printf(
            "%s %d\r",
            tele_commands[i].name,
            tele_commands[i].param.i);
          break;
        default:
          args->serial->printf("Type not yet supported for streaming.");
          break;
      }
    }
    wait(1);
  }
}
