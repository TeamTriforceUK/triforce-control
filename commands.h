#ifndef TC_COMMANDS_H
#define TC_COMMANDS_H

#define NUM_COMMANDS 4


enum command_id_t {
  FULLY_DISARM = 0,
  PARTIAL_DISARM,
  PARTIAL_ARM,
  FULLY_ARM
};

typedef struct {
  command_id_t id;
  const char *name;
} command_t;

static const command_t available_commands[] = {
  {.id = FULLY_DISARM, .name = "disarm"},
  {.id = PARTIAL_DISARM, .name = "decarm"},
  {.id = PARTIAL_ARM, .name = "incarm"},
  {.id = FULLY_ARM, .name = "arm"}
};

const char * command_get_str(command_id_t id);
int command_generate(command_t *command, char *buffer);

#endif //TC_COMMANDS_H
