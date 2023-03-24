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
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(__sighandler_t, sysv_signal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);
DEFINE_HOOK_FUNCTION(__sighandler_t, signal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);
DEFINE_HOOK_FUNCTION(__sighandler_t, bsd_signal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);
DEFINE_HOOK_FUNCTION(int, kill, EAGAIN, -1, pid_t, pid, int, sig);
DEFINE_HOOK_FUNCTION(int, killpg, EAGAIN, -1, pid_t, pgrp, int, sig);
DEFINE_HOOK_FUNCTION(int, raise, EAGAIN, -1, int, sig);
DEFINE_HOOK_FUNCTION(__sighandler_t, ssignal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);