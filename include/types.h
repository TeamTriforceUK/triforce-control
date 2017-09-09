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
* @file types.h
* @author Cameron A. Craig
* @date 9 Sep 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines various structs, used throughout the project.
*
*/

#ifndef TC_TYPES_H
#define TC_TYPES_H

#include "PwmIn.h"
#include "config.h"

/* Positions of controls */
typedef struct {
    float channel[RC_NUMBER_CHANNELS];
} rc_controls_t;

typedef struct {
  PwmIn *channel[RC_NUMBER_CHANNELS];
} rc_receiver_t;

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
    int weapon_motor_3;
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

enum orientation_t {
  UPRIGHT = 0,
  INVERTED
};

typedef struct {
  float min;
  float max;
} channel_limits_t;

const char * orientation_to_str(orientation_t orientation);

#endif //TC_TYPES_H
