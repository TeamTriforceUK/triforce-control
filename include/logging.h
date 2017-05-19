#ifndef TC_LOGGING_H
#define TC_LOGGING_H

extern Serial serial;

#define LOG( args...) \
  serial.printf(args); \


#endif //TC_LOGGING_H
