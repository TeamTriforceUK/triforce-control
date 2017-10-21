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
* @file i2c_device.h
* @author Cameron A. Craig
* @date 04 Oct 2017
* @copyright 2017 Cameron A. Craig
* @brief Little I2C wrapper, hiding some quirks.
*/

#ifndef TC_I2C_DEVICE_H_
#define TC_I2C_DEVICE_H_

#include "stdint.h"
#include "mbed.h"

// Error code
#define I2C_INIT_WRONG_DEVICE 1

typedef struct {
  /**
   * Use mbed i2c lib
   */
  I2C *i2c;

  /**
   * 7-bit address (allows up to 127 devices)
   */
  uint8_t addr;
} i2c_device_t;

void i2c_device_init(i2c_device_t *dev, uint8_t addr, uint32_t freq, PinName sda_pin, PinName scl_pin);

void i2c_read_reg(i2c_device_t *dev, uint8_t addr, uint8_t *result, size_t len);

void i2c_write_reg(i2c_device_t *dev, uint8_t addr, uint8_t data);

#endif //TC_I2C_DEVICE
