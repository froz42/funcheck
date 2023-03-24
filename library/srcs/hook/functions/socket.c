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

#include <sys/socket.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, socket, ENOMEM, -1, int, domain, int, type, int, protocol);
DEFINE_HOOK_FUNCTION(int, socketpair, ENOMEM, -1, int, domain, int, type, int, protocol, int *, sv);
DEFINE_HOOK_FUNCTION(int, bind, EADDRNOTAVAIL, -1, int, sockfd, const struct sockaddr *, addr, socklen_t, addrlen);
DEFINE_HOOK_FUNCTION(int, getsockname, EFAULT, -1, int, sockfd, struct sockaddr *, addr, socklen_t *, addrlen);
DEFINE_HOOK_FUNCTION(int, connect, ECONNREFUSED, -1, int, sockfd, const struct sockaddr *, addr, socklen_t, addrlen);
DEFINE_HOOK_FUNCTION(int, getpeername, EFAULT, -1, int, sockfd, struct sockaddr *, addr, socklen_t *, addrlen);
DEFINE_HOOK_FUNCTION(ssize_t, send, EFAULT, -1, int, sockfd, const void *, buf, size_t, len, int, flags);
DEFINE_HOOK_FUNCTION(ssize_t, recv, EFAULT, -1, int, sockfd, void *, buf, size_t, len, int, flags);
DEFINE_HOOK_FUNCTION(ssize_t, sendto, EFAULT, -1, int, sockfd, const void *, buf, size_t, len, int, flags, __CONST_SOCKADDR_ARG, dest_addr, socklen_t, addrlen);
DEFINE_HOOK_FUNCTION(ssize_t, recvfrom, EFAULT, -1, int, sockfd, void *, buf, size_t, len, int, flags, __SOCKADDR_ARG, src_addr, socklen_t *, addrlen);
DEFINE_HOOK_FUNCTION(ssize_t, sendmsg, EFAULT, -1, int, sockfd, const struct msghdr *, msg, int, flags);
DEFINE_HOOK_FUNCTION(ssize_t, recvmsg, EFAULT, -1, int, sockfd, struct msghdr *, msg, int, flags);
DEFINE_HOOK_FUNCTION(int, getsockopt, EFAULT, -1, int, sockfd, int, level, int, optname, void *, optval, socklen_t *, optlen);
DEFINE_HOOK_FUNCTION(int, setsockopt, EFAULT, -1, int, sockfd, int, level, int, optname, const void *, optval, socklen_t, optlen);
DEFINE_HOOK_FUNCTION(int, listen, EFAULT, -1, int, sockfd, int, backlog);
DEFINE_HOOK_FUNCTION(int, accept, EFAULT, -1, int, sockfd, __SOCKADDR_ARG, addr, socklen_t *, addrlen);
DEFINE_HOOK_FUNCTION(int, accept4, EFAULT, -1, int, sockfd, __SOCKADDR_ARG, addr, socklen_t *, addrlen, int, flags);
DEFINE_HOOK_FUNCTION(int, shutdown, EFAULT, -1, int, sockfd, int, how);
DEFINE_HOOK_FUNCTION(int, sockatmark, EFAULT, -1, int, sockfd);
DEFINE_HOOK_FUNCTION(int, isfdtype, EFAULT, -1, int, fd, int, fdtype);