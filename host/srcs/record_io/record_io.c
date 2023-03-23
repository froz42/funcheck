#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "record_io.h"
#include "../logs/logs.h"

/**
 * @brief Record the output of a file descriptor
 * 
 * @param param The record parameters (t_record_io *)
 * @return void* NULL
 */
static void *record_routine(void *param)
{
    char buffer[512];
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
        if (record_param->tmp_file_store)
            fwrite(buffer, read_size, 1, record_param->tmp_file_store);
    }
    if (record_param->fd_to_write != NO_FD)
        close(record_param->fd_to_write);
    pthread_exit(NULL);
}

/**
 * @brief Launch a thread to record the output of a file descriptor
 * 
 * @param record_param The record parameters
 */
void launch_record(t_record_io *record_param)
{
    record_param->fd_to_read = dup(record_param->fd_to_read);
    if(pthread_create(&record_param->thread_id, NULL, record_routine, record_param) != 0)
        log_fatal("record_io: pthread_create failed", true);
}

/**
 * @brief Stop the recording of a file descriptor
 * 
 * @param record_param The record parameters
 */
void stop_record(t_record_io *record_param)
{
    if (record_param->fd_to_read != NO_FD)
        close(record_param->fd_to_read);
    pthread_cancel(record_param->thread_id);
    pthread_join(record_param->thread_id, NULL);
}

/**
 * @brief Write the content of a record to a file descriptor
 * 
 * @param fd the file descriptor to write to
 * @param record the record to read from
 */
void write_record_to_fd(int fd, FILE *record)
{
    rewind(record);
    char buffer[BUFSIZ];
    size_t read_size;

    while ((read_size = fread(buffer, 1, sizeof(buffer), record)) > 0)
        write(fd, buffer, read_size);
}

/**
 * @brief Write the content of a record to stdout
 * 
 * @param record the record to read from
 */
void write_record_to_stdout(FILE *record)
{
    rewind(record);
    char buffer[BUFSIZ];
    size_t read_size;

    while ((read_size = fread(buffer, 1, sizeof(buffer) - 1, record)) > 0)
    {
        buffer[read_size] = '\0';
        printf("%s", buffer);
    }
}

t_record_io init_record_io(int fd_to_read, int fd_to_write)
{
    t_record_io record_param;

    record_param.fd_to_read = fd_to_read;
    record_param.fd_to_write = fd_to_write;
    record_param.tmp_file_store = NULL;
    return record_param;
}