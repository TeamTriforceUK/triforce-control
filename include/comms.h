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
* @file comms.h
* @author Cameron A. Craig
* @date 23 Sep 2017
* @copyright 2017 Cameron A. Craig
* @brief Generic structure to offer many communcation methods within
*        the single structure.
*/

#ifndef TC_COMMS_H
#define TC_COMMS_H

#include "stdint.h"

#define COMMS_OUTPUT_DRIVE_1 0U
#define COMMS_OUTPUT_DRIVE_2 1U
#define COMMS_OUTPUT_DRIVE_3 2U
#define COMMS_OUTPUT_WEAPON_1 3U
#define COMMS_OUTPUT_WEAPON_2 4U
#define COMMS_OUTPUT_WEAPON_3 5U

typedef uint32_t comms_esc_id_t;

typedef struct {
  comms_esc_id_t id;
  const char *str;
} comms_esc_t;

typedef uint32_t comms_impl_id_t;

typedef struct {
  comms_impl_id_t impl_id;
  const char *str;
  void (*init_comms)();
  void (*init_esc)(comms_esc_t *esc, comms_esc_id_t id);
  void (*set_speed)(comms_esc_t *esc, uint32_t speed);
  void (*get_speed)(const void*);
  void (*get_status)(const void*);
  void (*stop)(comms_esc_t *esc);
} comms_impl_t;

#endif //TC_COMMS_PWM_H
