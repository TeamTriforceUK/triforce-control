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
 * @file tele_params.h
 * @author Cameron A. Craig
 * @date 30 May 2017
 * @copyright 2017 Cameron A. Craig
 * @brief Defines the telemetry parameters.
 */

#ifndef INCLUDE_TELE_PARAMS_H_
#define INCLUDE_TELE_PARAMS_H_

#include "tele_param.h"

static tele_command_t tele_commands[] = {
  {.id = CID_DRIVE_RPM_1, .name = "drive_rpm_1", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_DRIVE_RPM_2, .name = "drive_rpm_2", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_DRIVE_RPM_3, .name = "drive_rpm_3", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_WEAPON_RPM_1, .name = "weapon_rpm_1", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_WEAPON_RPM_2, .name = "weapon_rpm_2", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_WEAPON_RPM_3, .name = "weapon_rpm_3", .unit = CU_RPM, .type = CT_FLOAT},
#ifdef DEVICE_BNO055
  {.id = CID_ACCEL_X, .name = "accel_x", .unit = CU_MPSPS, .type = CT_FLOAT},
  {.id = CID_ACCEL_Y, .name = "accel_y", .unit = CU_MPSPS, .type = CT_FLOAT},
  {.id = CID_ACCEL_Z, .name = "accel_z", .unit = CU_MPSPS, .type = CT_FLOAT},
  {.id = CID_PITCH, .name = "pitch", .unit = CU_DEGREES, .type = CT_FLOAT},
  {.id = CID_ROLL, .name = "roll", .unit = CU_DEGREES, .type = CT_FLOAT},
  {.id = CID_YAW, .name = "yaw", .unit = CU_DEGREES, .type = CT_FLOAT},
  {.id = CID_AMBIENT_TEMP, .name = "temp", .unit = CU_CELCIUS, .type = CT_INT32},
#endif
  {.id = CID_WEAPON_VOLTAGE_1, .name = "weapon_voltage_1", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_WEAPON_VOLTAGE_2, .name = "weapon_voltage_2", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_WEAPON_VOLTAGE_3, .name = "weapon_voltage_3", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_DRIVE_VOLTAGE_1, .name = "drive_voltage_1", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_DRIVE_VOLTAGE_2, .name = "drive_voltage_2", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_DRIVE_VOLTAGE_3, .name = "drive_voltage_3", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_ARM_STATUS, .name = "arm_status", .unit = CU_NONE, .type = CT_INT32},
};

#define NUM_TELE_COMMANDS (sizeof(tele_commands) / sizeof(tele_command_t))

#endif  // INCLUDE_TELE_PARAMS_H_
