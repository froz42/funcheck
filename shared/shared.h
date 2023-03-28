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

#ifndef SHARED_H
#define SHARED_H

#include "bool.h"
#include <semaphore.h>

#define ENV_MEMORY_NAME "funcheck_MEMORY_NAME"

typedef enum e_runtype
{
	RUNTYPE_UNDEFINED,
	RUNTYPE_FETCH,
	RUNTYPE_TEST
} t_runtype;

#define MAX_BACKTRACE_DEPTH 100

typedef enum e_event
{
	NONE,
	ALLOC,
	REMOVE_ALLOC,
	FUNCTION_CALL,
	CRASH,
	EXIT
} t_event;

typedef unsigned long ptr_address;

typedef struct allocation_data_s
{
	size_t size;
	void *ptr;
} t_allocation_data;

typedef struct s_shared_info
{
	t_runtype runtype;
	ptr_address backtrace[MAX_BACKTRACE_DEPTH];
	size_t iteration_to_test;
	char function_name[256];
	bool_t should_test;
	t_allocation_data allocation;
	t_event event;
	sem_t lock_guest;
	sem_t lock_host;
	char treat_abort_as_crash;
} t_shared_info;

#endif