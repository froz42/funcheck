# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/22 18:46:49 by tmatis            #+#    #+#              #
#    Updated: 2022/09/09 16:37:25 by tmatis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= funcheck
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g
DFLAGS	= -MMD -MF $(@:.o=.d)

LIBFLAG = -lpthread

SRCS_PATH		= ./srcs
SRCS	= 	main.c\
			shared_memory/shared_memory.c \
			run/runner.c logs/logger.c \
			events/event_utils.c \
			events/handle_event.c \
			function_footprint/function_footprint.c \
			function_footprint/compares.c \
			env/env.c \
			leak_check/leak_check.c \
			record_io/record_io.c \
			backtrace/backtrace.c \
			path/path.c \
			symbolizer/parse_output/addr2line.c \
			symbolizer/parse_output/llvm_symbolizer.c \
			symbolizer/symbolizer.c \
			stages/0_general_setup.c \
			stages/1_allocation_fetch.c \
			stages/2_allocation_test.c


OBJS	= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.o})
DEPS	= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.d})

all: $(NAME)

-include $(DEPS) $(DEPS_MAIN)
$(NAME):	${OBJS}
			$(CC) $(CFLAGS) $(DFLAGS) -o $@ ${OBJS} $(LIBFLAG)

objs/%.o: 	$(SRCS_PATH)/%$(FILE_EXTENSION)
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) $(DFLAGS) ${DEFINE} -c $< -o $@

clean:		
			rm -rf objs

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re