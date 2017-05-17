#ifndef TC_COMMAND_H
#define TC_COMMAND_H

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

#endif //TC_COMMAND_H
