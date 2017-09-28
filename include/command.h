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
* @file command.h
* @author Cameron A. Craig
* @date 18 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines command structs, see also commands.h
*        for definition of available commands.
*/

#ifndef INCLUDE_COMMAND_H_
#define INCLUDE_COMMAND_H_

#include "./tele_param.h"

/**
 * Available commands (used for serial interface).
 */
typedef enum  {
  FULLY_DISARM = 0,
  PARTIAL_DISARM,
  PARTIAL_ARM,
  FULLY_ARM,
  STATUS,
  GET_PARAM,
  SET_PARAM,
  CALIBRATE_CHANNELS
} command_id_t;

/**
 * Stores command parameters.
 */
typedef struct {
  command_id_t id;
  const char *name;

  /**
   * Used if the command gets or sets a parameter.
   */
  tele_command_t *tele_param;

  /**
   * Stores a value as one of a number of types.
   */
  union {
    float f;
    int i;
    char c;
    bool b;
    const char *s;
  } value;
} command_t;

#endif  // INCLUDE_COMMAND_H_
