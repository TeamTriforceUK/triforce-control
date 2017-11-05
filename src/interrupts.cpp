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
 * @file interrupts.cpp
 * @author Cameron A. Craig
 * @date 05 Nov 2017
 * @copyright 2017 Cameron A. Craig
 * @brief Interrupt handler functions.
 */

#include "mbed.h"
#include "thread_args.h"
#include "comms_vesc_can.h"


void handler_can(void *targs) {
  thread_args_t *args = (thread_args_t*) targs;
  CANMessage msg;
  if(args->can->read(msg)) {
  //   //Split ID into upper and lower 8-bit chunks
    uint8_t id = msg.id & 0xFF;
    uint8_t cmd = msg.id >> 8;

    // SW Filter (mbed can.filter() is broken)
    int i;
    for(i = 0; i < sizeof(args->escs.arr)/sizeof(args->escs.arr[0]); i++){
      if(args->escs.arr[i].id == id && (cmd == CAN_PACKET_STATUS)){
          // Construct parameters from 8-byte buffer from CAn message
          int32_t rpm = (msg.data[0] << 24) | (msg.data[1] << 16) | (msg.data[2] << 8) | (msg.data[3]);
          int16_t current = ((msg.data[4] << 8) | msg.data[5]);
          int16_t duty_cycle = ((msg.data[6] << 8) | msg.data[7]);

          // Convert parameters into more suitable type and store
          args->escs.arr[i].params.rpm = rpm; //Units are RPM
          args->escs.arr[i].params.current = (float) current / 10.0f;
          args->escs.arr[i].params.duty_cycle = (float) duty_cycle / 1000.0f;
      }
    }
  }
}
