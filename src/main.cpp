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
* @file main.cpp
* @author Euan W. Mutch, Cameron A. Craig
* @date 13 May 2017
* @copyright 2017 Euan W. Mutch, Cameron A. Craig
* @brief Drives ESCs for Triforce fighting robot based on PWM signals from receiver.
* @mainpage Triforce Control
  @section intro_sec Introduction
  This documentation has been generated from the Doxygen comments within the source
  code of this project. This is a useful developers reference.

  @section install_sec Installation
     @code{.unparsed}
      git clone https://github.com/TeamTriforceUK/triforce-control.git
      cd triforce-control
      mbed deploy
      mbed compile -t GCC_ARM -m lpc1768
     @endcode
*/

/* Includes */
#include "mbed.h"
#include "rtos.h"
#include "esc.h"
#include "PwmIn.h"

#include "bno055.h"
#include "tmath.h"
#include "thread_args.h"
#include "config.h"
#include "tasks.h"
#include "utilc-logging.h"
#include "return_codes.h"


/* Set up logging */
LocalFileSystem local("local");
Serial *serial_ptr;

int esp8266_wait_until_ready(thread_args_t *args) {
  while (!args->esp_ready_pin->read()) {
    wait(0.5);
  }
  return RET_OK;
}

/** Main
 *
 * Main Loop
 */
int main() {
  // Configure serial connection to a PC (for debug)
  Serial *serial = new Serial(USBTX, USBRX);
  serial->baud(115200);

  // Initialise thread arguments structure
  thread_args_t *targs;
  targs = (thread_args_t*) malloc(sizeof(thread_args_t));
  memset(targs, 0x00, sizeof(thread_args_t));
  thread_args_init(targs);

  // Configure serial connection to ESP8266
  targs->esp_serial = new Serial(ESP_TX, ESP_RX);
  targs->esp_serial->baud(115200);

  // Set up ready line, so the ESP8266 can tell when it's ready.
  targs->esp_ready_pin = new DigitalIn(ESP8266_READY_PIN);


  //Set baud rate for USB serial
  targs->serial = serial;
  serial_ptr = serial;


  // print_all_thread_info();
  // print_heap_and_isr_stack_info();

  // Print initial messsage inidicating start of new process
  targs->serial->puts("Triforce Control System v");
  targs->serial->puts(VERSION);
  targs->serial->puts("\r\n");

  targs->serial->puts("init(): BNO055\r\n");
  while (!bno055_init()) {}

  targs->serial->puts("init(): ESP8266\r\n");
  while (!esp8266_wait_until_ready(targs)) {}

  targs->serial->puts("init(): PWM inputs\r\n");

  /* RC inputs from two reveiver units */
  PwmIn rx_drive[RC_NUMBER_CHANNELS] = {
    RECV_D_CHAN_1_PIN,
    RECV_D_CHAN_2_PIN,
    RECV_D_CHAN_3_PIN,
    RECV_D_CHAN_4_PIN,
    RECV_D_CHAN_5_PIN,
    RECV_D_CHAN_6_PIN
  };

  PwmIn rx_weapon[RC_NUMBER_CHANNELS] = {
    RECV_W_CHAN_1_PIN,
    RECV_W_CHAN_2_PIN,
    RECV_W_CHAN_3_PIN,
    RECV_W_CHAN_4_PIN,
    RECV_W_CHAN_5_PIN,
    RECV_W_CHAN_6_PIN
  };

  int chan;
  for(chan= 0; chan < RC_NUMBER_CHANNELS; chan++){
    targs->receiver[0].channel[chan] = &rx_drive[chan];
    targs->receiver[1].channel[chan] = &rx_weapon[chan];
  }

  targs->serial->puts("init(): PWM Outputs\r\n");

  /* 5 channel ESC output */
  ESC esc_omni_1(DRIVE_ESC_OUT_1_PIN, 20, 1500);
  targs->escs.drive[0] = &esc_omni_1;

  ESC esc_omni_2(DRIVE_ESC_OUT_2_PIN, 20, 1500);
  targs->escs.drive[1] = &esc_omni_2;

  ESC esc_omni_3(DRIVE_ESC_OUT_3_PIN, 20, 1500); //LED4
  targs->escs.drive[2] = &esc_omni_3;

  ESC esc_weapon_1(WEAPON_ESC_OUT_1_PIN); //LED3
  targs->escs.weapon[0] = &esc_weapon_1;

  ESC esc_weapon_2(WEAPON_ESC_OUT_2_PIN); //LED2
  targs->escs.weapon[1] = &esc_weapon_2;

  targs->serial->puts("init(): onboard LEDS\r\n");

  /* LEDs */
  DigitalOut led[NUM_SURFACE_LEDS] = {
    LED1,
    LED2,
    LED3,
    LED4
  };

  int l;
  for(l = 0; l < NUM_SURFACE_LEDS; l++){
    targs->leds[l] = &led[l];
  }

  targs->serial->puts("init(): Command Queue\r\n");

  Mail<command_t, COMMAND_QUEUE_LEN> *command_queue = new Mail<command_t, COMMAND_QUEUE_LEN>();
  targs->command_queue = command_queue;

  targs->serial->puts("init(): Mutexes\r\n");
  targs->mutex.pc_serial = new Mutex();

  targs->serial->puts("init(): Starting Tasks\r\n");

  Thread thread_serial_commands_in;
  thread_serial_commands_in.set_priority(osPriorityRealtime);
  thread_serial_commands_in.start(callback(task_serial_commands_in, (void *) targs));

  Thread thread_process_commands;
  thread_process_commands.set_priority(osPriorityHigh);
  thread_process_commands.start(callback(task_process_commands, (void *) targs));

  Thread thread_state_leds;
  thread_state_leds.start(callback(task_state_leds, (void *) targs));

  // Thread thread_read_receiver;
  // thread_read_receiver.start(callback(task_read_receiver, (void *) targs));
  //
  // Thread thread_set_escs;
  // thread_set_escs.start(callback(task_set_escs, (void *) targs));

  Thread thread_calc_orientation;
  thread_calc_orientation.start(callback(task_calc_orientation, (void *) targs));

  Thread thread_collect_telemetry;
  // thread_collect_telemetry.set_priority(osPriorityHigh);
  thread_collect_telemetry.start(callback(task_collect_telemetry, (void *) targs));

  Thread thread_stream_telemetry;
  // thread_stream_telemetry.set_priority(osPriorityHigh);
  thread_stream_telemetry.start(callback(task_stream_telemetry, (void *) targs));


  thread_serial_commands_in.join();
  thread_process_commands.join();
  thread_state_leds.join();
  // thread_read_receiver.join();
  // thread_set_escs.join();
  thread_calc_orientation.join();
  thread_collect_telemetry.join();
  thread_stream_telemetry.join();

  free(targs);
  delete(targs->esp_ready_pin);
  delete(command_queue);
  delete(serial);
}
