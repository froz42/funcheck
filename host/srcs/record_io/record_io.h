/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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