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

#include <semaphore.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, sem_init, EAGAIN, -1, sem_t *, sem, int, pshared, unsigned int, value);
DEFINE_HOOK_FUNCTION(int, sem_destroy, EAGAIN, -1, sem_t *, sem);

sem_t *EXPORT sem_open(const char *name, int oflag, ...)
{
    if (!is_hooks_enabled())
    {
        typeof(&sem_open) sem_open_original = get_original_function();
        va_list ap;
        va_start(ap, oflag);
        mode_t mode = va_arg(ap, mode_t);
        unsigned int value = va_arg(ap, unsigned int);
        sem_t *ret = sem_open_original(name, oflag, mode, value);
        va_end(ap);
        return ret;
    }
    disable_function_hooks();
    set_hooked_function_name(__func__);
    t_shared_info *shared_memory = get_shared_memory();
    get_backtrace(shared_memory->backtrace);
    if (should_block(shared_memory->backtrace))
    {
        errno = EACCES;
        enable_function_hooks();
        return SEM_FAILED;
    }
    typeof(&sem_open) sem_open_original = get_original_function();
    va_list ap;
    va_start(ap, oflag);
    mode_t mode = va_arg(ap, mode_t);
    unsigned int value = va_arg(ap, unsigned int);
    send_function_call_event(shared_memory);
    sem_t *ret = sem_open_original(name, oflag, mode, value);
    va_end(ap);
    set_hooked_function_name(NULL);
    enable_function_hooks();
    return ret;
}

DEFINE_HOOK_FUNCTION(int, sem_unlink, EAGAIN, -1, const char *, name);
DEFINE_HOOK_FUNCTION(int, sem_wait, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_trywait, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_post, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_getvalue, EAGAIN, -1, sem_t *, sem, int *, sval);
