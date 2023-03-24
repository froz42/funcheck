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