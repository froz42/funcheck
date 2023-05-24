# Funcheck - A tool for checking functions calls return protections
# Copyright (C) 2023  Theo Matis
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

NAME	= libfuncheck.so
CC		= clang
CFLAGS	= -Wall -Wextra -Werror -g -fPIC -fvisibility=hidden
DFLAGS	= -MMD -MF $(@:.o=.d)
LIBFLAG = -ldl -lpthread

SRCS_PATH		= ./srcs

SRCS	=	backtrace/get_backtrace.c \
			events/event_sender.c \
			hook/hook.c \
			hook/crashs.c \
			hook/exit.c \
			hook/free.c \
			hook/main.c \
			hook/mlx.c \
			hook/functions/unistd.c \
			hook/functions/stdio.c \
			hook/functions/fcntl.c \
			hook/functions/stdlib.c \
			hook/functions/pthread.c \
			hook/functions/poll.c \
			hook/functions/epoll.c \
			hook/functions/select.c \
			hook/functions/socket.c \
			hook/functions/signal.c \
			hook/functions/string.c \
			hook/functions/wait.c \
			hook/functions/stat.c \
			hook/functions/mman.c \
			hook/functions/semaphore.c \
			hook/allocs/malloc.c \
			hook/allocs/realloc.c \
			hook/allocs/calloc.c \
			function_search/function_search.c \
			shared_memory/shared_memory.c \
			env/clean_env.c \
			setup/setup.c \
			utils/error.c

OBJS	= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.o})
DEPS	= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.d})

all: $(NAME)

-include $(DEPS) $(DEPS_MAIN)
$(NAME):	${OBJS}
			$(CC) $(CFLAGS) $(DFLAGS) -shared -o $@ ${OBJS} $(LIBFLAG)

objs/%.o: 	$(SRCS_PATH)/%$(FILE_EXTENSION)
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) $(DFLAGS) ${DEFINE} -c $< -o $@

clean:		
			rm -rf objs

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re