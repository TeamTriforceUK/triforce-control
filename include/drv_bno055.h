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
* @file drv_bno055.h
* @author Cameron A. Craig
* @date 12 Oct 2017
* @copyright 2017 Cameron A. Craig
* @brief Tons of ifdefs!
*/

#ifndef TC_DRV_BNO055_H
#define TC_DRV_BNO055_H

#include "i2c_device.h"
#include "stdint.h"

#define BNO_AXIS_MAP_SIGN   0x42
#define BNO_AXIS_MAP_CONFIG 0x41
#define BNO_TEMP_SOURCE     0x40
#define BNO_SYS_TRIGGER     0x3F
#define BNO_PWR_MODE        0x3E
#define BNO_OPR_MODE        0x3D
#define BNO_UNIT_SEL        0x3B
#define BNO_SYS_ERR         0x3A
#define BNO_SYS_STATUS      0x39
#define BNO_SYS_CLK_STATUS  0x38
#define BNO_INT_STA         0x37
#define BNO_ST_RESULT       0x36
#define BNO_CALIB_STAT      0x35
#define BNO_TEMP            0x34

#define BNO_GRV_DATA_Z_MSB  0x33
#define BNO_GRV_DATA_Z_LSB  0x32
#define BNO_GRV_DATA_Y_MSB  0x31
#define BNO_GRV_DATA_Y_LSB  0x30
#define BNO_GRV_DATA_X_MSB  0x2F
#define BNO_GRV_DATA_X_LSB  0x2E

#define BNO_LIA_DATA_Z_MBS  0x2D
#define BNO_LIA_DATA_Z_LSB  0x2C
#define BNO_LIA_DATA_Y_MBS  0x2B
#define BNO_LIA_DATA_Y_LSB  0x2A
#define BNO_LIA_DATA_X_MBS  0x29
#define BNO_LIA_DATA_X_LSB  0x28

#define BNO_QUA_DATA_Z_MSB  0x27
#define BNO_QUA_DATA_Z_LSB  0x26
#define BNO_QUA_DATA_Y_MSB  0x25
#define BNO_QUA_DATA_Y_LSB  0x24
#define BNO_QUA_DATA_X_MSB  0x23
#define BNO_QUA_DATA_X_LSB  0x22
#define BNO_QUA_DATA_W_MSB  0x21
#define BNO_QUA_DATA_W_LSB  0x20


#define BNO_EUL_PITCH_MSB  0x1F
#define BNO_EUL_PITCH_LSB  0x1E

#define BNO_EUL_ROLL_MSB 0x1D
#define BNO_EUL_ROLL_LSB 0x1C

#define BNO_EUL_HEADING_MSB 0x1B
#define BNO_EUL_HEADING_LSB 0x1A

#define BNO_GYR_DATA_Z_MSB 0x19
#define BNO_GYR_DATA_Z_LSB 0x18
#define BNO_GYR_DATA_Y_MSB 0x17
#define BNO_GYR_DATA_Y_LSB 0x16
#define BNO_GYR_DATA_X_MSB 0x15
#define BNO_GYR_DATA_X_LSB 0x14
#define BNO_MAG_DATA_Z_MSB 0x13
#define BNO_MAG_DATA_Z_LSB 0x12
#define BNO_MAG_DATA_Y_MSB 0x11
#define BNO_MAG_DATA_Y_LSB 0x10

#define BNO_MAG_DATA_X_MSB 0x0F
#define BNO_MAG_DATA_X_LSB 0x0E
#define BNO_ACC_DATA_Z_MSB 0x0D
#define BNO_ACC_DATA_Z_LSB 0x0C
#define BNO_ACC_DATA_Y_MSB 0x0B
#define BNO_ACC_DATA_Y_LSB 0x0A
#define BNO_ACC_DATA_X_MSB 0x09
#define BNO_ACC_DATA_X_LSB 0x08

#define BNO_PAGE_ID 0x07
#define BNO_BL_REV_ID 0x06
#define BNO_SW_REV_ID_MSB 0x05
#define BNO_SW_REV_ID_LSB 0x04
#define BNO_GYR_ID 0x03
#define BNO_MAG_ID 0x02
#define BNO_ACC_ID 0x01
#define BNO_CHIP_ID 0x00
#define BNO_UNIQUE_ID 0x50 //0x50 - 0x5F

