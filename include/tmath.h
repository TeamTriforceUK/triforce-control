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
* @file tmath.h
* @author Euan W. Mutch, Cameron A. Craig
* @date 13 May 2017
* @copyright 2017 Cameron A. Craig
* @brief Math helper functions.
*/

#ifndef TC_MATH_H
#define TC_MATH_H

#define BETWEEN(value, min, max) (value < max && value > min)

/**
* @brief Weird mapping function written by Euan.
*/
float map(float in, float inMin, float inMax, float outMin, float outMax);

/**
* @brief Limit a value between min and max.
*/
float clamp(float d, float min, float max);

/**
 * @brief Convert from pulsewidth in seconds, to %
 */
int convert_pulsewidth(float pulsewidth);

/**
* @brief Go from 0 -> 360 range to -180 to 180 range
*/
float normalize(float heading);

#endif //TC_MATH_H
