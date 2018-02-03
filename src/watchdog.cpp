/* Copyright (c) 2018 Cameron A. Craig, MIT License
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
 * @file watchdog.cpp
 * @author Cameron A. Craig
 * @date 3 Feb 2018
 * @copyright 2018 Cameron A. Craig
 * @brief Watchdog timer kick and test functions.
 */

#include "mbed.h"
#include "watchdog.h"

void Watchdog::kick(float s) {
  LPC_WDT->WDCLKSEL = 0x1;                // Set CLK src to PCLK
  uint32_t clk = SystemCoreClock / 16;    // WD has a fixed /4 prescaler, PCLK default is /4
  LPC_WDT->WDTC = s * (float)clk;
  LPC_WDT->WDMOD = 0x3;                   // Enabled and Reset
  kick();
}

void Watchdog::kick(void) {
  LPC_WDT->WDFEED = 0xAA;
  LPC_WDT->WDFEED = 0x55;
}

bool Watchdog::is_wdt_reset(void) {
  return ((LPC_WDT->WDMOD >> 2) & 1);
}
