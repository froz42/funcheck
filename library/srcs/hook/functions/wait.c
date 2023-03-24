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

#include <sys/wait.h>
#include <sys/types.h>
#include <bits/types/siginfo_t.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(__pid_t, wait, EAGAIN, -1, int *, status);
DEFINE_HOOK_FUNCTION(__pid_t, waitpid, EAGAIN, -1, __pid_t, pid, int *, status, int, options);
DEFINE_HOOK_FUNCTION(int, waitid, EAGAIN, -1, idtype_t, idtype, __id_t, id, siginfo_t *, infop, int, options);
DEFINE_HOOK_FUNCTION(__pid_t, wait3, EAGAIN, -1, int *, status, int, options, struct rusage *, rusage);
DEFINE_HOOK_FUNCTION(__pid_t, wait4, EAGAIN, -1, __pid_t, pid, int *, status, int, options, struct rusage *, rusage);
