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
 * @file commands.cpp
 * @author Cameron A. Craig
 * @date 18 May 2017
 * @copyright 2017 Cameron A. Craig
 * @brief Provides functions to implement available commands as well as
 *        helper functions for parsing command strings and printing commands.
 */

#include "mbed.h"
#include "commands.h"
#include "utilc-logging.h"
#include "utils.h"
#include "states.h"
#include "return_codes.h"
#include "thread_args.h"
#include "types.h"
#include "tele_params.h"
#include "tasks.h"

const char * command_get_str(command_id_t id) {
  if (id > 0 && id < NUM_COMMANDS)
    return available_commands[id].name;
  else
    return "INVALID COMMAND";
}

int command_generate(command_t *command, char *buffer) {
  // Get the size of the entire command string
  size_t command_len = strlen(buffer);
  char command_str[command_len];
  memcpy(command_str, buffer, command_len);

  // Seperate commands into parts
  char param_part[2][10];
  char command_part[10];

  char * pch;
  int part = 0;
  pch = strtok (command_str," ");
  while (pch != NULL){
    if(part == 0){
      strncpy(command_part, pch, strlen(pch));
    } else {
      strncpy(param_part[part-1], pch, strlen(pch));
    }
    pch = strtok (NULL, " ");
    part++;
  }

  // Find a matching command for the given command string
  int i;
  for (i = 0; i < NUM_COMMANDS; i++) {
    size_t cplen = strlen(command_part);
    char *command_compare;
    command_compare = (char *) command_get_str(available_commands[i].id);
    size_t cclen = strlen(command_compare);

    if (strncmp(command_compare, command_part, MIN(cclen, cplen)) == 0) {
      // When a matching command is found, populate the command
      command->id = available_commands[i].id;
      command->name = available_commands[i].name;

      if (command->id == GET_PARAM || command->id == SET_PARAM) {
        int k;
        for (k = 0; k < NUM_TELE_COMMANDS; k++) {
          printf("Checking %s\r\n", tele_commands[k].name);
          if (strncmp(param_part[0],
            tele_commands[k].name,
            MIN(strlen(param_part[0]), strlen(tele_commands[k].name))) == 0) {
            command->tele_param = &tele_commands[k];
            printf("%s %s\r\n", command_get_str(command->id), tele_commands[k].name);
            break;
          }
        }
      }

      if (command->id == SET_PARAM) {
        char *end;
        switch (command->tele_param->type) {
            case CT_INT:
              command->value.i = strtol(param_part[1], &end, 10);
              if (end == param_part[1]) {
                printf("Conversion error\r\n");
                return RET_ERROR;
              }
              printf("d: %d\r\n", command->value.i);
              break;
            case CT_FLOAT:
              command->value.f = strtod(param_part[1], &end);
              if (end == param_part[1]) {
                printf("Conversion error\r\n");
                return RET_ERROR;
              }
              printf("f: %f\r\n", command->value.f);
              break;
            case CT_BOOLEAN:
              command->value.b = (strtod(param_part[1], &end) == 1);
              if (param_part[1] == end) {
                return RET_ERROR;
              }
              return RET_OK;
            case CT_STRING:
            default:
              printf("unsupported param\r\n");
              return RET_ERROR;
        }
      }

      // Set command parameters for parameterised commands
      // int i;
      // for(i = 0; i < part; i++){
      //   memcpy(command->param[i], param_part[i], strlen(param_part[i]));
      // }

      // return true if matching command is found
      return RET_OK;
    }
  }

  // Return false (0) if no match is found
  return RET_ERROR;
}

int command_execute(command_t *command, thread_args_t *targs) {
  switch (command->id) {
    case FULLY_DISARM:
      return command_fully_disarm(command, targs);
    case PARTIAL_DISARM:
      return command_partial_disarm(command, targs);
    case PARTIAL_ARM:
      return command_partial_arm(command, targs);
    case FULLY_ARM:
      return command_fully_arm(command, targs);
    case STATUS:
      return command_status(command, targs);
    case GET_PARAM:
      return command_get_param(command, targs);
    case SET_PARAM:
      return command_set_param(command, targs);
#ifdef TASK_CALIBRATE_CHANNELS
    case CALIBRATE_CHANNELS:
      return command_calibrate_channels(command, targs);
#endif  // TASK_CALIBRATE_CHANNELS
    default:
      return RET_ERROR;
  }
}

int command_fully_disarm(command_t *command, thread_args_t *targs) {
  if (targs->state == STATE_DISARMED) {
    return RET_ALREADY_DISARMED;
  }
  targs->state = STATE_DISARMED;
  return RET_OK;
}

