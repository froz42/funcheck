#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "record_io.h"

static void *record_routine(void *param)
{
    char buffer[512];
    size_t size_stdin_record = 0;
    size_t capacity_stdin_record = 0;
    t_record_io *record_param = (t_record_io *)param;

    while (1)
    {
        ssize_t read_size = read(record_param->fd_to_read, buffer, sizeof(buffer) - 1);
        if (read_size == -1)
            break;
        else if (read_size == 0)
            break;
        if (record_param->fd_to_write != NO_FD)
            write(record_param->fd_to_write, buffer, read_size);
        buffer[read_size] = '\0';
        if (size_stdin_record + read_size + 1 > capacity_stdin_record)
        {
            capacity_stdin_record = size_stdin_record + read_size + 1;
            record_param->record = realloc(record_param->record, capacity_stdin_record);
        }
        memcpy(record_param->record + size_stdin_record, buffer, read_size);
        size_stdin_record += read_size;
        record_param->record[size_stdin_record] = '\0';
    }
    if (record_param->fd_to_write != NO_FD)
        close(record_param->fd_to_write);
    pthread_exit(NULL);
}

void launch_record(t_record_io *record_param)
{
    record_param->fd_to_read = dup(record_param->fd_to_read);
    pthread_create(&record_param->thread_id, NULL, record_routine, record_param);
}

void stop_record(t_record_io *record_param)
{
    if (record_param->fd_to_read != NO_FD)
        close(record_param->fd_to_read);
    pthread_join(record_param->thread_id, NULL);
}

void force_stop_record(t_record_io *record_param)
{
    if (record_param->fd_to_read != NO_FD)
        close(record_param->fd_to_read);
    pthread_cancel(record_param->thread_id);
    pthread_join(record_param->thread_id, NULL);
}