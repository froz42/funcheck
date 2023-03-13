#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct timeval timeval_t;

typedef unsigned int msseconds_t;

timeval_t get_time(void);
msseconds_t get_timelapse(timeval_t start);
const char *get_time_string(msseconds_t lapse);

#endif
