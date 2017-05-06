#ifndef UWBQuad__ESC__H
#define UWBQuad__ESC__H

/** ESC Class Example:
 * 
 * @code
 *
#include "mbed.h"
#include "esc.h"

ESC esc1(PTD4);
ESC esc2(PTA12);
ESC esc3(PTA4);
ESC esc4(PTA5);

Serial pc(USBTX, USBRX);    // tx, rx

int main() {
    
    char c;
    int var = 0;

    while(1) {
        c = pc.getc();
        
        if (c == 'u') {
            if (var < 100) {
                var++;
            }
            if (esc1.setThrottle(var) && esc2.setThrottle(var) && esc3.setThrottle(var) && esc4.setThrottle(var)) {
                printf("%i\r\n", var);
            }
        }
        else if (c == 'd') {
            if (var > 0) {
                var--;
            }
            if (esc1.setThrottle(var) && esc2.setThrottle(var) && esc3.setThrottle(var) && esc4.setThrottle(var)) {
                printf("%i\r\n", var);
            }
        }
        else if (c == 'r') {
            var = 0;
            if (esc1.setThrottle(var) && esc2.setThrottle(var) && esc3.setThrottle(var) && esc4.setThrottle(var)) {
                printf("%i\r\n", var);                
            }
        }
        
        esc1.pulse();
        esc2.pulse();
        esc3.pulse();
        esc4.pulse();
        wait_ms(20);  // 20ms is the default period of the ESC pwm
    }
}
 * @endcode
 */

class ESC {
public:
    ESC(PwmOut,int=20,int=1000);         // Constructor(pwmPinOut,period_ms)
    bool setThrottle(int);      // range 0-100
    void pulse(void);
    void failsafe(void);
        
private:
    PwmOut esc;                 // pinout from MCU
    int period;
    int throttle;
 };

#endif // end of ESC