#ifndef LOGS_H
#define LOGS_H

typedef enum e_task_end
{
    TASK_END_SUCCESS,
    TASK_END_CRASH,
} t_task_end;

#include "../utils/bool.h"

void log_info(const char *message);
void log_warn(const char *message);
void log_error(const char *message);
void log_fatal(const char *message, bool_t show_errno);
void log_task(const char *message, t_task_end task_end);
void log_success(const char *message);
#endif