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
			run/runner.c \
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
			output/utils/output_utils.c \
			output/loading_bar/loading_bar.c \
			output/pretty/pretty_header.c \
			output/pretty/pretty_fetch_result.c \
			output/pretty/pretty_test_result.c \
			output/pretty/pretty_backtrace.c \
			output/pretty/pretty_allocation_track.c \
			output/json/json_header.c \
			output/json/json_fetch_result.c \
			output/json/json_test_result.c \
			output/json/json_backtrace.c \
			output/json/json_allocation_track.c \
			logs/logs.c \
			time/time.c \
			functions_fetch/functions_fetch.c \
			functions_test/functions_test.c


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