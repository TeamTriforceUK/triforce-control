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

/* Includes */
#include "bno055.h"
#include "esc.h"
#include "PwmIn.h"
#include "mbed.h"
#include "triforce.h"
#include "math.h"
#include "tmath.h"

/* USB */
Serial pc(USBTX, USBRX);

/* 8 Channel RC input */
PwmIn rc_channel_1(p5); // Roll
PwmIn rc_channel_2(p6); // Pitch
PwmIn rc_channel_3(p7); // Throttle
PwmIn rc_channel_4(p8); // Yaw
PwmIn rc_channel_5(p11); // Switch R front
PwmIn rc_channel_6(p12); // Dial R
PwmIn rc_channel_7(p13); // Dial L
PwmIn rc_channel_8(p14); // Switch L Back

/* 5 channel ESC output */
ESC esc_omni_1(p21, 20, 1500);
ESC esc_omni_2(p22, 20, 1500);
ESC esc_omni_3(p23, 20, 1500); //LED4
ESC esc_weapon_1(p24); //LED3
ESC esc_weapon_2(p25); //LED2

/* LEDs */
DigitalOut led1(LED1);

/* Initial control positions */
static struct rc_controls controls = {
    .channel_1 = 50,
    .channel_2 = 50,
    .channel_3 = 50,
    .channel_4 = 0,
    .channel_5 = 0,
    .channel_6 = 0,
    .channel_7 = 0,
    .channel_8 = 0
};

/* Initial output positions */
static struct rc_outputs outputs = {
    .wheel_1 = 50,
    .wheel_2 = 50,
    .wheel_3 = 50,
    .weapon_motor_1 = 0,
    .weapon_motor_2 = 0
};

/* Direction vector */
static struct direction_vector direction = {
    .rotation = 50,
    .x_translation = 50,
    .y_translation = 50
};


/******************************************************************************/


/** Init
 *
 * Sets up devices
 * Will loop init for bno055 untill it starts correctly.
 */
void init(){
    #ifdef ORIENTATION
    init_bno055:
    /* Initialise the sensor */
    if(!initBNO055()){

        /* There was a problem detecting the BNO055 ... check your connections */
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        pc.printf("BNO055 not detected\r\n");
        #endif
        goto init_bno055;

    } else {
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        pc.printf("BNO055 was detected!\r\n");
        #endif
    }
    #endif
}

void reset_outputs(){
    outputs.wheel_1 = 50;
    outputs.wheel_2 = 50;
    outputs.wheel_3 = 50;
}


/** Read control positions from RC receiver
 *
 * Converts control input pulsewidths read in seconds to throttle %
 */
