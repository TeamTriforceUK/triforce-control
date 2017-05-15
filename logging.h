#define SERIAL_LOG(type, targs, args...) \

#define SERIAL_LOG_INFO(rate, targs, args...) \
targs->serial->printf(type);
      targs->serial->printf(args); \
