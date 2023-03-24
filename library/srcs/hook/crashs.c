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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared_memory/shared_memory.h"
#include "../events/event_sender.h"
#include "../backtrace/backtrace.h"
#include "./hook.h"
#include "../utils/error.h"

/**
 * @brief Signal handler hook
 * 
 * @param signo the signal number
 */
static void sig_handler(int signo)
{
	disable_function_and_alloc_hooks();
	t_shared_info *shared_memory = get_shared_memory();

	if (!(!shared_memory->treat_abort_as_crash && signo == SIGABRT))
	{
		strncpy(
			shared_memory->function_name,
			strsignal(signo),
			sizeof(shared_memory->function_name));
		get_backtrace(shared_memory->backtrace);
		shared_memory->event = CRASH;
	}
	exit(signo + 128);
}

/**
 * @brief Setup signal handlers
 * 
 */
void setup_signals(void)
{
	// handle all signals that crash the program
	static const int signals[] = {
		SIGSEGV,
		SIGBUS,
		SIGILL,
		SIGFPE,
		SIGABRT,
	};

	for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++)
	{
		if (signal(signals[i], sig_handler) == SIG_ERR)
			raise_error("setup_signals: signal", true);
	}
}
