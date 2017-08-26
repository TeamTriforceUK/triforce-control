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
* @file drive_mode.h
* @author Cameron A. Craig
* @date 25 Aug 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines structures used to store driving modes.
*/

#ifndef TC_DRIVE_MODE_H_
#define TC_DRIVE_MODE_H_

/* Drive */

typedef enum {
  DM_3_WHEEL_HOLONOMIC = 0,
  DM_2_WHEEL_DIFFERENTIAL
} drive_mode_id_t;

typedef struct {
  drive_mode_id_t id;
  const char *name;
  int wheels;
  void (*drive)(const void*);
} drive_mode_t;

/* Weapon */

typedef enum {
  WM_MANUAL_THROTTLE = 0
} weapon_mode_id_t;

typedef struct {
  weapon_mode_id_t id;
  const char *name;
  void (*weapon)(const void*);
} weapon_mode_t;

#endif //TC_DRIVE_MODE_H_
