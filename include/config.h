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
* @file config.h
* @author Cameron A. Craig
* @date 15 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Configure pins and other parameters.
*/

#define VERSION "0.2.0"

#ifndef TC_CONFIG_H
#define TC_CONFIG_H

/* The following macros are used to enable and disable tasks at
   preprocessing time. */

// #define TASK_READ_SERIAL
// #define TASK_PROCESS_COMMANDS
#define TASK_LED_STATE
#define TASK_MOTOR_DRIVE
#define TASK_ARMING
#define TASK_FAILSAFE
#define TASK_CALC_ORIENTATION
#define TASK_COLLECT_TELEMETRY
#define TASK_STREAM_TELEMETRY
#define TASK_CALIBRATE_CHANNELS

/* Pin Assignments */
#define RECV_D_CHAN_1_PIN p5
#define RECV_D_CHAN_2_PIN p6
#define RECV_D_CHAN_3_PIN p7
#define RECV_D_CHAN_4_PIN p8
#define RECV_D_CHAN_5_PIN p11
#define RECV_D_CHAN_6_PIN p12

#define RECV_W_CHAN_1_PIN p13
#define RECV_W_CHAN_2_PIN p14
#define RECV_W_CHAN_3_PIN p15
#define RECV_W_CHAN_4_PIN p16
#define RECV_W_CHAN_5_PIN p17
#define RECV_W_CHAN_6_PIN p18

#define DRIVE_ESC_OUT_1_PIN p21
#define DRIVE_ESC_OUT_2_PIN p22
#define DRIVE_ESC_OUT_3_PIN p23

#define WEAPON_ESC_OUT_1_PIN p24
#define WEAPON_ESC_OUT_2_PIN p25
#define WEAPON_ESC_OUT_3_PIN p26 // Extra pin if we add 3rd motor

#define ESP_TX p28
#define ESP_RX p27

#define ESP8266_READY_PIN p29

/* End of Pin Assignments */

#define RC_NUMBER_CHANNELS 6
#define RC_NUMBER_CONTROLLERS 2

// 150ms increments
#define NO_SIGNAL_TIMEOUT 70

#define RC_ARM_CHANNEL_1 90
#define RC_ARM_CHANNEL_2 90
#define RC_ARM_CHANNEL_3 10
#define RC_ARM_CHANNEL_4 10
#define RC_ARM_DELAY 5

#define CHANNEL_MAX 85 //85
#define CHANNEL_MIN 3  //3
#define RC_ON_THRES 70 // ON threshold for switches
#define RC_OFF_THRES 30 // OFF threshold for switches
#define RC_FAILSAFE_THRES 150 // FAILSAFE threshold
#define RC_SWITCH_MIDPOINT 50.0f


#define COMMAND_QUEUE_LEN 100

#define MAIL_TIMEOUT_MS 1

#define NUM_SURFACE_LEDS 4

// Default channel limits (RC0/Weapon)
#define RC_0_CHAN_1_MIN   1069.0f
#define RC_0_CHAN_1_MAX   1895.0f

#define RC_0_CHAN_2_MIN   1043.0f
#define RC_0_CHAN_2_MAX   1871.0f

#define RC_0_CHAN_3_MIN   1014.0f
#define RC_0_CHAN_3_MAX   1839.0f

#define RC_0_CHAN_4_MIN   1030.0f
#define RC_0_CHAN_4_MAX   1854.0f

#define RC_0_CHAN_5_MIN   1030.0f
#define RC_0_CHAN_5_MAX   1857.0f

#define RC_0_CHAN_6_MIN   1041.0f
#define RC_0_CHAN_6_MAX   1857.0f

// Default channel limits (RC1/Drive)
#define RC_1_CHAN_1_MIN   1202.0f
#define RC_1_CHAN_1_MAX   1896.0f

#define RC_1_CHAN_2_MIN   1092.0f
#define RC_1_CHAN_2_MAX   1896.0f

#define RC_1_CHAN_3_MIN   1092.0f
#define RC_1_CHAN_3_MAX   1895.0f

#define RC_1_CHAN_4_MIN   1093.0f
#define RC_1_CHAN_4_MAX   1896.0f

#define RC_1_CHAN_5_MIN   1092.0f
#define RC_1_CHAN_5_MAX   1892.0f

#define RC_1_CHAN_6_MIN   1092.0f
#define RC_1_CHAN_6_MAX   1894.0f

//Define purpose of each channel for readability


#define RC_0_AILERON      0
#define RC_0_ELEVATION    1
#define RC_0_THROTTLE     2
#define RC_0_RUDDER       3
#define RC_0_ARM_SWITCH   4 // Channel 5 [0 --> 5]

#define RC_1_AILERON      1
#define RC_1_ELEVATION    2
#define RC_1_THROTTLE     0
#define RC_1_RUDDER       3
#define RC_1_ARM_SWITCH   4 // Channel 5 [0 --> 5]


#endif //TC_CONFIG_H
