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

static _bool _hooks_enabled = false;

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

void disable_hooks(void)
{
	_hooks_enabled = false;
}

void enable_hooks(void)
{
	_hooks_enabled = true;
}

_bool is_hooks_enabled(void)
{
	return _hooks_enabled;
}

_bool should_block(ptr_address *backtrace)
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

void setup_hooks(void)
{
	setup_signals();
	setup_exit_hook();
}