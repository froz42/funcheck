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
		printf("Usage: %s <program> [args]\n", argv[0]);
		return 1;
	}

	int argc_guest = argc - 1;
	char **argv_guest = argv + 1;

	print_header(argc_guest, argv_guest);

	char *program_path = get_program_in_path(argv_guest[0]);

	t_symbolizer symbolizer = symbolizer_init(program_path);

	t_fetch_result fetch_result = allocation_fetch(
		argc_guest,
		argv_guest,
		envp, &symbolizer);
	allocation_test(argc_guest, argv_guest, envp, &fetch_result, &symbolizer);
	clear_fetch_result(&fetch_result);

	symbolizer_stop(&symbolizer);
	free(program_path);

	return 0;
}