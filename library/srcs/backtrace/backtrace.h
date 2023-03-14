#ifndef BACKTRACE_H
#define BACKTRACE_H

#include "../shared_memory/shared_memory.h"

void get_backtrace(ptr_address dest[MAX_BACKTRACE_DEPTH]);

#endif