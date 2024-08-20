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

#include <unistd.h>
#include "../hook.h"

/**
 * @brief This file contain hooks definition for unistd.h functions
 *
 */

typedef int pipe_t[2];
typedef char *char_ptr_t;
typedef void *void_ptr_t;
typedef int *int_ptr_t;

DEFINE_HOOK_FUNCTION(int, access, EACCES, -1, const char *, name, int, type);
DEFINE_HOOK_FUNCTION(int, euidaccess, EACCES, -1, const char *, name, int, type);
DEFINE_HOOK_FUNCTION(int, execveat, EACCES, -1, int, dirfd, const char *, name, char *const *, argv, char *const *, envp, int, flags);
DEFINE_HOOK_FUNCTION(int, faccessat, EACCES, -1, int, dirfd, const char *, name, int, type, int, flags);

DEFINE_HOOK_FUNCTION(__off_t, lseek, EIO, -1, int, fd, __off_t, offset, int, whence);
DEFINE_HOOK_FUNCTION(__off64_t, lseek64, EIO, -1, int, fd, __off64_t, offset, int, whence);

DEFINE_HOOK_FUNCTION(ssize_t, read, EIO, -1, int, fd, void *, buf, size_t, count);
DEFINE_HOOK_FUNCTION(ssize_t, write, EIO, -1, int, fd, const void *, buf, size_t, count);

DEFINE_HOOK_FUNCTION(ssize_t, pread, EIO, -1, int, fd, void *, buf, size_t, count, __off_t, offset);
DEFINE_HOOK_FUNCTION(ssize_t, pwrite, EIO, -1, int, fd, const void *, buf, size_t, count, __off_t, offset);

DEFINE_HOOK_FUNCTION(ssize_t, pread64, EIO, -1, int, fd, void *, buf, size_t, count, __off64_t, offset);
DEFINE_HOOK_FUNCTION(ssize_t, pwrite64, EIO, -1, int, fd, const void *, buf, size_t, count, __off64_t, offset);

DEFINE_HOOK_FUNCTION(int, pipe, EAGAIN, -1, pipe_t, pipefd);
DEFINE_HOOK_FUNCTION(int, pipe2, EAGAIN, -1, pipe_t, pipefd, int, flags);

DEFINE_HOOK_FUNCTION(int, usleep, EAGAIN, -1, __useconds_t, usec);
DEFINE_HOOK_FUNCTION(int, pause, EAGAIN, -1);

DEFINE_HOOK_FUNCTION(int, chown, EACCES, -1, const char *, path, uid_t, owner, gid_t, group);
DEFINE_HOOK_FUNCTION(int, fchown, EACCES, -1, int, fd, uid_t, owner, gid_t, group);
DEFINE_HOOK_FUNCTION(int, lchown, EACCES, -1, const char *, path, uid_t, owner, gid_t, group);
DEFINE_HOOK_FUNCTION(int, fchownat, EACCES, -1, int, dirfd, const char *, path, uid_t, owner, gid_t, group, int, flags);

DEFINE_HOOK_FUNCTION(int, chdir, EACCES, -1, const char *, path);
DEFINE_HOOK_FUNCTION(int, fchdir, EACCES, -1, int, fd);
DEFINE_HOOK_FUNCTION(char_ptr_t, getcwd, ENOMEM, NULL, char *, buf, size_t, size);

DEFINE_HOOK_FUNCTION(char_ptr_t, get_current_dir_name, ENOMEM, NULL);

DEFINE_HOOK_FUNCTION(int, dup, EAGAIN, -1, int, fd);
DEFINE_HOOK_FUNCTION(int, dup2, EAGAIN, -1, int, fd, int, fd2);
DEFINE_HOOK_FUNCTION(int, dup3, EAGAIN, -1, int, fd, int, fd2, int, flags);

DEFINE_HOOK_FUNCTION(int, execve, EACCES, -1, const char *, path, char *const *, argv, char *const *, envp);

DEFINE_HOOK_FUNCTION(long int, pathconf, EACCES, -1, const char *, path, int, name);
DEFINE_HOOK_FUNCTION(long int, fpathconf, EACCES, -1, int, fd, int, name);
DEFINE_HOOK_FUNCTION(long int, sysconf, EACCES, -1, int, name);
DEFINE_HOOK_FUNCTION(size_t, confstr, EACCES, -1, int, name, char *, buf, size_t, len);

DEFINE_HOOK_FUNCTION(__pid_t, getpid, EACCES, -1);
DEFINE_HOOK_FUNCTION(__pid_t, getppid, EACCES, -1);
DEFINE_HOOK_FUNCTION(__pid_t, getpgrp, EACCES, -1);
DEFINE_HOOK_FUNCTION(__pid_t, getpgid, EACCES, -1, __pid_t, pid);

DEFINE_HOOK_FUNCTION(int, setpgid, EACCES, -1, __pid_t, pid, __pid_t, pgid);
DEFINE_HOOK_FUNCTION(__pid_t, setsid, EACCES, -1);
DEFINE_HOOK_FUNCTION(__uid_t, getuid, EACCES, -1);
DEFINE_HOOK_FUNCTION(__uid_t, geteuid, EACCES, -1);
DEFINE_HOOK_FUNCTION(__gid_t, getgid, EACCES, -1);
DEFINE_HOOK_FUNCTION(__gid_t, getegid, EACCES, -1);