void read_inputs(){


    /* Check for armed status */


    if(!armed || just_armed){
        #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
        if(arming_count == 0){

            pc.printf("MOVE STICKS TO ARMING POSITIONS AND HOLD TO ARM \r\n");
        }
        #endif

        failsafe = true;
        int c1 = convert_pulsewidth(rc_channel_1.pulsewidth());
        int c2 = convert_pulsewidth(rc_channel_2.pulsewidth());
        int c3 = convert_pulsewidth(rc_channel_3.pulsewidth());
        int c4 = convert_pulsewidth(rc_channel_4.pulsewidth());
        pc.printf("Input: C1: %i \t C2: %i \t C3: %i \t C4: %i \r\n", c1, c2, c3, c4);

        if(c1 > RC_ARM_CHANNEL_1 && c2 > RC_ARM_CHANNEL_2 && c3 < RC_ARM_CHANNEL_3 && c4 < RC_ARM_CHANNEL_4){

            if(!armed){
                #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                pc.printf("ARMING %i\r\n", arming_count);
                #endif
                arming_count++;
                wait_ms(1000);
                if(arming_count > RC_ARM_DELAY){
                    armed = true;
                    just_armed = true;
                }
            }else{
                #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                pc.printf("ARMED - CENTER CONTROLS \r\n");
                #endif
            }
        } else if(just_armed){
            /* Wait for controls to be centered to avoid unexpected movement */
            if(BETWEEN(c1, 45, 55) && BETWEEN(c2, 45, 55) && c3 < RC_ARM_CHANNEL_3 && BETWEEN(c4, 45, 55)){
                #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
                pc.printf("CONTROLS CENTERED DISABLING ARM LOCK\r\n");
                #endif
                just_armed = false;
                failsafe_timer = 0;
            }

        }else {
            arming_count = 0;
        }


    }else{





        #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
        pc.printf("******************ARMED****************** \r\n");
        #endif

        controls.channel_1 = convert_pulsewidth(rc_channel_1.pulsewidth());
        controls.channel_2 = convert_pulsewidth(rc_channel_2.pulsewidth());
        controls.channel_3 = convert_pulsewidth(rc_channel_3.pulsewidth());
        controls.channel_4 = convert_pulsewidth(rc_channel_4.pulsewidth());
        controls.channel_5 = convert_pulsewidth(rc_channel_5.pulsewidth());
        controls.channel_6 = convert_pulsewidth(rc_channel_6.pulsewidth());
        controls.channel_7 = convert_pulsewidth(rc_channel_7.pulsewidth());
        controls.channel_8 = convert_pulsewidth(rc_channel_8.pulsewidth());

        /* Check if rc is stalled and failsafe output */

        if(rc_channel_1.stallTimer.read_ms() > 200){
            failsafe = true;
            #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
            pc.printf("NO SIGNAL - FAILSAFE ACTIVATED \r\n");
            #endif
            failsafe_timer++;
            wait_ms(100);
            if(failsafe_timer > NO_SIGNAL_TIMEOUT){
                armed = false;
            }
        }else{
            failsafe_timer = 0;
            failsafe = false;
        }



        direction.rotation = controls.channel_4;
        direction.x_translation = controls.channel_2;
        direction.y_translation = controls.channel_1;

        /* Heading lock enable switch */
        if(controls.channel_8 > 75 && !heading_lock_enabled){
            heading_lock = orientation.heading;
            heading_lock_enabled = true;
        }else if(controls.channel_8 < 75 && heading_lock_enabled){
            heading_lock_enabled = false;
        }

        /* Speed for heading lock */
        heading_lock_speed = (int) controls.channel_7;

        #if defined (PC_DEBUGGING) && defined (DEBUG_INPUT)
        pc.printf("Input: C1: %i \t C2: %i \t C3: %i \t C4: %i \t C5: %i \t C6: %i \t C7: %i \t C8: %i\r\n", controls.channel_1, controls.channel_2, controls.channel_3, controls.channel_4, controls.channel_5, controls.channel_6, controls.channel_7, controls.channel_8);

        pc.printf("Heading Lock: %s \r\n", heading_lock_enabled ? "true" : "false");
        pc.printf("Heading Lock Speed: %i \r\n", heading_lock_speed);
        #endif
   }
}


/** Calculate Orientation
 *
 * Calculates orientation using the BNO055 sensor
 */
void calculate_orientation(){

    /* If there is an error then we maintain the same
     * orientation to stop random control flipping */
    if(!bno055Healthy()){
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        pc.printf("ERROR: BNO055 has an error/status problem!!!\r\n");
        #endif
    }else{

        /* Read in the Euler angles */
        orientation = getEulerAngles();

        /* We are upside down in range -30 -> -90
         * the sensor will report -60 when inverted */
        if(orientation.roll < -30 && orientation.roll > -90){
            inverted = true;
        }else{
            inverted = false;
        }
        #if defined (PC_DEBUGGING) && defined (DEBUG_ORIENTATION)
        pc.printf("Inverted= %s \t (%7.2f) \r\n", inverted ? "true" : "false", orientation.roll);
        #endif
    }


}

/** Calculate controls
 *
 * Calculates controls based on if the robot is inverted or not
 * TODO: Add inverted support
 */
void calculate_controls(){

    #ifdef OMNI_MIXER

    float x = direction.x_translation -50;
    float y = direction.y_translation -50;

    float theta = (float)atan2((double)x, (double)y);
    float magnitude = (float)sqrt((double)((x*x)+(y*y)));

    if(magnitude > 5.0f){

        float vx = magnitude * sin(theta);
        float vy = magnitude * cos(theta);
        const float sqrt3o2 = 1.0*sqrt(3.0)/2.0;

        float w0 = -vx;                   // v dot [-1, 0] / 25mm
        float w1 = 0.5*vx - sqrt3o2 * vy; // v dot [1/2, -sqrt(3)/2] / 25mm
        float w2 = 0.5*vx + sqrt3o2 * vy; // v dot [1/2, +sqrt(3)/2] / 25mm
        #if defined (PC_DEBUGGING) && defined (DEBUG_CONTROLS)
        pc.printf("Calculated Controls: (%7.2f) \t (%7.2f) \t (%7.2f) \r\n", w0, w1, w2);
        #endif
        float w0_speed =  map(w0, -70, 70, 0, 100);
        float w1_speed =  map(w1, -70, 70, 0, 100);
        float w2_speed =  map(w2, -70, 70, 0, 100);

        /* Add in rotation */
        #if defined (PC_DEBUGGING) && defined (DEBUG_CONTROLS)
        pc.printf("Mapped Controls: (%7.2f) \t (%7.2f) \t (%7.2f) \r\n", w0_speed, w1_speed, w2_speed);
        #endif
        outputs.wheel_1 += w0_speed -50;
        outputs.wheel_2 += w1_speed -50;
        outputs.wheel_3 += w2_speed -50;


    }else{
        outputs.wheel_1 = 50;
        outputs.wheel_2 = 50;
        outputs.wheel_3 = 50;
    }

     outputs.wheel_1 += direction.rotation -50;
     outputs.wheel_2 += direction.rotation -50;
     outputs.wheel_3 += direction.rotation -50;

    #else

    outputs.wheel_1 = direction.rotation;
    outputs.wheel_2 = direction.x_translation;
    outputs.wheel_3 = direction.y_translation;

    #endif

    /* Clamp outputs to correct range */
    outputs.wheel_1 = clamp(outputs.wheel_1, 0, 100);
    outputs.wheel_2 = clamp(outputs.wheel_2, 0, 100);
    outputs.wheel_3 = clamp(outputs.wheel_3, 0, 100);

    #if defined (PC_DEBUGGING) && defined (DEBUG_CONTROLS)
    pc.printf("Final Controls: %i \t %i \t %i \r\n", outputs.wheel_1, outputs.wheel_2, outputs.wheel_3);
    #endif

}

