/**
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
  {.id = CID_RING_RPM, .name = "ring_rpm", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_CON_1_RPM, .name = "con_1_rpm", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_CON_2_RPM, .name = "con_2_rpm", .unit = CU_RPM, .type = CT_FLOAT},
  {.id = CID_ACCEL_X, .name = "accel_x", .unit = CU_MPSPS, .type = CT_FLOAT},
  {.id = CID_ACCEL_Y, .name = "accel_y", .unit = CU_MPSPS, .type = CT_FLOAT},
  {.id = CID_ACCEL_Z, .name = "accel_z", .unit = CU_MPSPS, .type = CT_FLOAT},
  {.id = CID_PITCH, .name = "pitch", .unit = CU_DEGREES, .type = CT_FLOAT},
  {.id = CID_ROLL, .name = "roll", .unit = CU_DEGREES, .type = CT_FLOAT},
  {.id = CID_YAW, .name = "yaw", .unit = CU_DEGREES, .type = CT_FLOAT},
  {.id = CID_WEAPON_VOLTAGE, .name = "w_voltage", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_DRIVE_VOLTAGE, .name = "d_voltage", .unit = CU_VOLTS, .type = CT_FLOAT},
  {.id = CID_AMBIENT_TEMP, .name = "temp", .unit = CU_CELCIUS, .type = CT_INT},
  {.id = CID_ESP_LED, .name = "esp_led", .unit = CU_NONE, .type = CT_BOOLEAN},
};

#define NUM_TELE_COMMANDS (sizeof(tele_commands) / sizeof(tele_command_t))

#endif  // INCLUDE_TELE_PARAMS_H_
