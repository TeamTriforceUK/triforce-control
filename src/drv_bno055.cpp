/**
* @file drv_bno055.cpp
* @author Cameron A. Craig
* @date 1 Oct 2017
* @copyright 2017 Cameron A. Craig
* @brief <brief>
*/

#include "drv_bno055.h"
#include "i2c_device.h"
#include "return_codes.h"
#include "mbed.h"


uint32_t bno055_init(i2c_device_t *dev, PinName sda, PinName scl) {
  uint8_t tmp_val;
  printf("init\r\n");
  i2c_device_init(dev, 0x28, 400000, sda, scl);
  printf("init done\r\n");
  // Ensure we have the correct device
  i2c_read_reg(dev, BNO_CHIP_ID, &tmp_val, 8);
  if (tmp_val != BNO_DEV_ID) {
      return RET_INIT_WRONG_DEVICE;
  }

  // Change to CONFIG mode to allow configuration changes
  i2c_write_reg(dev, BNO_OPR_MODE, BNO_CONFIG_OP_MODE);
  wait(0.2);

  // Use external crytal oscillator
  i2c_write_reg(dev, BNO_SYS_TRIGGER, 0x80);
  wait(0.2);

  // Set mode to NDOF fusion mode
  i2c_write_reg(dev, BNO_OPR_MODE, BNO_NDOF_OP_MODE);
  wait(0.2);
  return RET_OK;
}
// bno055_read_accel()
//
// bno055_read_temp()
//
// bno055_read_direction()
//
// bno055_read
//
// bno055_config
