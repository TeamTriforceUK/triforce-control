/* Copyright (c) 2015 Simon Scott (https://developer.mbed.org/users/simonscott/code/BNO055_Example/), MIT License
 *               2017 Euan W. Mutch, MIT License
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

#include "mbed.h"
I2C i2c(p9, p10);

const int bno055_addr = 0x28 << 1;

const int BNO055_ID_ADDR                                          = 0x00;
const int BNO055_EULER_H_LSB_ADDR                                 = 0x1A;
const int BNO055_TEMP_ADDR                                        = 0x34;
const int BNO055_OPR_MODE_ADDR                                    = 0x3D;
const int BNO055_CALIB_STAT_ADDR                                  = 0x35;
const int BNO055_SYS_STAT_ADDR                                    = 0x39;
const int BNO055_SYS_ERR_ADDR                                     = 0x3A;
const int BNO055_AXIS_MAP_CONFIG_ADDR                             = 0x41;
const int BNO055_SYS_TRIGGER_ADDR                                 = 0x3F;

typedef struct CalibStatus_t
{
    int mag;
    int acc;
    int gyr;
    int sys;
} CalibStatus;

typedef struct Euler_t
{
    float heading;
    float pitch;
    float roll;
} Euler;


/**
 * Function to write to a single 8-bit register
 */
void writeReg(int regAddr, char value)
{
    char wbuf[2];
    wbuf[0] = regAddr;
    wbuf[1] = value;
    i2c.write(bno055_addr, wbuf, 2, false);
}

/**
 * Function to read from a single 8-bit register
 */
char readReg(int regAddr)
{
    char rwbuf = regAddr;
    i2c.write(bno055_addr, &rwbuf, 1, false);
    i2c.read(bno055_addr, &rwbuf, 1, false);
    return rwbuf;
}

/**
 * Returns the calibration status of each component
 */
CalibStatus readCalibrationStatus()
{
    CalibStatus status;
    int regVal = readReg(BNO055_CALIB_STAT_ADDR);

    status.mag = regVal & 0x03;
    status.acc = (regVal >> 2) & 0x03;
    status.gyr = (regVal >> 4) & 0x03;
    status.sys = (regVal >> 6) & 0x03;

    return status;
}


/**
 * Checks that there are no errors on the accelerometer
 */
bool bno055Healthy()
{
    int sys_error = readReg(BNO055_SYS_ERR_ADDR);
    wait(0.001);
    int sys_stat = readReg(BNO055_SYS_STAT_ADDR);
    wait(0.001);

    if(sys_error == 0 && sys_stat == 5)
        return true;
    else
        return false;
}


/**
 * Configure and initialize the BNO055
 */
bool initBNO055()
{
    unsigned char regVal;
    i2c.frequency(400000);
    bool startupPass = true;

    // Do some basic power-up tests
    regVal = readReg(BNO055_ID_ADDR);
    if(regVal != 0xA0){
        startupPass = false;
    }

    regVal = readReg(BNO055_TEMP_ADDR);

    if(regVal == 0)
        startupPass = false;

    // Change mode to CONFIG
    writeReg(BNO055_OPR_MODE_ADDR, 0x00);
    wait(0.2);

    regVal = readReg(BNO055_OPR_MODE_ADDR);
    wait(0.1);

    // Remap axes
    writeReg(BNO055_AXIS_MAP_CONFIG_ADDR, 0x06);    // b00_00_01_10
    wait(0.1);

    // Set to external crystal
    writeReg(BNO055_SYS_TRIGGER_ADDR, 0x80);
    wait(0.2);

    // Change mode to NDOF
    writeReg(BNO055_OPR_MODE_ADDR, 0x0C);
    wait(0.2);

    regVal = readReg(BNO055_OPR_MODE_ADDR);
    wait(0.1);

    return startupPass;
}

/**
 * Reads the Euler angles, zeroed out
 */
Euler getEulerAngles()
{
    char buf[16];
    Euler e;

    // Read in the Euler angles
    buf[0] = BNO055_EULER_H_LSB_ADDR;
    i2c.write(bno055_addr, buf, 1, false);
    i2c.read(bno055_addr, buf, 6, false);

    short int euler_head = buf[0] + (buf[1] << 8);
    short int euler_roll = buf[2] + (buf[3] << 8);
    short int euler_pitch = buf[4] + (buf[5] << 8);

    e.heading = ((float)euler_head) / 16.0;
    e.roll = ((float)euler_roll) / 16.0;
    e.pitch = ((float)euler_pitch) / 16.0;

    return e;
}
