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
* @file utils.cpp
* @author Cameron A. Craig
* @date 7 Jun 2017
* @copyright 2017 Cameron A. Craig
* @brief Useful utility functions.
*/

#include "utils.h"

bool is_drive_stalled(thread_args_t *args){
  /* We have averted an extremely dangerous situation by also checking if (stall_time < 0).
    Confused?  What happens if stallTimer overflows? It becomes negative.
    If the last known controller positions were in the arming positions,
    the arming criteria would be met as soon as the stall time overflows.
    Disaster averted :)
    */

  int stall_time = args->receiver[1].channel[RC_1_AILERON]->stallTimer.read_ms();
  return  (stall_time > 200) || (stall_time < 0);
}

bool is_weapon_stalled(thread_args_t *args){
  /* We have averted an extremely dangerous situation by also checking if (stall_time < 0).
    Confused?  What happens if stallTimer overflows? It becomes negative.
    If the last known controller positions were in the arming positions,
    the arming criteria would be met as soon as the stall time overflows.
    Disaster averted :)
    */

  int stall_time = args->receiver[0].channel[RC_0_THROTTLE]->stallTimer.read_ms();
  return  (stall_time > 200) || (stall_time < 0);
}
