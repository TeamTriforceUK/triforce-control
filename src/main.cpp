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
#include "assert.h"

#include "bno055.h"
#include "tmath.h"
#include "thread_args.h"
#include "config.h"
#include "tasks.h"
#include "utilc-logging.h"
#include "return_codes.h"
#include "task.h"

// For memory debugging
// #include "mbed_memory_status.h"


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

  //For memory debugging
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

  /* Set and print channel limits, these can be modified at runtime using the
  calibrate command. */

  targs->channel_limits[0][0].min = RC_0_CHAN_1_MIN;
  targs->channel_limits[0][0].max = RC_0_CHAN_1_MAX;

  targs->channel_limits[0][1].min = RC_0_CHAN_2_MIN;
  targs->channel_limits[0][1].max = RC_0_CHAN_2_MAX;

  targs->channel_limits[0][2].min = RC_0_CHAN_3_MIN;
  targs->channel_limits[0][2].max = RC_0_CHAN_3_MAX;

  targs->channel_limits[0][3].min = RC_0_CHAN_4_MIN;
  targs->channel_limits[0][3].max = RC_0_CHAN_4_MAX;

  targs->channel_limits[0][4].min = RC_0_CHAN_5_MIN;
  targs->channel_limits[0][4].max = RC_0_CHAN_5_MAX;

  targs->channel_limits[0][5].min = RC_0_CHAN_6_MIN;
  targs->channel_limits[0][5].max = RC_0_CHAN_6_MAX;

  targs->channel_limits[1][0].min = RC_1_CHAN_1_MIN;
  targs->channel_limits[1][0].max = RC_1_CHAN_1_MAX;

  targs->channel_limits[1][1].min = RC_1_CHAN_2_MIN;
  targs->channel_limits[1][1].max = RC_1_CHAN_2_MAX;

  targs->channel_limits[1][2].min = RC_1_CHAN_3_MIN;
  targs->channel_limits[1][2].max = RC_1_CHAN_3_MAX;

  targs->channel_limits[1][3].min = RC_1_CHAN_4_MIN;
  targs->channel_limits[1][3].max = RC_1_CHAN_4_MAX;

  targs->channel_limits[1][4].min = RC_1_CHAN_5_MIN;
  targs->channel_limits[1][4].max = RC_1_CHAN_5_MAX;

  targs->channel_limits[1][5].min = RC_1_CHAN_6_MIN;
  targs->channel_limits[1][5].max = RC_1_CHAN_6_MAX;

  int chan;
  // while(1) {
    for (chan= 0; chan < RC_NUMBER_CHANNELS; chan++) {
      targs->receiver[0].channel[chan] = &rx_drive[chan];
      targs->receiver[1].channel[chan] = &rx_weapon[chan];
      // A test read ensures PwmIn is configured correctly
      targs->serial->printf("\tinit(): RX %d channel %d: %d\r\n",
        0, chan,
        convert_pulsewidth(
          targs->receiver[0].channel[chan]->pulsewidth()));
      targs->serial->printf("\tinit(): RX %d channel %d: %d\r\n",
        1, chan,
        convert_pulsewidth(
          targs->receiver[1].channel[chan]->pulsewidth()));
    }
    // Thread::wait(500);
  // }

  targs->serial->puts("init(): PWM Outputs\r\n");

  /* 5 channel ESC output */
  ESC esc_omni_1(DRIVE_ESC_OUT_1_PIN, 20, 1500);
  targs->escs.drive[0] = &esc_omni_1;

  ESC esc_omni_2(DRIVE_ESC_OUT_2_PIN, 20, 1500);
  targs->escs.drive[1] = &esc_omni_2;

  ESC esc_omni_3(DRIVE_ESC_OUT_3_PIN, 20, 1500);
  targs->escs.drive[2] = &esc_omni_3;

  ESC esc_weapon_1(WEAPON_ESC_OUT_1_PIN);
  targs->escs.weapon[0] = &esc_weapon_1;

  ESC esc_weapon_2(WEAPON_ESC_OUT_2_PIN);
  targs->escs.weapon[1] = &esc_weapon_2;

  ESC esc_weapon_3(WEAPON_ESC_OUT_3_PIN);
  targs->escs.weapon[2] = &esc_weapon_3;

  targs->serial->puts("init(): onboard LEDS\r\n");

  /* LEDs */
  DigitalOut led[NUM_SURFACE_LEDS] = {
    LED1,
    LED2,
    LED3,
    LED4
  };

  int l;
  for (l = 0; l < NUM_SURFACE_LEDS; l++){
    targs->leds[l] = &led[l];
  }

  targs->serial->puts("init(): Command Queue\r\n");

  Mail<command_t, COMMAND_QUEUE_LEN> *command_queue = new Mail<command_t, COMMAND_QUEUE_LEN>();
  targs->command_queue = command_queue;

  targs->serial->puts("init(): Mutexes\r\n");
  targs->mutex.pc_serial = new Mutex();

  targs->serial->printf("init(): Starting %d Tasks\r\n", NUM_TASKS);

  // Allow access to tasks from threads
  targs->tasks = (task_t *) &tasks;

  Thread threads[NUM_TASKS] = {
#ifdef TASK_READ_SERIAL
    {tasks[TASK_READ_SERIAL_ID].priority, tasks[TASK_READ_SERIAL_ID].stack_size},
#endif
#ifdef TASK_PROCESS_COMMANDS
    {tasks[TASK_PROCESS_COMMANDS_ID].priority, tasks[TASK_PROCESS_COMMANDS_ID].stack_size},
#endif
#ifdef TASK_LED_STATE
    {tasks[TASK_LED_STATE_ID].priority, tasks[TASK_LED_STATE_ID].stack_size},
#endif
#ifdef TASK_READ_RECEIVERS
    {tasks[TASK_READ_RECEIVERS_ID].priority, tasks[TASK_READ_RECEIVERS_ID].stack_size},
#endif
#ifdef TASK_ARMING
    {tasks[TASK_ARMING_ID].priority, tasks[TASK_ARMING_ID].stack_size},
#endif
#ifdef TASK_FAILSAFE
    {tasks[TASK_FAILSAFE_ID].priority, tasks[TASK_FAILSAFE_ID].stack_size},
#endif
#ifdef TASK_SET_ESCS
    {tasks[TASK_SET_ESCS_ID].priority, tasks[TASK_SET_ESCS_ID].stack_size},
#endif
#ifdef TASK_CALC_ORIENTATION
    {tasks[TASK_CALC_ORIENTATION_ID].priority, tasks[TASK_CALC_ORIENTATION_ID].stack_size},
#endif
#ifdef TASK_COLLECT_TELEMETRY
    {tasks[TASK_COLLECT_TELEMETRY_ID].priority, tasks[TASK_COLLECT_TELEMETRY_ID].stack_size},
#endif
#ifdef TASK_STREAM_TELEMETRY
    {tasks[TASK_STREAM_TELEMETRY_ID].priority, tasks[TASK_STREAM_TELEMETRY_ID].stack_size},
#endif
#ifdef TASK_CALIBRATE_CHANNELS
    {tasks[TASK_CALIBRATE_CHANNELS_ID].priority, tasks[TASK_CALIBRATE_CHANNELS_ID].stack_size}
#endif
  };
  // Allow access to Thread objects thread thread_args
  targs->threads = (Thread*) &threads;

  // Print all tasks and their properties
  int t;
  for (t = 0; t < NUM_TASKS; t++) {
    targs->serial->printf("\rinit(): Task %d (%s) active: %s, stack: %d\r\n", tasks[t].id, tasks[t].name, tasks[t].active ? "Yes" : "No", tasks[t].stack_size);
  }

  // Start all tasks
  for (t = 0; t < NUM_TASKS; t++) {
    // Print amount of heap used
    // print_heap_and_isr_stack_info();

    targs->serial->printf("\rinit(): Starting %s (%d) active?: %s\r\n", tasks[t].name, tasks[t].id, tasks[t].active ? "Yes" : "No");
    tasks[t].args = targs;
    // threads[t].set_priority(tasks[t].priority);
    threads[t].start(callback(tasks[t].func, tasks[t].args));

    // The task ID must correspond with its position within the task array
    assert(t == tasks[t].id);

    // Print amount of heap used
    // print_heap_and_isr_stack_info();
  }

  // Wait for all tasks to complete
  for (t = 0; t < NUM_TASKS; t++) {
    threads[t].join();
  }

  free(targs);
  delete(targs->esp_ready_pin);
  delete(command_queue);
  delete(serial);
}
