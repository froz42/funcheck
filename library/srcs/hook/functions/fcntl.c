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

#include <fcntl.h>
#include "../hook.h"

typedef long long int universal_param_t;

int EXPORT fcntl(int fd, int cmd, ...)
{
    if (!is_hooks_enabled())
    {
        typeof(&fcntl) fcntl_original = get_original_function();
        va_list ap;
        va_start(ap, cmd);
        universal_param_t arg = va_arg(ap, universal_param_t);
        int ret = fcntl_original(fd, cmd, arg);
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
        return -1;
    }
    typeof(&fcntl) fcntl_original = get_original_function();
    va_list ap;
    va_start(ap, cmd);
    send_function_call_event(shared_memory);
    int ret = fcntl_original(fd, cmd);
    va_end(ap);
    set_hooked_function_name(NULL);
    enable_function_hooks();
    return ret;
}

int EXPORT open(const char *pathname, int flags, ...)
{
    if (!is_hooks_enabled())
    {
        typeof(&open) open_original = get_original_function();
        va_list ap;
        va_start(ap, flags);
        mode_t mode = va_arg(ap, mode_t);
        int ret = open_original(pathname, flags, mode);
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
        return -1;
    }
    typeof(&open) open_original = get_original_function();
    va_list ap;
    va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);
    send_function_call_event(shared_memory);
    int ret = open_original(pathname, flags, mode);
    va_end(ap);
    set_hooked_function_name(NULL);
    enable_function_hooks();
    return ret;
}

int EXPORT openat(int fd, const char *file, int oflag, ...)
{
    if (!is_hooks_enabled())
    {
        typeof(&openat) openat_original = get_original_function();
        va_list ap;
        va_start(ap, oflag);
        mode_t mode = va_arg(ap, mode_t);
        int ret = openat_original(fd, file, oflag, mode);
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
        return -1;
    }
    typeof(&openat) openat_original = get_original_function();
    va_list ap;
    va_start(ap, oflag);
    mode_t mode = va_arg(ap, mode_t);
    send_function_call_event(shared_memory);
    int ret = openat_original(fd, file, oflag, mode);
    va_end(ap);
    set_hooked_function_name(NULL);
    enable_function_hooks();
    return ret;
}

DEFINE_HOOK_FUNCTION(int, creat, EACCES, -1, const char *, file, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, lockf, EACCES, -1, int, fd, int, cmd, off_t, len);
DEFINE_HOOK_FUNCTION(int, posix_fadvise, EACCES, -1, int, fd, off_t, offset, off_t, len, int, advice);
DEFINE_HOOK_FUNCTION(int, posix_fallocate, EACCES, -1, int, fd, off_t, offset, off_t, len);
