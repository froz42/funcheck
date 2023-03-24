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

#include <sys/time.h>
#include <stdio.h>
#include "time.h"

/**
 * @brief Return the current time.
 *
 * @return timeval_t the current time
 */
timeval_t get_time(void)
{
    timeval_t time;
    gettimeofday(&time, NULL);
    return time;
}

/**
 * @brief Return the time lapse between two time in ms.
 * 
 * @param start start time
 * @param end end time
 * @return msseconds_t 
 */
msseconds_t get_timelapse(timeval_t start)
{
    timeval_t end = get_time();
    return (end.tv_sec - start.tv_sec) * 1000
        + (end.tv_usec - start.tv_usec) / 1000;
}

/**
 * @brief return a string representation 10min 30sec for example
 * 
 * @param lapse time lapse in ms
 * @return const char*  string representation
 */
const char *get_time_string(msseconds_t lapse)
{
    static char buffer[1024];
    int seconds = lapse / 1000;
    int minutes = seconds / 60;

    if (minutes > 0)
        sprintf(buffer, "%d min %d sec", minutes, seconds % 60);
    else if (seconds > 0)
        sprintf(buffer, "%d sec", seconds % 60);
    else
        sprintf(buffer, "%d ms", lapse);
    return buffer;
}
