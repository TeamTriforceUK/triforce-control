/* mbed PwmIn Library
 * Copyright (c) 2008-2010, sford
 * Subsequently modified by Trung Tin Ian HUA 2014 to add lost signal detection.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "mbed.h"

#ifndef MBED_PWMIN_H_
#define MBED_PWMIN_H_

/** PwmIn class to read PWM inputs
 *
 * Uses InterruptIn to measure the changes on the input
 * and record the time they occur
 *
 * @note uses InterruptIn, so not available on p19/p20
 */
class PwmIn
{
public:
    /** Create a PwmIn
     *
     * @param p The pwm input pin (must support InterruptIn)
     */
    PwmIn(PinName p);

    void rise();
    void fall();

    /** Read the current period
     *
     * @returns the period in seconds
     */
    float period();

    /** Read the current pulsewidth
     *
     * @returns the pulsewidth in seconds
     */
    float pulsewidth();

    /** Read the current dutycycle
     *
     * @returns the dutycycle as a percentage, represented between 0.0-1.0
     */
    float dutycycle();

    /** Timer to detect when a pulse has not changed over time (Ian).
     */
    Timer stallTimer;
protected:
    InterruptIn _p;
    Timer _t;
    volatile int _pulsewidth, _period;
    bool stallInit;
};

#endif
