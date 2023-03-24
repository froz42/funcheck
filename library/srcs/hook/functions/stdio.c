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

#include <stdio.h>
#include <errno.h>
#include "../hook.h"

typedef FILE *file_ptr_t;
typedef char *char_ptr_t;
typedef char char_array_L_tmpnam_t[L_tmpnam];

DEFINE_HOOK_FUNCTION(int, remove, EACCES, -1, const char *, path);
DEFINE_HOOK_FUNCTION(int, rename, EACCES, -1, const char *, oldpath, const char *, newpath);
DEFINE_HOOK_FUNCTION(int, renameat, EACCES, -1, int, olddirfd, const char *, oldpath, int, newdirfd, const char *, newpath);
DEFINE_HOOK_FUNCTION(int, renameat2, EACCES, -1, int, olddirfd, const char *, oldpath, int, newdirfd, const char *, newpath, unsigned int, flags);
DEFINE_HOOK_FUNCTION(int, fclose, EIO, EOF, FILE *, stream);
DEFINE_HOOK_FUNCTION(file_ptr_t, tmpfile, ENOSPC, NULL);
DEFINE_HOOK_FUNCTION(file_ptr_t, tmpfile64, ENOSPC, NULL);
DEFINE_HOOK_FUNCTION(char_ptr_t, tmpnam, ENOSPC, NULL, char_array_L_tmpnam_t, s);
DEFINE_HOOK_FUNCTION(char_ptr_t, tmpnam_r, ENOSPC, NULL, char_array_L_tmpnam_t, s);
DEFINE_HOOK_FUNCTION(char_ptr_t, tempnam, ENOSPC, NULL, const char *, dir, const char *, pfx);
DEFINE_HOOK_FUNCTION(int, fflush, EIO, EOF, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, fflush_unlocked, EIO, EOF, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, fcloseall, EIO, EOF);

DEFINE_HOOK_FUNCTION(file_ptr_t, fopen, EACCES, NULL, const char *, path, const char *, mode);
DEFINE_HOOK_FUNCTION(file_ptr_t, freopen, EACCES, NULL, const char *, path, const char *, mode, FILE *, stream);
DEFINE_HOOK_FUNCTION(file_ptr_t, fopen64, EACCES, NULL, const char *, path, const char *, mode);
DEFINE_HOOK_FUNCTION(file_ptr_t, freopen64, EACCES, NULL, const char *, path, const char *, mode, FILE *, stream);
DEFINE_HOOK_FUNCTION(file_ptr_t, fdopen, EACCES, NULL, int, fd, const char *, mode);

#include <bits/types/cookie_io_functions_t.h>

DEFINE_HOOK_FUNCTION(file_ptr_t, fopencookie, EACCES, NULL, void *, cookie, const char *, mode, cookie_io_functions_t, io_funcs);
DEFINE_HOOK_FUNCTION(file_ptr_t, fmemopen, EACCES, NULL, void *, buf, size_t, size, const char *, mode);
DEFINE_HOOK_FUNCTION(file_ptr_t, open_memstream, EACCES, NULL, char **, bufp, size_t *, sizep);
DEFINE_HOOK_FUNCTION(file_ptr_t, open_wmemstream, EACCES, NULL, wchar_t **, bufp, size_t *, sizep);
DEFINE_HOOK_FUNCTION(int, setvbuf, EACCES, -1, FILE *, stream, char *, buf, int, mode, size_t, size);

/* printf family */


DEFINE_HOOK_FUNCTION(int, vfprintf, EIO, -1, FILE *, stream, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vprintf, EIO, -1, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vsprintf, EIO, -1, char *, str, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vsnprintf, EIO, -1, char *, str, size_t, size, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vasprintf, EIO, -1, char *, str, size_t, size, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vdprintf, EIO, -1, int, fd, const char *, format, __gnuc_va_list, arg);

/* scanf family */

DEFINE_HOOK_FUNCTION(int, vfscanf, EIO, -1, FILE *, stream, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vscanf, EIO, -1, const char *, format, __gnuc_va_list, arg);
DEFINE_HOOK_FUNCTION(int, vsscanf, EIO, -1, const char *, str, const char *, format, __gnuc_va_list, arg);

DEFINE_HOOK_FUNCTION(int, fgetc, EIO, EOF, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, getc, EIO, EOF, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, getchar, EIO, EOF);

