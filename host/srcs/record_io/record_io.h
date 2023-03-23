#ifndef RECORD_IO_H
#define RECORD_IO_H

#include <pthread.h>
#include <stdio.h>

#define NO_FD -1

typedef struct
{
    pthread_t thread_id;
    int fd_to_read;
    int fd_to_write;
    FILE *tmp_file_store;
} t_record_io;

void launch_record(t_record_io *record_param);
void stop_record(t_record_io *record_param);
void write_record_to_fd(int fd, FILE *record);
void write_record_to_stdout(FILE *record);
t_record_io init_record_io(int fd_to_read, int fd_to_write);
#endif