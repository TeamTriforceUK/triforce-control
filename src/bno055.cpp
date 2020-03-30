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
 * @file bno055.cpp
 * @author Simon Scott, Euan W. Mutch, Cameron A. Craig
 * @date 13 May 2017
 * @copyright 2015 Simon Scott, 2017 Euan W. Mutch, Cameron A. Craig
 * @brief Interfaces with BNO055 via mbed I2C.
 *
 */

#include <stdint.h>
#include "mbed.h"
#include "bno055.h"

I2C i2c(p9, p10);

/**
 * Function to write to a single 8-bit register
 */
void bno055_write_reg(int regAddr, char value)
{
    char wbuf[2];
    wbuf[0] = regAddr;
    wbuf[1] = value;
    i2c.write(bno055_addr, wbuf, 2, false);
}

/**
 * Function to read from a single 8-bit register
 */
char bno055_read_reg(int regAddr) {
    char rwbuf = regAddr;
    i2c.write(bno055_addr, &rwbuf, 1, false);
    i2c.read(bno055_addr, &rwbuf, 1, false);
    return rwbuf;
}

/**
 * Returns the calibration status of each component
 */
calib_status_t bno055_read_calibration_status() {
    calib_status_t status;
    int regVal = bno055_read_reg(BNO055_CALIB_STAT_ADDR);

    status.mag = regVal & 0x03;
    status.acc = (regVal >> 2) & 0x03;
    status.gyr = (regVal >> 4) & 0x03;
    status.sys = (regVal >> 6) & 0x03;

    return status;
}


/**
 * Checks that there are no errors on the accelerometer
 */
bool bno055_healthy() {
    int sys_error = bno055_read_reg(BNO055_SYS_ERR_ADDR);
    wait(0.001);
    int sys_stat = bno055_read_reg(BNO055_SYS_STAT_ADDR);
    wait(0.001);

    if(sys_error == 0 && sys_stat == 5)
        return true;
    else
        return false;
}


/**
 * Configure and initialize the BNO055
 */
bool bno055_init() {
    unsigned char regVal;
    i2c.frequency(400000);
    bool startupPass = true;

    // Do some basic power-up tests
    regVal = bno055_read_reg(BNO055_ID_ADDR);
    if (regVal != 0xA0) {
        startupPass = false;
    }

    regVal = bno055_read_reg(BNO055_TEMP_ADDR);

    if (regVal == 0)
        startupPass = false;

    // Change mode to CONFIG
    bno055_write_reg(BNO055_OPR_MODE_ADDR, 0x00);
    wait(0.2);

    regVal = bno055_read_reg(BNO055_OPR_MODE_ADDR);
    wait(0.1);

    // Remap axes
    bno055_write_reg(BNO055_AXIS_MAP_CONFIG_ADDR, 0x06);    // b00_00_01_10
    wait(0.1);

    // Set to external crystal
    bno055_write_reg(BNO055_SYS_TRIGGER_ADDR, 0x80);
    wait(0.2);

    // Change mode to NDOF
    bno055_write_reg(BNO055_OPR_MODE_ADDR, 0x0C);
    wait(0.2);

    regVal = bno055_read_reg(BNO055_OPR_MODE_ADDR);
    wait(0.1);

    return startupPass;
}

/**
 * Reads the Euler angles, zeroed out
 */
euler_t bno055_read_euler_angles() {
    char buf[16];
    euler_t e;

    // Read in the Euler angles
    buf[0] = BNO055_EULER_H_LSB_ADDR;
    i2c.write(bno055_addr, buf, 1, false);
    i2c.read(bno055_addr, buf, 6, false);

    uint16_t euler_head = buf[0] + (buf[1] << 8);
    uint16_t euler_roll = buf[2] + (buf[3] << 8);
    uint16_t euler_pitch = buf[4] + (buf[5] << 8);

    e.heading = ((float) euler_head) / 16.0;
    e.roll = ((float) euler_roll) / 16.0;
    e.pitch = ((float) euler_pitch) / 16.0;

    return e;
}

euler_t bno055_read_accel() {
    char buf[16];
    euler_t e;

    // Read in the Euler angles
    buf[0] = BNO055_ACCEL_DATA_X_LSB_ADDR;
    i2c.write(bno055_addr, buf, 1, false);
    i2c.read(bno055_addr, buf, 6, false);

    uint16_t x = buf[0] + (buf[1] << 8);
    uint16_t y = buf[2] + (buf[3] << 8);
    uint16_t z = buf[4] + (buf[5] << 8);

    e.x = ((float) x) / 100.0;
    e.y = ((float) y) / 100.0;
    e.z = ((float) z) / 100.0;

    return e;
}

int16_t bno055_read_z_angular_velocity(){
	char buf[16];

    // Read in the Z angular velocity
    buf[0] = BNO055_GYR_Z_LSB_ADDR;
    i2c.write(bno055_addr, buf, 1, false);
    i2c.read(bno055_addr, buf, 2, false);

    return x = buf[0] + (buf[1] << 8);
}

uint8_t bno055_read_temp() {
  return (uint8_t) bno055_read_reg(BNO055_TEMP_ADDR);
}
