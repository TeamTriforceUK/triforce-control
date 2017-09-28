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
* @file telemetry_param.h
* @author Cameron A. Craig
* @date 28 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines all parameters avaiable for telemetry.
*/

#ifndef INCLUDE_TELE_PARAM_H_
#define INCLUDE_TELE_PARAM_H_

/* TODO(camieac): I want to refactor the commands to make it easy to add new
   ones. I'll probably add a function for each command that updates the
   telemetry value. And a pointer to this function stored in tele_command_t.
*/

/**
 * Supported types that can be passed as a value in a telemetry parameter.
 */
typedef enum {
  CT_INT = 0,
  CT_FLOAT,
  CT_STRING,
  CT_BOOLEAN,
  CT_NONE
} tele_command_type_t;

/**
 * Supported units that can be associated with a telemetry parameter.
 */
typedef enum {
  CU_RPM = 0,
  CU_RPS,
  CU_MPSPS,
  CU_CELCIUS,
  CU_VOLTS,
  CU_DEGREES,
  CU_NONE
} tele_command_unit_t;

/**
 * Defines all telemetry parameters.
 * Note: For ease of access, the command ID (CID) value should correspond with its
 * position within the tele_commands array (tele_params.h).
 */
typedef enum tele_command_id_t {
  CID_RING_RPM = 0,
  CID_CON_1_RPM,
  CID_CON_2_RPM,
  CID_ACCEL_X,
  CID_ACCEL_Y,
  CID_ACCEL_Z,
  CID_PITCH,
  CID_ROLL,
  CID_YAW,
  CID_WEAPON_VOLTAGE,
  CID_DRIVE_VOLTAGE,
  CID_AMBIENT_TEMP,
  CID_ESP_LED,
  CID_ARM_STATUS,
};

/**
 * This info stores the name and value of a parameter to be sent to ESP8266.
 */
typedef struct {
  tele_command_id_t id;

  const char *name;
  tele_command_unit_t unit;
  tele_command_type_t type;

  /**
   * Allow one of many types to be used.
   */
  union {
    float f;
    int i;
    char c;
    bool b;
    const char *s;
  } param;

} tele_command_t;

#endif  // INCLUDE_TELE_PARAM_H_
