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

#ifndef TRIFORCE_H
#define TRIFORCE_H

#include "tmath.h"

/******************************************************************************/

/* Debugging defines */
#define DEBUG_INPUT  // Send input values to usb serial
//#define DEBUG_ORIENTATION // Send orientation readings to usb serial
//#define DEBUG_HEADING_LOCK // Send heading lock values to usb serial
//#define DEBUG_CONTROLS // Send calculated controls to usb serial
#define DEBUG_OUTPUT // Send output values to usb serial
#define PC_DEBUGGING // Output debugging information over usb serial

/* Configuration defines */
#define HEADING_LOCK // Enable heading lock functionality
#define ORIENTATION // Enable orientation functionality
#define OMNI_MIXER // Enable omni mixer functionality

#define channel_max 85 //85
#define channel_min 3  //3
#define RC_ON_THRES 70 // ON threshold for switches
#define RC_OFF_THRES 30 // OFF threshold for switches
#define RC_FAILSAFE_THRES 150 // FAILSAFE threshold


/******************************************************************************/

#include "math.h"

/******************************************************************************/

/* Positions of controls */
struct rc_controls{
    int channel_1;
    int channel_2;
    int channel_3;
    int channel_4;
    int channel_5;
    int channel_6;
    int channel_7;
    int channel_8;
};

/* Current movement command */
struct direction_vector{
    float rotation;
    float x_translation;
    float y_translation;
};

/* Outputs for devices */
struct rc_outputs{
    int wheel_1;
    int wheel_2;
    int wheel_3;
    int weapon_motor_1;
    int weapon_motor_2;
};

/* Ring definition */
struct ring{
    const int max_rpm;
    int rpm;
    const int max_energy;
    float energy;
};

/* Battery definition */
struct battery{
    int capacity;
    int used_capacity;
    const float max_voltage;
    float voltage;
    const int max_amps;
    int amps;
    const int max_temperature;
    int temperature;
};

/* Motor definition */
struct motor{
    const int max_rpm;
    int rpm;
    const int max_power;
    int power;
    const float max_voltage;
    float voltage;
    const float max_amps;
    float amps;
    const int max_temperature;
    int temperature;
};

/* Distance sensor definition */
struct distance_sensor{
    int id;
    const float max_range;
    float range;
};

/******************************************************************************/

/* Convert from pulsewidth in seconds, to % */
int convert_pulsewidth(float pulsewidth){
  //return(int) ((pulsewidth -1000)  / 10.0f);
    float value = (float) ((pulsewidth -1000)  / 10.0f);
    int pulse = (((value-channel_min)/channel_max) * 100.0);
    return clamp(pulse, 0, 100);
}

float normalize(float heading){
    while (heading > 180)
        heading -= 360;
    while (heading < -180)
        heading += 360;
    return heading;
}

/******************************************************************************/

/* Orientation */
Euler orientation;

/* Is the robot inverted */
bool inverted = false;

bool heading_lock_enabled = false;
float heading_lock = 0; // heading to rotate to
int heading_lock_speed = 10; // percentage
int heading_lock_deadband = 10; // degrees / 2

bool failsafe = false;
const int NO_SIGNAL_TIMEOUT = 70; // 150ms increments
int failsafe_timer = 0;

bool armed = false;
bool just_armed = false;

int arming_count = 0;
const int RC_ARM_CHANNEL_1 = 90;
const int RC_ARM_CHANNEL_2 = 90;
const int RC_ARM_CHANNEL_3 = 10;
const int RC_ARM_CHANNEL_4 = 10;
const int RC_ARM_DELAY = 5;


/******************************************************************************/

#endif