/** Heading correction
 *
 *  Works out which way to spin to return to set heading
 *  Based on tiberius turnto code
 *
 *  @param speed speed to rotate in percent
 */
void calculate_heading_correction(){
    if(heading_lock_enabled){

        /* Normalize headings */
        float n_current_heading = normalize(orientation.heading);
        float n_heading_lock = normalize(heading_lock);

        #if defined (PC_DEBUGGING) && defined (DEBUG_HEADING_LOCK)
        pc.printf("Current heading: %f \t Desired Heading: %f \r\n", n_current_heading, n_heading_lock);
        #endif

        /* Calculate error */
        float error = n_current_heading - n_heading_lock;

        /* Normalize error */
        error = normalize(error);

        /* Figure out which way to turn */
        if(abs(error) > heading_lock_deadband){

            /* Speed is 0 -> 100
             * error is -180 -> 180
             * output is 0 -> 100
             */
            //error = error * (heading_lock_speed / 100.0);
            #if defined (PC_DEBUGGING) && defined (DEBUG_HEADING_LOCK)
            pc.printf("ERROR: %7.2f", error);
            #endif
            error = map(error, -180, 180, 1.0, -1.0);


            float amount = error * 50;

            amount *= heading_lock_speed / 100.0;

            direction.rotation += amount;

        }else{

            /* In deadband so do nothing */
            direction.rotation = 50;
        }
        #if defined (PC_DEBUGGING) && defined (DEBUG_HEADING_LOCK)
        pc.printf("Heading error: (%7.2f) \t Rotation: (%7.2f) \t Speed: (%7.2f) \r\n", error, direction.rotation, heading_lock_speed);
        #endif

    }
}

/** Output to OmniMixer and ESCs
 *
 * Sends calculated output values to devices
 */
void output_stage(){
    if(!failsafe){
    //outputs.wheel_1 -= 18;
    //outputs.wheel_2 -= 27;
    //outputs.wheel_3 -= 14;

    esc_omni_1.setThrottle(outputs.wheel_1);
    esc_omni_2.setThrottle(outputs.wheel_2);
    esc_omni_3.setThrottle(outputs.wheel_3);
    //esc_weapon_1.setThrottle(outputs.weapon_motor_1);
    //esc_weapon_2.setThrottle(outputs.weapon_motor_2);
    }else{
        esc_omni_1.failsafe();
        esc_omni_2.failsafe();
        esc_omni_3.failsafe();
    }

    #if defined (PC_DEBUGGING) && defined (DEBUG_OUTPUT)
    pc.printf("OUTPUT M1: %i \t M2: %i \t M3: %i \t W1: %i \t W2: %i \r\n", outputs.wheel_1, outputs.wheel_2, outputs.wheel_3, outputs.weapon_motor_1, outputs.weapon_motor_2);
    #endif
}

/******************************************************************************/

/** Main
 *
 * Main Loop
 */
int main() {
    #ifdef PC_DEBUGGING
    pc.baud(115200);
    pc.printf("Triforce Control System \r\n");
    #endif

    init();

    while(true){
        led1 = armed;
        reset_outputs();

        #ifdef PC_DEBUGGING
        pc.printf("\x1b[2J\x1b[H");
        #endif

        read_inputs();
        if(armed && !just_armed){
            led1 = armed;
            #ifdef ORIENTATION
            calculate_orientation();
            #endif

            #ifdef HEADING_LOCK
            calculate_heading_correction();
            #endif
            calculate_controls();

            output_stage();
        }
        #ifdef PC_DEBUGGING
            wait_ms(50);
        #endif
    }
}
