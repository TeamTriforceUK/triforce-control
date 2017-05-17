#ifndef TC_RETURN_CODES_H
#define TC_RETURN_CODES_H

enum return_codes_t {
  RET_ERROR = 0,
  RET_OK,
  RET_ALREADY_DISARMED,
  RET_ALREADY_ARMED
};

const char * ret_str[] = {
  "Error",
  "Ok",
  "Already disarmed",
  "Already armed"
};

#endif //TC_RETURN_CODES_H
