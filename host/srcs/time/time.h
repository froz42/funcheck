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
