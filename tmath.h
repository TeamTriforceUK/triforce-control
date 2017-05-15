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

float map(float in, float inMin, float inMax, float outMin, float outMax);

float clamp(float d, float min, float max);

// void calculate_heading_correction(){
//     if(heading_lock_enabled){
//
//         /* Normalize headings */
//         float n_current_heading = normalize(orientation.heading);
//         float n_heading_lock = normalize(heading_lock);
//
//         #if defined (PC_DEBUGGING) && defined (DEBUG_HEADING_LOCK)
//         pc.printf("Current heading: %f \t Desired Heading: %f \r\n", n_current_heading, n_heading_lock);
//         #endif
//
//         /* Calculate error */
//         float error = n_current_heading - n_heading_lock;
//
//         /* Normalize error */
//         error = normalize(error);
//
//         /* Figure out which way to turn */
//         if(abs(error) > heading_lock_deadband){
//
//             /* Speed is 0 -> 100
//              * error is -180 -> 180
//              * output is 0 -> 100
//              */
//             //error = error * (heading_lock_speed / 100.0);
//             #if defined (PC_DEBUGGING) && defined (DEBUG_HEADING_LOCK)
//             pc.printf("ERROR: %7.2f", error);
//             #endif
//             error = map(error, -180, 180, 1.0, -1.0);
//
//
//             float amount = error * 50;
//
//             amount *= heading_lock_speed / 100.0;
//
//             direction.rotation += amount;
//
//         }else{
//
//             /* In deadband so do nothing */
//             direction.rotation = 50;
//         }
//         #if defined (PC_DEBUGGING) && defined (DEBUG_HEADING_LOCK)
//         pc.printf("Heading error: (%7.2f) \t Rotation: (%7.2f) \t Speed: (%7.2f) \r\n", error, direction.rotation, heading_lock_speed);
//         #endif
//
//     }
// }



/* Convert from pulsewidth in seconds, to % */
int convert_pulsewidth(float pulsewidth);

float normalize(float heading);

#endif //TC_MATH_H
