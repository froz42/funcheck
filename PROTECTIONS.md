# Protections

This document list the tested functions in the program. If a function is not listed, it means that it is not tested yet. You can help us by adding it to the code.

## How to add a new functions

1. Create a new file `library/srcs/hook/functions/<header_where_the_function_is_declared>.c`
2. Include the hook header and the header where the function is declared

   ```c
   #include "../hook.h"
   #include <header_where_the_function_is_declared.h>
   ```

3. Use the macro `DEFINE_HOOK_FUNCTION` to define the hook function like this:

   ```c
   DEFINE_HOOK_FUNCTION(<return_type>, <function_name>, <error_no_to_set>, <error_return_value>, <arg1_type>, <arg1_name>, <arg2_type>, <arg2_name>, ...)
   ```

   For example, if you want to hook the `read` function, you can do this:

   ```c
   DEFINE_HOOK_FUNCTION(ssize_t, read, EIO, -1, int, fd, void *, buf, size_t, count)
   ```

   **if you want to hook a function that returns a pointer, you have to make a typedef for the return type and use it in the macro.**

   For example, if you want to hook the `strdup` function, you can do this:

   ```c
   typedef char *char_ptr_t;
   DEFINE_HOOK_FUNCTION(char_ptr_t, strdup, ENOMEM, NULL, const char *, s)
   ```

4. Add the file to the Makefile

If you want to add a function that take a variable number of arguments, look hot it's done for open in `library/srcs/hook/functions/fcntl.c`.

## Tested functions

### allocs

- malloc
- calloc
- realloc

### unistd.h

- access
- euidaccess
- execveat
- faccessat
- lseek
- lseek64
- close
- read
- write
- pread
- pwrite
- pread64
- pwrite64
- pipe
- pipe2
- usleep
- pause
- chown
- fchown
- lchown
- fchownat
- chdir
- fchdir
- getcwd
- get_current_dir_name
- dup
- dup2
- dup3
- execve
- confstr
- getpid
- getppid
- getpgrp
- getpgid
- setpgid
- setsid
- getuid
- geteuid
- getgid
- getegid
- setuid
- setgid
- seteuid
- getresuid
- getresgid
- setresuid
- setresgid
- fork
- vfork
- ttyname
- ttyname_r
- isatty
- ttyslot
- link
- linkat
- symlink
- symlinkat
- readlink
- readlinkat
- unlink
- unlinkat
- rmdir
- tcgetpgrp
- tcsetpgrp
- getlogin
- getlogin_r
- setlogin
- gethostname
- sethostname
- sethostid
- getdomainname
- setdomainname
- vhangup
- revoke
- profil
- acct
- getusershell
- deamon
- chroot
- getpass
- fsync
- syncfs
- truncate
- truncate64
- ftruncate
- ftruncate64
- brk
- sbrk

### stdlib.h

- initstate
- setstate
- random_r
- srandom_r
- initstate_r
- setstate_r
- valloc
- on_exit
- at_quick_exit
- on_quick_exit
- getenv
- putenv
- setenv
- unsetenv
- mktemp
- mkstemp
- mkstemps
- mkostemp
- mkostemps
- system
- realpath
- posix_openpt
- grantpt
- unlockpt
- ptsname
- ptsname_r
- getpt

### stdio.h

- remove
- rename
- renameat
- renameat2
- fclose
- tmpfile
- tmpfile64
- tmpnam
- tmpnam_r
- tempnam
- fflush
- fflush_unlocked
- fcloseall
- fopen
- freopen
- fopen64
- freopen64
- fdopen
- fopencookie
- fmemopen
- open_memstream
- open_wmemstream
- setvbuf
- vfprintf
- vprintf
- vsprintf
- vsnprintf
- vasprintf
- vdprintf
- vfscanf
- vscanf
- vsscanf
- fgetc
- getc
- getchar
- fgetc_unlocked
- getc_unlocked
- getchar_unlocked
- fputc
- putc
- putchar
- fputc_unlocked
- putc_unlocked
- putchar_unlocked
- getw
- putw
- fgets
- gets
- fgets_unlocked
- getdelim
- getline
- fputs
- puts
- ungetc
- fread
- fwrite
- fputs_unlocked
- fread_unlocked
- fwrite_unlocked
- fseek
- ftell
- fseeko
- ftello
- fgetpos
- fsetpos
- fseeko64
- ftello64
- fgetpos64
- fsetpos64
- feof
- ferror
- fileno
- fileno_unlocked
- pclose
- popen
- ctermid
- cuserid
- obstack_vprintf
- ftrylockfile

### fcntl.h

- fcntl
- open
- openat
- creat
- lockf
- posix_fadvise
- posix_fallocate

### stat.h

- stat
- fstat
- stat64
- fstat64
- lstat
- lstat64
- fstatat
- fstatat64
- chmod
- fchmod
- lchmod
- fchmodat
- umask
- mkdir
- mkdirat
- mknod
- mknodat
- mkfifo
- mkfifoat
- utimensat
- futimens

### mman.h

- mmap
- mmap64
- munmap
- mprotect
- msync
- madvise
- posix_madvise
- mlock
- munlock
- mlockall
- munlockall
- mincore
- shm_open
- shm_unlink

### wait.h

- wait
- waitpid
- waitid
- wait3
- wait4

### string.h

- strdup
- strndup

### signal.h

- sysv_signal
- signal
- bsd_signal
- kill
- killpg
- raise
- ssignal

### socket.h

- socket
- socketpair
- bind
- getsockname
- connect
- getpeername
- send
- recv
- sendto
- recvfrom
- sendmsg
- recvmsg
- getsockopt
- setsockopt
- listen
- accept
- accept4
- shutdown
- sockatmark
- isfdtype

### semaphore.h

- sem_open
- sem_init
- sem_destroy
- sem_close
- sem_unlink
- sem_wait
- sem_trywait
- sem_post
- sem_getvalue

### poll.h

- poll
- ppoll

### select.h

- select
- pselect
