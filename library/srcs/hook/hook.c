#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include "hook.h"
#include "../utils/bool.h"
#include "../utils/export.h"
#include "../backtrace/backtrace.h"
#include "../utils/error.h"

static ptr_address *_backtrace_to_block = NULL;
static size_t _iteration_to_block;

static size_t _current_iteration;

static bool_t _function_hooks_enabled = false;
static bool_t _alloc_hooks_enabled = false;

static const char *_hooked_function_name = NULL;

void set_hooked_function_name(const char *function_name)
{
	_hooked_function_name = function_name;
}

const char *get_hooked_function_name(void)
{
	return _hooked_function_name;
}

/**
 * @brief set the backtrace to block and the iteration to block
 * 
 * @param backtrace backtrace to block
 * @param iteration_to_block iteration to block
 */
void set_backtrace_to_block(ptr_address *backtrace, size_t iteration_to_block)
{
	size_t backtrace_length = 0;
	while (backtrace[backtrace_length] != 0x0)
		backtrace_length++;
	_backtrace_to_block = malloc(sizeof(ptr_address) * (backtrace_length + 1));
	if (!_backtrace_to_block)
		raise_error("set_backtrace_to_block: malloc", true);

	for (size_t i = 0; i < backtrace_length; i++)
		_backtrace_to_block[i] = backtrace[i];
	_backtrace_to_block[backtrace_length] = 0x0;

	_iteration_to_block = iteration_to_block;
	_current_iteration = 0;
}

void disable_function_hooks(void)
{
	_function_hooks_enabled = false;
}

void enable_function_hooks(void)
{
	_function_hooks_enabled = true;
}

void disable_alloc_hooks(void)
{
	_alloc_hooks_enabled = false;
}

void enable_alloc_hooks(void)
{
	_alloc_hooks_enabled = true;
}

void disable_function_and_alloc_hooks(void)
{
	disable_function_hooks();
	disable_alloc_hooks();
}

void enable_function_and_alloc_hooks(void)
{
	enable_function_hooks();
	enable_alloc_hooks();
}

bool_t is_function_hooks_enabled(void)
{
	return _function_hooks_enabled;
}

bool_t is_alloc_hooks_enabled(void)
{
	return _alloc_hooks_enabled;
}

bool_t is_hooks_enabled(void)
{
	return _function_hooks_enabled || _alloc_hooks_enabled;
}


/**
 * @brief Check if the backtrace should be blocked
 * 
 * @param backtrace the backtrace to check
 * @return bool_t true if the backtrace should be blocked, false otherwise
 */
bool_t should_block(ptr_address *backtrace)
{
	// compare backtrace_to_block with backtrace
	if (_backtrace_to_block == NULL)
		return false;
	size_t i = 0;
	while (_backtrace_to_block[i] != 0x0)
	{
		if (_backtrace_to_block[i] != backtrace[i])
			return false;
		i++;
	}
	if (_iteration_to_block++ == _current_iteration)
		return true;
	return false;
}

/**
 * @brief Set the up hooks object (signals and exit hook)
 * 
 */
void setup_hooks(void)
{
	setup_signals();
	setup_exit_hook();
}