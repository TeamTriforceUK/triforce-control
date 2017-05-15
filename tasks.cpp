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
#include "triforce.h"
#include "tmath.h"
#include "states.h"



void task_state_leds(const void *targs){
  thread_args_t * args = (thread_args_t *) targs;
  while(args->active){
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
}
 /** Read control positions from RC receiver
 *
 * Converts control input pulsewidths read in seconds to throttle %
 */
void task_read_receiver(const void *targs) {
    thread_args_t * args = (thread_args_t *) targs;
    static int arming_count = 0;
    static int failsafe_timer = 0;

    /* Check for armed status */


        if(!args->armed || args->just_armed){
            #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
            if(arming_count == 0){

                args->serial->printf("MOVE STICKS TO ARMING POSITIONS AND HOLD TO ARM \r\n");
            }
            #endif

            args->failsafe = true;
            int c1 = convert_pulsewidth(args->rc_channel[0]->pulsewidth());
            int c2 = convert_pulsewidth(args->rc_channel[1]->pulsewidth());
            int c3 = convert_pulsewidth(args->rc_channel[2]->pulsewidth());
            int c4 = convert_pulsewidth(args->rc_channel[3]->pulsewidth());
            args->serial->printf("Input: C1: %i \t C2: %i \t C3: %i \t C4: %i \r\n", c1, c2, c3, c4);

            if(c1 > RC_ARM_CHANNEL_1 && c2 > RC_ARM_CHANNEL_2 && c3 < RC_ARM_CHANNEL_3 && c4 < RC_ARM_CHANNEL_4){

                if(!args->armed){
                    #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                    args->serial->printf("ARMING %i\r\n", arming_count);
                    #endif
                    arming_count++;
                    wait_ms(1000);
                    if(arming_count > RC_ARM_DELAY){
                        args->armed = true;
                        args->just_armed = true;
                    }
                }else{
                    #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                    args->serial->printf("ARMED - CENTER CONTROLS \r\n");
                    #endif
                }
            } else if(args->just_armed){
                /* Wait for controls to be centered to avoid unexpected movement */
                if(BETWEEN(c1, 45, 55) && BETWEEN(c2, 45, 55) && c3 < RC_ARM_CHANNEL_3 && BETWEEN(c4, 45, 55)){
                    #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                    args->serial->printf("CONTROLS CENTERED DISABLING ARM LOCK\r\n");
                    #endif
                    args->just_armed = false;
                    failsafe_timer = 0;
                }

            }else {
                arming_count = 0;
            }


        }else{

            #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
            args->serial->printf("******************ARMED****************** \r\n");
            #endif

            args->controls.channel_1 = convert_pulsewidth(args->rc_channel[0]->pulsewidth());
            args->controls.channel_2 = convert_pulsewidth(args->rc_channel[1]->pulsewidth());
            args->controls.channel_3 = convert_pulsewidth(args->rc_channel[2]->pulsewidth());
            args->controls.channel_4 = convert_pulsewidth(args->rc_channel[3]->pulsewidth());
            args->controls.channel_5 = convert_pulsewidth(args->rc_channel[4]->pulsewidth());
            args->controls.channel_6 = convert_pulsewidth(args->rc_channel[5]->pulsewidth());
            args->controls.channel_7 = convert_pulsewidth(args->rc_channel[6]->pulsewidth());
            args->controls.channel_8 = convert_pulsewidth(args->rc_channel[7]->pulsewidth());

            /* Check if rc is stalled and failsafe output */

            if(args->rc_channel[0]->stallTimer.read_ms() > 200){
                args->failsafe = true;
                #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                args->serial->printf("NO SIGNAL - FAILSAFE ACTIVATED \r\n");
                #endif
                failsafe_timer++;
                wait_ms(100);
                if(failsafe_timer > NO_SIGNAL_TIMEOUT){
                    args->armed = false;
                }
            }else{
                failsafe_timer = 0;
                args->failsafe = false;
            }

            args->direction.rotation = args->controls.channel_4;
            args->direction.x_translation = args->controls.channel_2;
            args->direction.y_translation = args->controls.channel_1;

            /* Heading lock enable switch */
            if(args->controls.channel_8 > 75 && !args->heading_lock_enabled){
                args->heading_lock = args->orientation.heading;
                args->heading_lock_enabled = true;
            }else if(args->controls.channel_8 < 75 && args->heading_lock_enabled){
                args->heading_lock_enabled = false;
            }

            /* Speed for heading lock */
            args->heading_lock_speed = (int) args->controls.channel_7;

            #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
            args->serial->printf("Input: C1: %i \t C2: %i \t C3: %i \t C4: %i \t C5: %i \t C6: %i \t C7: %i \t C8: %i\r\n", args->controls.channel_1, args->controls.channel_2, args->controls.channel_3, args->controls.channel_4, args->controls.channel_5, args->controls.channel_6, args->controls.channel_7, args->controls.channel_8);

            args->serial->printf("Heading Lock: %s \r\n", args->heading_lock_enabled ? "true" : "false");
            args->serial->printf("Heading Lock Speed: %i \r\n", args->heading_lock_speed);
            #endif
       }
}

/** Output to OmniMixer and ESCs
 *
 * Sends calculated output values to devices
 */
void task_set_escs(const void *targs) {
    thread_args_t * args = (thread_args_t *) targs;
    if(!args->failsafe){
    //outputs.wheel_1 -= 18;
    //outputs.wheel_2 -= 27;
    //outputs.wheel_3 -= 14;

    args->escs.drive[0]->setThrottle(args->outputs.wheel_1);
    args->escs.drive[1]->setThrottle(args->outputs.wheel_2);
    args->escs.drive[2]->setThrottle(args->outputs.wheel_3);
    //esc_weapon_1.setThrottle(outputs.weapon_motor_1);
    //esc_weapon_2.setThrottle(outputs.weapon_motor_2);
    } else {
        args->escs.drive[0]->failsafe();
        args->escs.drive[1]->failsafe();
        args->escs.drive[2]->failsafe();
    }

    #if defined (PC_DEBUGGING) && defined (DEBUG_OUTPUT)
    args->serial->printf("OUTPUT M1: %i \t M2: %i \t M3: %i \t W1: %i \t W2: %i \r\n", args->outputs.wheel_1, args->outputs.wheel_2, args->outputs.wheel_3, args->outputs.weapon_motor_1, args->outputs.weapon_motor_2);
    #endif
}

// void task_calc_escs(const void *targs) {
//     thread_args_t * args = (thread_args_t *) targs;
// }
/** Calculate Orientation
 *
 * Calculates orientation using the BNO055 sensor
 */
void task_calc_orientation(const void *targs){
  thread_args_t * args = (thread_args_t *) targs;

      /* If there is an error then we maintain the same
       * orientation to stop random control flipping */
      if(!bno055_healthy()){
          #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
          args->serial->printf("ERROR: BNO055 has an error/status problem!!!\r\n");
          #endif
      }else{

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
