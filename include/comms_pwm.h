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
 *
 * @file comms_pwm.h
 * @author Cameron A. Craig
 * @date 23 Sep 2017
 * @copyright 2017 Cameron A. Craig
 * @brief Implements PWM communication for ESC control.
 */

#ifndef TC_COMMS_PWM_H
#define TC_COMMS_PWM_H

#include "comms.h"

//Make sure that IDs are unique when adding new comms implememnations!
#define COMMS_IMPL_PWM 0

/**
* @brief Initialise classes required for use of PWM comms mode.
*/
void comms_impl_pwm_init_comms(void);

/**
* @brief Set speed of ESC.
*/
void comms_impl_pwm_set_speed(comms_esc_t *esc, uint32_t speed);

/**
* @brief Stop ESC
*/
void comms_impl_pwm_stop(comms_esc_t *esc);

#endif //TC_COMMS_PWM_H