#define BNO_GYR_AM_SET 0x1F
#define BNO_GYR_AM_THRES 0x1E
#define BNO_GYR_DUR_Z 0x1D
#define BNO_GYR_HR_Z_SET 0x1C

#define BNO_GYR_DUR_Y 0x1B
#define BNO_GYR_HR_Y_SET 0x1A
#define BNO_GYR_DUR_X 0x19
#define BNO_GYR_HR_X_SET 0x18
#define BNO_GYR_INT_SETTING 0x17

#define BNO_ACC_NM_SET 0x16
#define BNO_ACC_NM_THRE 0x15
#define BNO_ACC_HG_THRES 0x14
#define BNO_ACC_HG_DURATION 0x13
#define BNO_ACC_INT_SETTINGS 0x12
#define BNO_ACC_AM_THRES 0x11

#define BNO_INT_EN 0x10
#define BNO_INT_MSK 0x0F

#define BNO_ACC_SLEEP_CONFIG_1 0x0D
#define BNO_ACC_SLEEP_CONFIG_0 0x0C
#define BNO_GYR_CONFIG_1 0x0B
#define BNO_GYR_CONFIG_0 0x0A

#define BNO_MAG_CONFIG 0x09

#define BNO_ACC_CONFIG 0x08
#define BNO_PAGE_ID 0x07

#define BNO_DEV_ID 0xA0

#define BNO_CONFIG_OP_MODE 0x00
#define BNO_NDOF_OP_MODE 0x0C

/**
 * ACC_Config is a register defining Accelerometer configuration,
 * Address:
 * Size: 8 bits
 * Structure: ooobbbgg (o: op_mode, b: bandwidth, r: range)
 * Values:
 *   op_mode:
 *   - Normal         000xxxxxb
 *   - Suspend        001xxxxxb
 *   - Low Power 1    010
 *   - Standby        011
 *   - Low Power 2    100
 *   - Deep Suspend   101
 *   bandwidth:
 *   - 7.81 Hz        xxx000xxb
 *   - 15.63 Hz       xxx001xxb
 *   - 31.25 Hz       xxx010xxb
 *   - 62.5 Hz        xxx011xxb
 *   - 125 Hz         xxx100xxb
 *   - 250 Hz         xxx101xxb
 *   - 500 Hz         xxx110xxb
 *   - 1000 Hz        xxx111xxb
 *   range:
 *   - 2G             xxxxxx00b
 *   - 4G             xxxxxx01b
 *   - 8G             xxxxxx10b
 *   - 16G            xxxxxx11b
 */
typedef struct {
  uint8_t op_mode: 3;
  uint8_t bandwidth: 3;
  uint8_t range: 2;
} accel_config_t;

/**
 * GYR_Config_0 is a register defining Gyroscope configuration,
 * Address:
 * Size: 8 bits
 * Structure: xxbbbrrr (b: op_mode, r: bandwidth)
 * Values:
 *   bandwidth:
 *   - 7.81 Hz        xx000xxxb
 *   - 15.63 Hz       xxx001xxb
 *   - 31.25 Hz       xxx010xxb
 *   - 62.5 Hz        xxx011xxb
 *   - 125 Hz         xxx100xxb
 *   - 250 Hz         xxx101xxb
 *   - 500 Hz         xxx110xxb
 *   - 1000 Hz        xxx111xxb
 *   range:
 *   - 2G             xxxxx000b
 *   - 4G             xxxxx001b
 *   - 8G             xxxxx010b
 *   - 16G            xxxxx011b
 *   - 16G            xxxxx100b
 */
typedef struct {
  uint8_t empty: 2;
  uint8_t bandwidth: 3;
  uint8_t range: 3;
} gyr_config_0_t;


/**
 * GYR_Config_1 is a register defining Gyroscope configuration,
 * Address:
 * Size: 8 bits
 * Structure: xxbbbrrr (b: op_mode, r: bandwidth)
 * Values:
 *   op_mode:
 *   - Normal              xxxxx000b
 *   - Fast Power up       xxxxx001b
 *   - Deep Suspend        xxxxx010b
 *   - Suspend             xxxxx011b
 *   - Advanced Powersave  xxxxx100b
 */
