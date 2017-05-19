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
#include "logging.h"
#include "commands.h"
#include "return_codes.h"

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
  LOG("Serial in task started \r\n");
  thread_args_t * args = (thread_args_t *) targs;

  char buffer[100];
  int pos = 0;
  LOG( "$");

  while(args->active){
    if(args->serial->readable()){
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
        case STATE_FULLY_ARMED:
          args->leds[0]->write(true);
          args->leds[1]->write(true);
          args->leds[2]->write(true);
          args->leds[3]->write(true);
          break;
      }
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
    args->controls.channel_1 = convert_pulsewidth(args->rc_channel[0]->pulsewidth());
    args->controls.channel_2 = convert_pulsewidth(args->rc_channel[1]->pulsewidth());
    args->controls.channel_3 = convert_pulsewidth(args->rc_channel[2]->pulsewidth());
    args->controls.channel_4 = convert_pulsewidth(args->rc_channel[3]->pulsewidth());
    args->controls.channel_5 = convert_pulsewidth(args->rc_channel[4]->pulsewidth());
    args->controls.channel_6 = convert_pulsewidth(args->rc_channel[5]->pulsewidth());
    args->controls.channel_7 = convert_pulsewidth(args->rc_channel[6]->pulsewidth());
    args->controls.channel_8 = convert_pulsewidth(args->rc_channel[7]->pulsewidth());
  }
}
/**
* @brief Change the arming state based on the arming switch.
* @param [in/out] targs Thread arguments.
*/
void task_arming(const void *targs){
  thread_args_t * args = (thread_args_t *) targs;
  while(args->active){
    switch(args->state){
      case STATE_FULLY_ARMED:
      case STATE_DRIVE_ONLY:
      case STATE_DISARMED:
        //Channel 4 should be configured on the transmitter as the front right switch
        //This switch is used for arming
        if(args->controls.channel_4 > RC_SWITCH_MIDPOINT){
          args->state = STATE_FULLY_ARMED;
        } else {
          args->state = STATE_DISARMED;
        }
        break;
    }
  }
}

void task_failsafe(const void *targs){
  thread_args_t * args = (thread_args_t *) targs;
  while(args->active){
    switch(args->state)
      case STATE_FULLY_ARMED:
      case STATE_DISARMED:
        if(args->rc_channel[0]->stallTimer.read_ms() > 200){
          args->state = STATE_DISARMED;
        }
        break;
  }
}

/** Output to OmniMixer and ESCs
 *
 * Sends calculated output values to devices
 */
void task_set_escs(const void *targs) {
    thread_args_t * args = (thread_args_t *) targs;

    switch(args->state){
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


void task_calc_orientation(const void *targs){
  thread_args_t * args = (thread_args_t *) targs;
  while(args->active){
    /* If there is an error then we maintain the same
     * orientation to stop random control flipping */
    if(!bno055_healthy()){
        LOG("ERROR: BNO055 has an error/status problem!!!\r\n");
    } else {
        /* Read in the Euler angles */
        args->orientation = bno055_read_euler_angles();

        /* We are upside down in range -30 -> -90
         * the sensor will report -60 when inverted */
        if(args->orientation.roll < -30 && args->orientation.roll > -90){
            args->inverted = true;
        }else{
            args->inverted = false;
        }
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        args->serial->printf("Inverted= %s \t (%7.2f) \r\n", args->inverted ? "true" : "false", orientation.roll);
        #endif
    }
  }
}
