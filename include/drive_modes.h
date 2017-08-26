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
* @file drive_modes.h
* @author Cameron A. Craig
* @date 25 Aug 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines driving modes.
*/

#ifndef TC_DRIVE_MODES_H_
#define TC_DRIVE_MODES_H_

#include "drive_mode.h"
#include "drive_functions.h"

/* Drive */

/* The various drive configurations are available in docs/drive_modes. */
static volatile drive_mode_t drive_modes[] = {
  {.id = DM_3_WHEEL_HOLONOMIC, .name = "3-Wheel Holonomic Drive", .wheels = 3, .drive = drive_3_wheel_holonomic },
  {.id = DM_2_WHEEL_DIFFERENTIAL, .name = "2-Wheel Differential Drive", .wheels = 2, .drive = drive_2_wheel_differential }
};

/* Weapon */

static volatile weapon_mode_t weapon_modes[] = {
  {.id = WM_MANUAL_THROTTLE, .name = "Manual Throttle", .weapon = weapon_manual_throttle }
};


#endif //TC_DRIVE_MODE_H_
