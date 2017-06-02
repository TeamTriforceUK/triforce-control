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
* @file return_codes.h
* @author Cameron A. Craig
* @date 18 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Defines return codes that are used to propogate errors to the user.
*
*/


#ifndef INCLUDE_RETURN_CODES_H_
#define INCLUDE_RETURN_CODES_H_

enum return_codes_t {
  RET_ERROR = 0,
  RET_OK,
  RET_ALREADY_DISARMED,
  RET_ALREADY_ARMED,
  RET_DISARM_FIRST
};

static const char * ret_str[] = {
  "Error",
  "Ok",
  "Already disarmed",
  "Already armed",
  "Disarm before running this command"
};

const char * err_to_str(int err);

#endif  // INCLUDE_RETURN_CODES_H_
