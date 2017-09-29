/* Copyright (c) 2015 Simon Scott (https://developer.mbed.org/users/simonscott/code/BNO055_Example/), MIT License
 *               2017 Euan W. Mutch, Cameron A. Craig, MIT License
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
 * @file bno055.h
 * @author Simon Scott, Euan W. Mutch, Cameron A. Craig
 * @date 13 May 2017
 * @copyright 2015 Simon Scott, 2017 Euan W. Mutch, Cameron A. Craig
 * @brief Interfaces with BNO055 via mbed I2C.
 */

#ifndef TC_BNO055_H
#define TC_BNO055_H

#include <stdint.h>

#include "mbed.h"


const int bno055_addr = 0x28 << 1;

const int BNO055_ID_ADDR                                          = 0x00;
const int BNO055_EULER_H_LSB_ADDR                                 = 0x1A;
const int BNO055_ACCEL_DATA_X_LSB_ADDR                            = 0x08;
const int BNO055_TEMP_ADDR                                        = 0x34;
const int BNO055_OPR_MODE_ADDR                                    = 0x3D;
const int BNO055_CALIB_STAT_ADDR                                  = 0x35;
const int BNO055_SYS_STAT_ADDR                                    = 0x39;
const int BNO055_SYS_ERR_ADDR                                     = 0x3A;
const int BNO055_AXIS_MAP_CONFIG_ADDR                             = 0x41;
const int BNO055_SYS_TRIGGER_ADDR                                 = 0x3F;

typedef struct
{
    int mag;
    int acc;
    int gyr;
    int sys;
} calib_status_t;

typedef struct {
  union {
    struct {
      float heading;
      float pitch;
      float roll;
    };
    struct {
      float x;
      float y;
      float z;
    };
  };
} euler_t;


void bno055_write_reg(int regAddr, char value);

char bno055_read_reg(int regAddr);

calib_status_t bno055_read_calibration_status();

bool bno055_healthy();

bool bno055_init();

euler_t bno055_read_euler_angles();
euler_t bno055_read_accel();

uint8_t bno055_read_temp();

#endif //TC_BNO055_H