typedef struct {
  uint8_t empty: 5;
  uint8_t op_mode: 3;
} gyr_config_1_t;


/**
 * MAG_config is a register defining Accelerometer configuration,
 * Address:
 * Size: 8 bits
 * Structure: ooobbbgg (o: op_mode, b: bandwidth, r: range)
 * Values:
 *   op_mode:
 *   - Normal         000xxxxxb
 *   - Suspend        001xxxxxb
 *   - Low Power 1    010
 *   - Standby        011
 *   - Low Power 2    100
 *   - Deep Suspend   101
 *   bandwidth:
 *   - 7.81 Hz        xxx000xxb
 *   - 15.63 Hz       xxx001xxb
 *   - 31.25 Hz       xxx010xxb
 *   - 62.5 Hz        xxx011xxb
 *   - 125 Hz         xxx100xxb
 *   - 250 Hz         xxx101xxb
 *   - 500 Hz         xxx110xxb
 *   - 1000 Hz        xxx111xxb
 *   range:
 *   - 2G             xxxxxx00b
 *   - 4G             xxxxxx01b
 *   - 8G             xxxxxx10b
 *   - 16G            xxxxxx11b
 */
typedef struct {
  uint8_t empty: 1;
  uint8_t power_mode: 2;
  uint8_t op_mode: 2;
  uint8_t data_rate: 3;
} mag_config_t;

/**
 * UNIT_SEL is a register defining units,
 * Address:
 * Size: 8 bits
 * Structure: ooobbbgg (o: op_mode, b: bandwidth, r: range)
 * Values:
 *   Acceleration
 *   - m/s/s         xxxxxxx0b
 *   - mg            xxxxxxx1b
 *   bandwidth:
 *   - 7.81 Hz        xxx000xxb
 *   - 15.63 Hz       xxx001xxb
 *   - 31.25 Hz       xxx010xxb
 *   - 62.5 Hz        xxx011xxb
 *   - 125 Hz         xxx100xxb
 *   - 250 Hz         xxx101xxb
 *   - 500 Hz         xxx110xxb
 *   - 1000 Hz        xxx111xxb
 *   range:
 *   - 2G             xxxxxx00b
 *   - 4G             xxxxxx01b
 *   - 8G             xxxxxx10b
 *   - 16G            xxxxxx11b
 */
typedef struct {
  uint8_t empty0: 3;
  uint8_t temp: 1;
  uint8_t empty1: 1;
  uint8_t euler_angles: 1;
  uint8_t angular_rate: 1;
  uint8_t accel: 1;
} unit_sel_t;

/**
 * UNIT_SEL is a register defining units,
 * Address:
 * Size: 8 bits
 * Structure: ooobbbgg (o: op_mode, b: bandwidth, r: range)
 * Values:
 *   Acceleration
 *   - m/s/s         xxxxxxx0b
 *   - mg            xxxxxxx1b
 *   bandwidth:
 *   - 7.81 Hz        xxx000xxb
 *   - 15.63 Hz       xxx001xxb
 *   - 31.25 Hz       xxx010xxb
 *   - 62.5 Hz        xxx011xxb
 *   - 125 Hz         xxx100xxb
 *   - 250 Hz         xxx101xxb
 *   - 500 Hz         xxx110xxb
 *   - 1000 Hz        xxx111xxb
 *   range:
 *   - 2G             xxxxxx00b
 *   - 4G             xxxxxx01b
 *   - 8G             xxxxxx10b
 *   - 16G            xxxxxx11b
 */
typedef struct {
  uint8_t data_x_lsb: 8;
  uint8_t data_x_msb: 8;
  uint8_t data_y_lsb: 8;
  uint8_t data_y_msb: 8;
  uint8_t data_z_lsb: 8;
  uint8_t data_z_msb: 8;
} accel_data_t;

uint32_t bno055_init(i2c_device_t *dev, PinName sda, PinName scl);

#endif //TC_DRV_BNO055_H