int command_partial_disarm(command_t *command, thread_args_t *targs) {
  switch (targs->state) {
    case STATE_DISARMED:
      return RET_ALREADY_DISARMED;
    case STATE_DRIVE_ONLY:
      targs->state = STATE_DISARMED;
      return RET_OK;
    case STATE_WEAPON_ONLY:
      targs->state = STATE_DRIVE_ONLY;
      return RET_OK;
    case STATE_FULLY_ARMED:
      targs->state = STATE_WEAPON_ONLY;
      return RET_OK;
    default:
      return RET_ERROR;
  }
}

int command_partial_arm(command_t *command, thread_args_t *targs) {
  switch (targs->state) {
    case STATE_DISARMED:
      targs->state = STATE_DRIVE_ONLY;
      return RET_OK;
    case STATE_DRIVE_ONLY:
      targs->state = STATE_WEAPON_ONLY;
      return RET_OK;
    case STATE_WEAPON_ONLY:
      targs->state = STATE_FULLY_ARMED;
      return RET_OK;
    case STATE_FULLY_ARMED:
      return RET_ALREADY_ARMED;
    default:
      return RET_ERROR;
  }
}

int command_fully_arm(command_t *command, thread_args_t *targs) {
  if (targs->state == STATE_FULLY_ARMED) {
    return RET_ALREADY_ARMED;
  }
  targs->state = STATE_FULLY_ARMED;
  return RET_OK;
}

int command_status(command_t *command, thread_args_t *targs) {
  LOG("\rStatus: %s\r\n", state_to_str(targs->state));
  // LOG("\r(ESCS) D1: %d, D2: %d, D3: %d, W1: %d, W2: %d\r\n",
  //   targs->outputs.wheel_1,
  //   targs->outputs.wheel_2,
  //   targs->outputs.wheel_3,
  //   targs->outputs.weapon_motor_1,
  //   targs->outputs.weapon_motor_2,
  // );
  // TODO: RPMs
  // LOG("\r(RPMS) W1: W2: D1: \r\n");
  LOG("\r(Orientation) detected: %s, overidden: %s\r\n",
    orientation_to_str(targs->orientation_detected),
    orientation_to_str(targs->orientation_override)
  );
  LOG("\r              heading: %d, pitch: %d, roll: %d\r\n", targs->orientation.heading, targs->orientation.pitch, targs->orientation.roll);
  return RET_OK;
}

int command_get_param(command_t *command, thread_args_t *targs) {
  LOG("Getting param\r\n");
  switch (command->tele_param->id) {
    case CID_DRIVE_RPM_1:
    case CID_DRIVE_RPM_2:
    case CID_DRIVE_RPM_3:
    case CID_WEAPON_RPM_1:
    case CID_WEAPON_RPM_2:
    case CID_WEAPON_RPM_3:
    case CID_ACCEL_X:
    case CID_ACCEL_Y:
    case CID_ACCEL_Z:
    case CID_PITCH:
    case CID_ROLL:
    case CID_YAW:
    case CID_WEAPON_VOLTAGE_1:
    case CID_WEAPON_VOLTAGE_2:
    case CID_WEAPON_VOLTAGE_3:
    case CID_DRIVE_VOLTAGE_1:
    case CID_DRIVE_VOLTAGE_2:
    case CID_DRIVE_VOLTAGE_3:
    case CID_AMBIENT_TEMP:
    case CID_ARM_STATUS:
      printf(
        "%s %s\r\n",
        tele_commands[command->tele_param->id].name,
        state_to_str(targs->state));
      break;
  }
  return RET_OK;
}
int command_set_param(command_t *command, thread_args_t *targs) {
  LOG("Setting param\r\n");
  switch (command->tele_param->id) {
    case CID_DRIVE_RPM_1:
    case CID_DRIVE_RPM_2:
    case CID_DRIVE_RPM_3:
    case CID_WEAPON_RPM_1:
    case CID_WEAPON_RPM_2:
    case CID_WEAPON_RPM_3:
    case CID_ACCEL_X:
    case CID_ACCEL_Y:
    case CID_ACCEL_Z:
    case CID_PITCH:
    case CID_ROLL:
    case CID_YAW:
    case CID_WEAPON_VOLTAGE_1:
    case CID_WEAPON_VOLTAGE_2:
    case CID_WEAPON_VOLTAGE_3:
    case CID_DRIVE_VOLTAGE_1:
    case CID_DRIVE_VOLTAGE_2:
    case CID_DRIVE_VOLTAGE_3:
    case CID_AMBIENT_TEMP:
      tele_commands[command->tele_param->id].param.f = command->value.f;
      break;
    case CID_ARM_STATUS:
      printf("Use arming commands to set arm_state!\r\n");
      return RET_ERROR;
  }
  return RET_OK;
}
#ifdef TASK_CALIBRATE_CHANNELS
int command_calibrate_channels(command_t *command, thread_args_t *targs) {
  if (targs->state != STATE_DISARMED) {
    return RET_DISARM_FIRST;
  }

  targs->tasks[TASK_CALIBRATE_CHANNELS_ID].active = true;
  return RET_OK;
}
#endif  // TASK_CALIBRATE_CHANNELS
