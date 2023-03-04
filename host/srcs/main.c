#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "run/runner.h"
#include "host.h"
#include "shared_memory/shared_memory.h"
#include "stages/stages.h"
#include "symbolizer/symbolizer.h"
#include "path/path.h"
#include "config/config.h"
#include "output/output.h"

/**
 * @brief The main function of the host
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @param envp the environment variables
 * @return 0 on success, 1 on failure
 */
int main(int argc, char **argv, char **envp)
{
	if (argc < 2)
	{
		display_help();
		return 1;
	}

	args_t args_guest = parse_args(argc, argv);

	const config_t *config = get_config();
	
	if (is_option_set(HELP_MASK, config))
	{
		display_help();
		return 0;
	}

	if (is_option_set(VERSION_MASK, config))
	{
		printf("%s\n", VERSION);
		return 0;
	}
	
	if (args_guest.argc == 0)
	{
		dprintf(2, "No program to run\n");
		display_help();
		return 1;
	}

	char *program_path = get_program_in_path(args_guest.argv[0]);
	if (program_path == NULL)
	{
		dprintf(2, "Program not found\n");
		return 1;
	}

	t_symbolizer symbolizer = symbolizer_init(program_path);

	write_header(args_guest);

	write_head_function_fetch();
	t_fetch_result fetch_result = allocations_fetch(
		args_guest.argc,
		args_guest.argv,
		envp, &symbolizer);
	write_tail_function_fetch();
	

	write_head_function_tests();
	int res = allocations_test(args_guest.argc, args_guest.argv, envp, &fetch_result, &symbolizer);
	write_tail_function_tests();
	clear_fetch_result(&fetch_result);

	symbolizer_stop(&symbolizer);
	free(program_path);

	write_tail();

	return res;
}