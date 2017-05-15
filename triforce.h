/* Copyright (c) 2017 Cameron A. Craig, Euan W. Mutch, MIT License
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
* @file triforce.h
* @author Euan W. Mutch, Cameron A. Craig
* @date 13 May 2017
* @copyright 2017 Euan W. Mutch, Cameron A. Craig
* @brief Defines constants structs and globals.
*/

#ifndef TRIFORCE_H
#define TRIFORCE_H

/* Debugging defines */
#define DEBUG_INPUT  // Send input values to usb serial
//#define DEBUG_ORIENTATION // Send orientation readings to usb serial
//#define DEBUG_HEADING_LOCK // Send heading lock values to usb serial
//#define DEBUG_CONTROLS // Send calculated controls to usb serial
#define DEBUG_OUTPUT // Send output values to usb serial
#define PC_DEBUGGING // Output debugging information over usb serial

/* Configuration defines */
#define HEADING_LOCK // Enable heading lock functionality
#define ORIENTATION // Enable orientation functionality
#define OMNI_MIXER // Enable omni mixer functionality



#endif
