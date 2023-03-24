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

#include <sys/poll.h>
#include <bits/types/__sigset_t.h>

#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, poll, EAGAIN, -1, struct pollfd *, fds, nfds_t, nfds, int, timeout);
DEFINE_HOOK_FUNCTION(int, ppoll, EAGAIN, -1, struct pollfd *, fds, nfds_t, nfds, const struct timespec *, timeout, const __sigset_t *, sigmask);
