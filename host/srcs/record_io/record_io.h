#ifndef RECORD_IO_H
#define RECORD_IO_H

#include <pthread.h>

# define NO_FD -1

typedef struct
{
    pthread_t   thread_id;
    char        *record;
    int         fd_to_read;
    int         fd_to_write;
} t_record_io;


void launch_record(t_record_io *record_param);
void stop_record(t_record_io *record_param);
void force_stop_record(t_record_io *record_param);

#endif