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

#include "../shared_memory/shared_memory.h"
#include "../events/event_sender.h"
#include "../shared_memory/shared_memory.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Close the shared memory on exit
 * 
 */
static void exit_hook(void)
{
	close_shared_memory();
}

/**
 * @brief Setup the exit hook
 * 
 */
void setup_exit_hook(void)
{
	atexit(exit_hook);
}