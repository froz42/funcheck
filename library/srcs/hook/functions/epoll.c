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

#include <sys/epoll.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, epoll_create, EAGAIN, -1, int, size);
DEFINE_HOOK_FUNCTION(int, epoll_create1, EAGAIN, -1, int, flags);
DEFINE_HOOK_FUNCTION(int, epoll_ctl, EAGAIN, -1, int, epfd, int, op, int, fd, struct epoll_event *, event);
DEFINE_HOOK_FUNCTION(int, epoll_wait, EAGAIN, -1, int, epfd, struct epoll_event *, events, int, maxevents, int, timeout);
DEFINE_HOOK_FUNCTION(int, epoll_pwait, EAGAIN, -1, int, epfd, struct epoll_event *, events, int, maxevents, int, timeout, const __sigset_t *, sigmask);
DEFINE_HOOK_FUNCTION(int, epoll_pwait2, EAGAIN, -1, int, epfd, struct epoll_event *, events, int, maxevents, const struct timespec *, timeout, const __sigset_t *, sigmask);
