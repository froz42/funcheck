#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "run/runner.h"
#include "host.h"
#include "shared_memory/shared_memory.h"
#include "symbolizer/symbolizer.h"
#include "path/path.h"
#include "config/config.h"
#include "output/output.h"
#include "logs/logs.h"
#include "functions_fetch/functions_fetch.h"
#include "functions_test/functions_test.h"

typedef struct
{
	t_symbolizer symbolizer;
	char *program_path;
} init_program_t;

static init_program_t init_program(args_t *args_guest)
{
	const config_t *config = get_config();

	if (is_option_set(HELP_MASK, config))
	{
		display_help();
		exit(EXIT_SUCCESS);
	}

	if (is_option_set(VERSION_MASK, config))
	{
		printf("%s\n", VERSION);
		exit(EXIT_SUCCESS);
	}

	if (args_guest->argc == 0)
	{
		log_error("No program specified, use --help for more information");
		exit(EXIT_FAILURE);
	}

	char *program_path = get_program_in_path(args_guest->argv[0]);
	if (program_path == NULL)
	{
		log_error("Program not found or not executable");
		exit(EXIT_FAILURE);
	}

	t_symbolizer symbolizer = symbolizer_init(program_path);

	init_program_t result = {symbolizer, program_path};
	return result;
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
	args_t args_guest = parse_args(argc, argv);

	init_program_t init_program_infos = init_program(&args_guest);

	write_header(args_guest);

	write_head_function_fetch();
	t_fetch_result fetch_result = functions_fetch(
		&args_guest,
		envp,
		&init_program_infos.symbolizer
	);
	write_tail_function_fetch();

	write_head_function_tests();
	size_t failed_tests_count = functions_test(
		args_guest,
		envp,
		&fetch_result,
		&init_program_infos.symbolizer
	);
	write_tail_function_tests(!failed_tests_count);
	clear_fetch_result(&fetch_result);
	symbolizer_stop(&init_program_infos.symbolizer);
	free(init_program_infos.program_path);
	write_tail();

	return failed_tests_count ? EXIT_FAILURE : EXIT_SUCCESS;
}