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

typedef enum {
  CT_INT = 0,
  CT_FLOAT,
  CT_STRING,
  CT_BOOLEAN
} tele_command_type_t;

typedef enum {
  CU_RPM = 0,
  CU_RPS,
  CU_MPSPS,
  CU_CELCIUS,
  CU_VOLTS,
  CU_DEGREES,
  CU_NONE
} tele_command_unit_t;

/* For ease of access, the command ID (CID) value should correspond with its
   position within the tele_commands array below
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
  CID_ESP_LED
};

typedef struct {
  tele_command_id_t id;

  const char *name;
  tele_command_unit_t unit;
  tele_command_type_t type;

  union {
    float f;
    int i;
    char c;
    bool b;
  } param;

} tele_command_t;

#endif  // INCLUDE_TELE_PARAM_H_
