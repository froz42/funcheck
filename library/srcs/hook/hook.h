#ifndef HOOK_H
#define HOOK_H

#include <stdlib.h>

#include "../backtrace/backtrace.h"
#include "../utils/bool.h"

void set_backtrace_to_block(ptr_address *backtrace, size_t iteration_to_block);

void disable_hooks(void);
void enable_hooks(void);
_bool is_hooks_enabled(void);

_bool should_block(ptr_address *backtrace);

void setup_exit_hook(void);
void setup_signals(void);

void setup_hooks(void);

#endif