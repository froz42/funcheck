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

#include <sys/select.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, select, EAGAIN, -1, int, nfds, fd_set *, readfds, fd_set *, writefds, fd_set *, exceptfds, struct timeval *, timeout);
DEFINE_HOOK_FUNCTION(int, pselect, EAGAIN, -1, int, nfds, fd_set *, readfds, fd_set *, writefds, fd_set *, exceptfds, const struct timespec *, timeout, const __sigset_t *, sigmask);