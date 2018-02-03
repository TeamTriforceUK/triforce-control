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
 *
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
#include "drive_mode.h"
#include "drive_modes.h"
#include "comms_pwm.h"
#include "comms_vesc_can.h"

/* Make available the ESC comms implementations */
extern comms_impl_t comms_impl_pwm;
extern comms_impl_t comms_impl_vesc_can;

// For memory debugging
// #include "mbed_memory_status.h"

/* Set up logging */
LocalFileSystem local("local");
Serial *serial_ptr;

int esp8266_wait_until_ready(thread_args_t *args) {
  unsigned esp8266_init_attempts = 0;
  while (!args->esp_ready_pin->read()) {
    wait(0.5);
    esp8266_init_attempts++;
    if(esp8266_init_attempts > 5) {
      // Disable tasks that require the BNO055
#if defined(TASK_STREAM_TELEMETRY) && defined(DEVICE_ESP8266)
      args->tasks[TASK_STREAM_TELEMETRY_ID].active = false;
#endif
      return RET_ERROR;
    }
  }
  return RET_OK;
}

int bno055_wait_until_ready(thread_args_t *args) {
  unsigned bno055_init_attempts = 0;
  while (!bno055_init()) {
    bno055_init_attempts++;
    if(bno055_init_attempts > 5) {
      // Disable tasks that require the BNO055
#ifdef TASK_COLLECT_TELEMETRY
      args->tasks[TASK_COLLECT_TELEMETRY_ID].active = false;
#endif
      return RET_ERROR;
    }
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

  // Create watchdog timer
  targs->wdt = new Watchdog();

  /* TODO: If recovering from a hang, we will want to restore arming state.
     This means that if we hang during a fight, there should be minimal
     interruption to control.

    Currently we arm using a switch on each controller so this isn't strictly
	necessary, but if we ever change arming mechanism to stick positions, then
	we'll need to be clever here and store arming state somewhere non-volatile.
  */
  if(targs->wdt->is_wdt_reset()) {
    serial->puts("System recovering from watchdog reset.\r\n");
  } else {
    serial->puts("System booting normally.\r\n");
  }

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

#ifdef DEVICE_BNO055
  targs->serial->printf("init(): BNO055\r\n");
  if (!bno055_wait_until_ready(targs)){
    targs->serial->printf("\tBNO055 not in use.\r\n");
  };
#endif

#ifdef DEVICE_ESP8266
  targs->serial->printf("init(): ESP8266\r\n");
  if (!esp8266_wait_until_ready(targs)) {
    targs->serial->printf("\tESP8266 not in use.\r\n");
  }
#endif

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

  uint32_t chan;
  for (chan= 0; chan < RC_NUMBER_CHANNELS; chan++) {
    targs->receiver[0].channel[chan] = &rx_weapon[chan];
    targs->receiver[1].channel[chan] = &rx_drive[chan];
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

  targs->serial->puts("init(): onboard LEDS\r\n");

  /* LEDs */
  DigitalOut led[NUM_SURFACE_LEDS] = {
    LED1,
    LED2,
    LED3,
    LED4
  };

  uint32_t l;
  for (l = 0; l < NUM_SURFACE_LEDS; l++){
    targs->leds[l] = &led[l];
  }

  //Set drive mode
  //TODO(camieac): Make drive & weapon mode configurable
  targs->drive_mode = (drive_mode_t*) &drive_modes[DM_2_WHEEL_DIFFERENTIAL];
  targs->weapon_mode = (weapon_mode_t*) &weapon_modes[WM_MANUAL_THROTTLE];

  targs->serial->puts("init(): PWM Outputs\r\n");

  //Set ESC comms implementation
  targs->comms_impl = (comms_impl_t *) &comms_impl_pwm;

  /* Initialise comms outputs to ESCS */
  targs->comms_impl->init_comms();

  /* Configure each ESC */
  targs->comms_impl->init_esc(&targs->escs.drive[0], COMMS_OUTPUT_DRIVE_1);
  targs->comms_impl->init_esc(&targs->escs.drive[1], COMMS_OUTPUT_DRIVE_2);
  targs->comms_impl->init_esc(&targs->escs.drive[2], COMMS_OUTPUT_DRIVE_3);
  targs->comms_impl->init_esc(&targs->escs.weapon[0], COMMS_OUTPUT_WEAPON_1);
  targs->comms_impl->init_esc(&targs->escs.weapon[1], COMMS_OUTPUT_WEAPON_2);
  targs->comms_impl->init_esc(&targs->escs.weapon[2], COMMS_OUTPUT_WEAPON_1);

  targs->serial->puts("init(): Command Queue\r\n");

  Mail<command_t, COMMAND_QUEUE_LEN> *command_queue = new Mail<command_t, COMMAND_QUEUE_LEN>();
  targs->command_queue = command_queue;

  targs->serial->puts("init(): Mutexes\r\n");
  targs->mutex.pc_serial = new Mutex();
  targs->mutex.controls = new Mutex();
  targs->mutex.outputs = new Mutex();
  targs->mutex.telemetry = new Mutex();

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
#ifdef TASK_MOTOR_DRIVE
    {tasks[TASK_MOTOR_DRIVE_ID].priority, tasks[TASK_MOTOR_DRIVE_ID].stack_size},
#endif
#ifdef TASK_ARMING
    {tasks[TASK_ARMING_ID].priority, tasks[TASK_ARMING_ID].stack_size},
#endif
#ifdef TASK_FAILSAFE
    {tasks[TASK_FAILSAFE_ID].priority, tasks[TASK_FAILSAFE_ID].stack_size},
#endif
#if defined(TASK_CALC_ORIENTATION) && defined(DEVICE_BNO055)
    {tasks[TASK_CALC_ORIENTATION_ID].priority, tasks[TASK_CALC_ORIENTATION_ID].stack_size},
#endif
#ifdef TASK_COLLECT_TELEMETRY
    {tasks[TASK_COLLECT_TELEMETRY_ID].priority, tasks[TASK_COLLECT_TELEMETRY_ID].stack_size},
#endif
#if defined(TASK_STREAM_TELEMETRY) && defined(DEVICE_ESP8266)
    {tasks[TASK_STREAM_TELEMETRY_ID].priority, tasks[TASK_STREAM_TELEMETRY_ID].stack_size},
#endif
#ifdef TASK_CALIBRATE_CHANNELS
    {tasks[TASK_CALIBRATE_CHANNELS_ID].priority, tasks[TASK_CALIBRATE_CHANNELS_ID].stack_size}
#endif
  };
  // Allow access to Thread objects thread thread_args
  targs->threads = (Thread*) &threads;

  // Print all tasks and their properties
  uint32_t t;
  for (t = 0; t < NUM_TASKS; t++) {
    targs->serial->printf("\rinit(): Task %d (%s) active: %s, stack: %d\r\n", tasks[t].id, tasks[t].name, tasks[t].active ? "Yes" : "No", tasks[t].stack_size);
  }

  //Start watchdog timer before we start the tasks
  targs->wdt->kick(WATCHDOG_TIME_SECONDS);

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


  delete(targs->mutex.pc_serial);
  delete(targs->mutex.controls);
  delete(targs->mutex.outputs);
  delete(targs->mutex.telemetry);

  delete(targs->esp_ready_pin);
  delete(targs->wdt);
  delete(command_queue);
  delete(serial);

  free(targs);
}
