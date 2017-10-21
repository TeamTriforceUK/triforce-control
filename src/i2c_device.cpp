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
* @file i2c_device.cpp
* @author Cameron A. Craig
* @date 04 Oct 2017
* @copyright 2017 Cameron A. Craig
* @brief Little I2C wrapper, hiding some quirks.
*/

#include "i2c_device.h"
#include "mbed.h"

void i2c_device_init(i2c_device_t *dev, uint8_t addr, uint32_t freq, PinName sda_pin, PinName scl_pin) {
  printf("%s 1\r\n", __func__);
  dev->i2c = new I2C(p9, p10);
    printf("%s 2\r\n", __func__);
  dev->addr = addr;
    printf("%s 3\r\n", __func__);
  dev->i2c->frequency(freq);
}

void i2c_read_reg(i2c_device_t *dev, uint8_t addr, uint8_t *result, size_t len) {
  // Set the address to read from
  dev->i2c->write(dev->addr, (char *) (&addr), 1, false);

  // Read from addr
  dev->i2c->read(dev->addr, (char *) result, len, false);
}

void i2c_write_reg(i2c_device_t *dev, uint8_t addr, uint8_t data) {
  // We fill buf with the reg address and the data to be written
  char buf[2];
  buf[0] = addr;
  buf[1] = data;
  dev->i2c->write(dev->addr << 1, buf, 2, false);
}