DEFINE_HOOK_FUNCTION(int, setuid, EACCES, -1, __uid_t, uid);
DEFINE_HOOK_FUNCTION(int, setgid, EACCES, -1, __gid_t, gid);
DEFINE_HOOK_FUNCTION(int, seteuid, EACCES, -1, __uid_t, uid);
DEFINE_HOOK_FUNCTION(int, getresuid, EACCES, -1, __uid_t *, ruid, __uid_t *, euid, __uid_t *, suid);
DEFINE_HOOK_FUNCTION(int, getresgid, EACCES, -1, __gid_t *, rgid, __gid_t *, egid, __gid_t *, sgid);
DEFINE_HOOK_FUNCTION(int, setresuid, EACCES, -1, __uid_t, ruid, __uid_t, euid, __uid_t, suid);
DEFINE_HOOK_FUNCTION(int, setresgid, EACCES, -1, __uid_t, ruid, __uid_t, euid, __uid_t, suid);

DEFINE_HOOK_FUNCTION(__pid_t, fork, EAGAIN, -1);
DEFINE_HOOK_FUNCTION(__pid_t, vfork, EAGAIN, -1);

DEFINE_HOOK_FUNCTION(char_ptr_t, ttyname, EACCES, NULL, int, fd);
DEFINE_HOOK_FUNCTION(int, ttyname_r, EACCES, -1, int, fd, char *, buf, size_t, buflen);
DEFINE_HOOK_FUNCTION(int, isatty, EACCES, -1, int, fd);

DEFINE_HOOK_FUNCTION(int, ttyslot, EACCES, -1);
DEFINE_HOOK_FUNCTION(int, link, EACCES, -1, const char *, oldpath, const char *, newpath);
DEFINE_HOOK_FUNCTION(int, linkat, EACCES, -1, int, olddirfd, const char *, oldpath, int, newdirfd, const char *, newpath, int, flags);
DEFINE_HOOK_FUNCTION(int, symlink, EACCES, -1, const char *, oldpath, const char *, newpath);
DEFINE_HOOK_FUNCTION(int, symlinkat, EACCES, -1, const char *, oldpath, int, newdirfd, const char *, newpath);
DEFINE_HOOK_FUNCTION(ssize_t, readlink, EACCES, -1, const char *, path, char *, buf, size_t, bufsiz);
DEFINE_HOOK_FUNCTION(ssize_t, readlinkat, EACCES, -1, int, dirfd, const char *, path, char *, buf, size_t, bufsiz);
DEFINE_HOOK_FUNCTION(int, unlink, EACCES, -1, const char *, pathname);
DEFINE_HOOK_FUNCTION(int, unlinkat, EACCES, -1, int, dirfd, const char *, pathname, int, flags);
DEFINE_HOOK_FUNCTION(int, rmdir, EACCES, -1, const char *, pathname);

DEFINE_HOOK_FUNCTION(__pid_t, tcgetpgrp, EACCES, -1, int, fd);
DEFINE_HOOK_FUNCTION(int, tcsetpgrp, EACCES, -1, int, fd, __pid_t, pgrp_id);

DEFINE_HOOK_FUNCTION(char_ptr_t, getlogin, EACCES, NULL);
DEFINE_HOOK_FUNCTION(int, getlogin_r, EACCES, -1, char *, name, size_t, namesize);

DEFINE_HOOK_FUNCTION(int, setlogin, EACCES, -1, const char *, name);

DEFINE_HOOK_FUNCTION(int, gethostname, EACCES, -1, char *, name, size_t, len);
DEFINE_HOOK_FUNCTION(int, sethostname, EACCES, -1, const char *, name, size_t, len);

DEFINE_HOOK_FUNCTION(int, sethostid, EACCES, -1, long int, id);
DEFINE_HOOK_FUNCTION(int, getdomainname, EACCES, -1, char *, name, size_t, len);
DEFINE_HOOK_FUNCTION(int, setdomainname, EACCES, -1, const char *, name, size_t, len);

DEFINE_HOOK_FUNCTION(int, vhangup, EACCES, -1);
DEFINE_HOOK_FUNCTION(int, revoke, EACCES, -1, const char *, file);
DEFINE_HOOK_FUNCTION(int, profil, EACCES, -1, unsigned short *, sample_buffer, size_t, size, size_t, offset, unsigned int, scale);

DEFINE_HOOK_FUNCTION(int, acct, EACCES, -1, const char *, filename);
DEFINE_HOOK_FUNCTION(char_ptr_t, getusershell, EACCES, NULL);
DEFINE_HOOK_FUNCTION(int, deamon, EACCES, -1, int, nochdir, int, noclose);
DEFINE_HOOK_FUNCTION(int, chroot, EACCES, -1, const char *, path);
DEFINE_HOOK_FUNCTION(char_ptr_t, getpass, EACCES, NULL, const char *, prompt);
DEFINE_HOOK_FUNCTION(int, fsync, EACCES, -1, int, fd);
DEFINE_HOOK_FUNCTION(int, syncfs, EACCES, -1, int, fd);

DEFINE_HOOK_FUNCTION(int, truncate, EACCES, -1, const char *, path, __off_t, length);
DEFINE_HOOK_FUNCTION(int, truncate64, EACCES, -1, const char *, path, __off64_t, length);
DEFINE_HOOK_FUNCTION(int, ftruncate, EACCES, -1, int, fd, __off_t, length);
DEFINE_HOOK_FUNCTION(int, ftruncate64, EACCES, -1, int, fd, __off64_t, length);

DEFINE_HOOK_FUNCTION(int, brk, EACCES, -1, void *, addr);

DEFINE_HOOK_FUNCTION(void_ptr_t, sbrk, EACCES, (void *)-1, long int, increment);

