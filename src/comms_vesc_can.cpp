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
 * @file comms_vesc_can.cpp
 * @author Cameron A. Craig
 * @date 23 Sep 2017
 * @copyright 2017 Cameron A. Craig
 * @brief Implements CAN communication with VESC.
 */

#include <stdlib.h>
#include <vector>
#include "mbed.h"
#include "comms.h"
#include "comms_vesc_can.h"
#include "config.h"

/* The various drive configurations are available in docs/drive_modes. */
volatile comms_impl_t comms_impl_vesc_can = {
  .impl_id = COMMS_IMPL_VESC_CAN,
  .str = "VESC CAN",
  .init_comms = comms_impl_vesc_can_init_comms,
  .init_esc = comms_init_esc,
  .set_speed = comms_impl_vesc_can_set_speed,
  .get_speed = comms_impl_vesc_can_get_speed,
  .get_status = NULL,
  .stop = comms_impl_vesc_can_stop
};


void comms_impl_vesc_can_init_comms() {
}

void comms_impl_vesc_can_init_esc(comms_esc_t *esc, comms_esc_id_t id){

}

void comms_impl_vesc_can_set_speed(comms_esc_t *esc, uint32_t speed) {

}

void comms_impl_vesc_can_get_speed(const void *args) {

}

void comms_impl_vesc_can_stop(comms_esc_t *esc) {

}
