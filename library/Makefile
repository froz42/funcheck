# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/22 18:46:49 by tmatis            #+#    #+#              #
#    Updated: 2022/08/24 02:17:54 by tmatis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
			hook/functions/unistd.c \
			hook/functions/stdio.c \
			hook/functions/fcntl.c \
			hook/functions/stdlib.c \
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