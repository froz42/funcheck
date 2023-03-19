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
	char function_name[256];
	bool_t should_test;
	t_allocation_data allocation;
	t_event event;
	sem_t lock_guest;
	sem_t lock_host;
	char treat_abort_as_crash;
} t_shared_info;

#endif