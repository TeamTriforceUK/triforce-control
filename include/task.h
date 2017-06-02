/**
* @file task.h
* @author Cameron A. Craig
* @date 1 Jun 2017
* @copyright 2017 Cameron A. Craig
* @brief <brief>
*/

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "mbed.h"

typedef struct {
  int id;
  const char *name;
  void (*func)(const void*);
  void * args;
  osPriority priority;
  volatile bool active;
} task_t;

#endif  // INCLUDE_TASK_H_
