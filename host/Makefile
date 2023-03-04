# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phabibis <phabibis@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/22 18:46:49 by tmatis            #+#    #+#              #
#    Updated: 2023/02/09 18:39:21 by phabibis         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= funcheck
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g
DFLAGS	= -MMD -MF $(@:.o=.d)

# get version from commit tag or short commit hash
VERSION = $(shell git describe --tags --always --dirty --broken 2>/dev/null || echo "unknown")
VERSION_DEFINE = -DVERSION=\"$(VERSION)\"

LIBFLAG = -lpthread

SRCS_PATH		= ./srcs
SRCS	= 	main.c \
			config/btree_char_ptr.c \
			config/config.c \
			shared_memory/shared_memory.c \
			run/runner.c logs/logger.c \
			events/event_utils.c \
			events/handle_event.c \
			function_footprint/function_footprint.c \
			function_footprint/compares.c \
			env/env.c \
			record_io/record_io.c \
			backtrace/backtrace.c \
			path/path.c \
			symbolizer/parse_output/addr2line.c \
			symbolizer/parse_output/llvm_symbolizer.c \
			symbolizer/symbolizer.c \
			json/json.c \
			output/output.c \
			output/output_allocation_track.c \
			output/output_fetch_result.c \
			output/output_test_result.c \
			output/output_backtrace.c \
			output/output_utils.c \
			stages/0_general_setup.c \
			stages/1_allocations_fetch.c \
			stages/2_allocations_test.c


OBJS	= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.o})
DEPS	= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.d})

all: $(NAME)

-include $(DEPS) $(DEPS_MAIN)
$(NAME):	${OBJS}
			$(CC) $(CFLAGS) $(DFLAGS) -o $@ ${OBJS} $(LIBFLAG)

objs/%.o: 	$(SRCS_PATH)/%$(FILE_EXTENSION)
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) $(DFLAGS) ${VERSION_DEFINE} -c $< -o $@

clean:		
			rm -rf objs

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re