DEFINE_HOOK_FUNCTION(int, fgetc_unlocked, EIO, EOF, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, getc_unlocked, EIO, EOF, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, getchar_unlocked, EIO, EOF);

DEFINE_HOOK_FUNCTION(int, fputc, EIO, EOF, int, c, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, putc, EIO, EOF, int, c, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, putchar, EIO, EOF, int, c);

DEFINE_HOOK_FUNCTION(int, fputc_unlocked, EIO, EOF, int, c, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, putc_unlocked, EIO, EOF, int, c, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, putchar_unlocked, EIO, EOF, int, c);

DEFINE_HOOK_FUNCTION(int, getw, EIO, EOF, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, putw, EIO, EOF, int, w, FILE *, stream);

DEFINE_HOOK_FUNCTION(char_ptr_t, fgets, EIO, NULL, char *, s, int, size, FILE *, stream);
DEFINE_HOOK_FUNCTION(char_ptr_t, gets, EIO, NULL, char *, s);

DEFINE_HOOK_FUNCTION(char_ptr_t, fgets_unlocked, EIO, NULL, char *, s, int, size, FILE *, stream);

DEFINE_HOOK_FUNCTION(__ssize_t, getdelim, EIO, -1, char **, lineptr, size_t *, n, int, delim, FILE *, stream);
DEFINE_HOOK_FUNCTION(__ssize_t, getline, EIO, -1, char **, lineptr, size_t *, n, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, fputs, EIO, EOF, const char *, s, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, puts, EIO, EOF, const char *, s);

DEFINE_HOOK_FUNCTION(int, ungetc, EIO, EOF, int, c, FILE *, stream);

DEFINE_HOOK_FUNCTION(size_t, fread, EIO, 0, void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);
DEFINE_HOOK_FUNCTION(size_t, fwrite, EIO, 0, const void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, fputs_unlocked, EIO, 0, const char *, s, FILE *, stream);
DEFINE_HOOK_FUNCTION(size_t, fread_unlocked, EIO, 0, void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);
DEFINE_HOOK_FUNCTION(size_t, fwrite_unlocked, EIO, 0, const void *, ptr, size_t, size, size_t, nmemb, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, fseek, EIO, -1, FILE *, stream, long, offset, int, whence);
DEFINE_HOOK_FUNCTION(long, ftell, EIO, -1, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, fseeko, EIO, -1, FILE *, stream, off_t, offset, int, whence);
DEFINE_HOOK_FUNCTION(off_t, ftello, EIO, -1, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, fgetpos, EIO, -1, FILE *, stream, fpos_t *, pos);
DEFINE_HOOK_FUNCTION(int, fsetpos, EIO, -1, FILE *, stream, const fpos_t *, pos);

DEFINE_HOOK_FUNCTION(int, fseeko64, EIO, -1, FILE *, stream, __off64_t, offset, int, whence);
DEFINE_HOOK_FUNCTION(__off64_t, ftello64, EIO, -1, FILE *, stream);

typedef fpos_t fpos64_t;

DEFINE_HOOK_FUNCTION(int, fgetpos64, EIO, -1, FILE *, stream, fpos64_t *, pos);
DEFINE_HOOK_FUNCTION(int, fsetpos64, EIO, -1, FILE *, stream, const fpos64_t *, pos);

DEFINE_HOOK_FUNCTION(int, feof, EIO, 0, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, ferror, EIO, 0, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, fileno, EIO, -1, FILE *, stream);
DEFINE_HOOK_FUNCTION(int, fileno_unlocked, EIO, -1, FILE *, stream);

DEFINE_HOOK_FUNCTION(int, pclose, EIO, -1, FILE *, stream);

DEFINE_HOOK_FUNCTION(file_ptr_t, popen, EIO, NULL, const char *, command, const char *, type);

DEFINE_HOOK_FUNCTION(char_ptr_t, ctermid, EIO, NULL, char *, s);
DEFINE_HOOK_FUNCTION(char_ptr_t, cuserid, EIO, NULL, char *, s);

typedef struct obstack *obstack_ptr_t;

DEFINE_HOOK_FUNCTION(int, obstack_vprintf, EIO, -1, obstack_ptr_t, h, const char *, format, __gnuc_va_list, args);

DEFINE_HOOK_FUNCTION(int, ftrylockfile, EIO, -1, FILE *, stream);
