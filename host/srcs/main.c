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

/**
 * @brief Print the header with important information
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @param memory_name the name of the shared memory
 */
void print_header(int argc, char **argv)
{
	printf("-------------------- funcheck v2 indev --------------------\n");
	printf("Command line: ");
	for (int i = 0; i < argc; i++)
		printf("%s ", argv[i]);
	printf("\n\n");
}

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
		printf("No program to run\n");
		display_help();
		return 1;
	}

	print_header(args_guest.argc, args_guest.argv);

	char *program_path = get_program_in_path(args_guest.argv[0]);

	t_symbolizer symbolizer = symbolizer_init(program_path);

	t_fetch_result fetch_result = allocations_fetch(
		args_guest.argc,
		args_guest.argv,
		envp, &symbolizer);
	allocations_test(args_guest.argc, args_guest.argv, envp, &fetch_result, &symbolizer);
	clear_fetch_result(&fetch_result);

	symbolizer_stop(&symbolizer);
	free(program_path);

	return 0;
}