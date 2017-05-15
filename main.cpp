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
* @file main.cpp
* @author Euan W. Mutch, Cameron A. Craig
* @date 13 May 2017
* @copyright 2017 Euan W. Mutch, Cameron A. Craig
* @brief Drives ESCs for Triforce fighting robot based on PWM signals from receiver.
* @mainpage Triforce Control
  @section intro_sec Introduction
  This documentation has been generated from the Doxygen comments within the source
  code of this project. This is a useful developers reference.

  @section install_sec Installation
     @code{.unparsed}
      git clone https://github.com/TeamTriforceUK/triforce-control.git
      cd triforce-control
      mbed deploy
      mbed compile -t GCC_ARM -m lpc1768
     @endcode
*/

/* Includes */
#include "mbed.h"
#include "esc.h"
#include "PwmIn.h"

#include "bno055.h"
#include "triforce.h"
#include "tmath.h"
#include "thread_args.h"
#include "config.h"
#include "tasks.h"


/** Init
 *
 * Sets up devices
 * Will loop init for bno055 untill it starts correctly.
 */
void init(){
    #ifdef ORIENTATION
    init_bno055:
    /* Initialise the sensor */
    if(!bno055_init()){

        /* There was a problem detecting the BNO055 ... check your connections */
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        //pc.printf("BNO055 not detected\r\n");
        #endif
        goto init_bno055;

    } else {
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        //pc.printf("BNO055 was detected!\r\n");
        #endif
    }
    #endif
}


/** Main
 *
 * Main Loop
 */
int main() {
    thread_args_t targs;
    memset(&targs, 0x00, sizeof(thread_args_t));

    /* USB */
    Serial serial(USBTX, USBRX);
    targs.serial = &serial;

    /* 8 Channel RC input */
    PwmIn rc_channel_1(RECV_CHAN_1_PIN); // Roll
    targs.rc_channel[0] = &rc_channel_1;

    PwmIn rc_channel_2(RECV_CHAN_2_PIN); // Pitch
    targs.rc_channel[1] = &rc_channel_2;

    PwmIn rc_channel_3(RECV_CHAN_3_PIN); // Throttle
    targs.rc_channel[2] = &rc_channel_3;

    PwmIn rc_channel_4(RECV_CHAN_4_PIN); // Yaw
    targs.rc_channel[3] = &rc_channel_4;

    PwmIn rc_channel_5(RECV_CHAN_5_PIN); // Switch R front
    targs.rc_channel[4] = &rc_channel_5;

    PwmIn rc_channel_6(RECV_CHAN_6_PIN); // Dial R
    targs.rc_channel[5] = &rc_channel_6;

    PwmIn rc_channel_7(RECV_CHAN_7_PIN); // Dial L
    targs.rc_channel[6] = &rc_channel_7;

    PwmIn rc_channel_8(RECV_CHAN_8_PIN); // Switch L Back
    targs.rc_channel[7] = &rc_channel_8;

    /* 5 channel ESC output */
    ESC esc_omni_1(DRIVE_ESC_OUT_1_PIN, 20, 1500);
    targs.escs.drive[0] = &esc_omni_1;

    ESC esc_omni_2(DRIVE_ESC_OUT_2_PIN, 20, 1500);
    targs.escs.drive[1] = &esc_omni_2;

    ESC esc_omni_3(DRIVE_ESC_OUT_3_PIN, 20, 1500); //LED4
    targs.escs.drive[2] = &esc_omni_3;

    ESC esc_weapon_1(WEAPON_ESC_OUT_1_PIN); //LED3
    targs.escs.weapon[0] = &esc_weapon_1;

    ESC esc_weapon_2(WEAPON_ESC_OUT_2_PIN); //LED2
    targs.escs.weapon[1] = &esc_weapon_2;

    /* LEDs */
    DigitalOut led1(LED1);

    #ifdef PC_DEBUGGING
    targs.serial->baud(115200);
    targs.serial->printf("Triforce Control System \r\n");
    #endif

    init();

    Thread thread_read_receiver;
    thread_read_receiver.start(callback(task_read_receiver, (void *) &targs));

    Thread thread_set_escs;
    thread_read_receiver.start(callback(task_set_escs, (void *) &targs));

    // Thread thread_calc_escs;
    // thread_read_receiver.start(callback(task_calc_escs, (void *) &targs));

    Thread thread_calc_orientation;
    thread_read_receiver.start(callback(task_calc_orientation, (void *) &targs));

    thread_read_receiver.join();
    thread_set_escs.join();
    // thread_calc_escs.join();
    thread_calc_orientation.join();

}
