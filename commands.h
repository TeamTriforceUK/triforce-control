#ifndef TC_COMMANDS_H
#define TC_COMMANDS_H

#include "thread_args.h"
#include "command.h"

#define NUM_COMMANDS 4


static const command_t available_commands[] = {
  {.id = FULLY_DISARM, .name = "disarm"},
  {.id = PARTIAL_DISARM, .name = "decarm"},
  {.id = PARTIAL_ARM, .name = "incarm"},
  {.id = FULLY_ARM, .name = "arm"}
};

const char * command_get_str(command_id_t id);
int command_generate(command_t *command, char *buffer);
int command_execute(command_t *command, thread_args_t *targs);

int command_fully_disarm(command_t *command, thread_args_t *targs);
int command_partial_disarm(command_t *command, thread_args_t *targs);
int command_partial_arm(command_t *command, thread_args_t *targs);
int command_fully_arm(command_t *command, thread_args_t *targs);

#endif //TC_COMMANDS_H
