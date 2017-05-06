#include "mbed.h"
#include "esc.h"

ESC::ESC(PwmOut pwmPinOut, int period, int throttle)
    : esc(pwmPinOut), period(period), throttle(throttle) {
        esc.period_ms(period);
        esc.pulsewidth_us(throttle);
    }

bool ESC::setThrottle(int t) {
    if (t >= 0 && t <= 100) {       // qualify range, 0-100
        throttle = t*10 + 1000;     // map to range, 1-2 ms (1000-2000us)
        esc.pulsewidth_us(throttle);
        return true;
    }
    return false;
}

void ESC::pulse(void) {
    esc.pulsewidth_us(throttle);
}

void ESC::failsafe(void) {
    esc.pulsewidth_us(0);
}
