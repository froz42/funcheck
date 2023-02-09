#include "config.h"
#include <stdio.h>
#include <string.h>
#include "../utils/color.h"

#define WIDTH_DESC 50

/**
 * @brief Callback to parse an argument
 * 
 */
typedef void (*parse_arg_callback_t)(char *arg, config_t *config);

/**
 * @brief Option structure to describe an option
 * 
 * @param letter The letter of the option (ex: -h)
 * @param name The name of the option (ex: --help)
 * @param description The description of the option (ex: Display this help)
 * @param mask The mask to set in the options field of the config
 */
typedef struct
{
    char letter;
    char *name;
    char *description;
    char mask;
    parse_arg_callback_t callback;
} option_t;

/**
 * @brief Parse the functions to test and add them to the config
 * 
 * @param arg The argument to parse
 * @param config The config to update
 */
static void parse_test_functions(char *arg, config_t *config)
{
    if (!arg)
        return;
    char *saveptr = NULL;
    char *token = strtok_r(arg, " ,", &saveptr);
    while (token != NULL)
    {
        btree_char_ptr_insert(&config->test_functions, &token);
        token = strtok_r(NULL, " ,", &saveptr);
    }
}

/**
 * @brief Parse the functions to ignore and add them to the config
 * 
 * @param arg The argument to parse
 * @param config The config to update
 */
static void parse_ignore_functions(char *arg, config_t *config)
{
    if (!arg)
        return;
    char *saveptr = NULL;
    char *token = strtok_r(arg, " ,", &saveptr);
    while (token != NULL)
    {
        btree_char_ptr_insert(&config->ignore_functions, &token);
        token = strtok_r(NULL, " ,", &saveptr);
    }
}

/**
 * @brief Set the option mask in the config
 * 
 * @param mask The mask to set
 * @param config The config to update
 */
static void set_option(char mask, config_t *config)
{
    config->options |= mask;
}

/**
 * @brief The list of options
 */
static const option_t options[] = {
    {'h', "help", "Display this help", HELP_MASK, NULL},
    {'v', "version", "Display the version", VERSION_MASK, NULL},
    {'t', "test-functions", "Specify the functions to test --test-functions='func1 func2 func3' or --test-functions 'func1 func2 func3'", 0, parse_test_functions},
    {'i', "ignore-functions", "Specify the functions to ignore --ignore-functions='func1 func2 func3' or --ignore-functions 'func1 func2 func3'", 0, parse_ignore_functions},
    {'o', "output", "Specify the output format --output=simple|json|colored", 0, NULL},
    {'l', "track-allocations", "Track memory allocations", TRACK_ALLOCATIONS_MASK, NULL},
    {'a', "all-output", "Output stdout and stderr of every test", ALL_OUTPUT_MASK, NULL},
    {'c', "abort-as-crash", "Treat abort() as a crash", ABORT_AS_CRASH_MASK, NULL},
    {0, NULL, NULL, 0, NULL}};

/**
 * @brief Write desc word by word and if the line is too long, add a new line
 * 
 * @param desc The description to write
 */
void write_desc(const char *desc)
{
    char *saveptr = NULL;
    char *str = strdup(desc);
    char *token = strtok_r(str, " ", &saveptr);
    int len = 0;
    while (token != NULL)
    {
        len += strlen(token) + 1;
        if (len > WIDTH_DESC)
        {
            printf("\n%31s", "");
            len = strlen(token) + 1;
        }
        printf("%s ", token);
        token = strtok_r(NULL, " ", &saveptr);
    }
    printf("\n");
    free(str);
}

void display_help(void)
{
    printf("\n  %s%sfuncheck%s - a functions calls protection checker\n\n", BOLD, CYAN, RESET);
    printf("  %s%sUSAGE%s\n\n", WHITE, BOLD, RESET);
    printf("    %s%s$%s %sfuncheck%s --help\n", CYAN, BOLD, RESET, BOLD, RESET);
    printf("    %s%s$%s %sfuncheck%s -v\n", CYAN, BOLD, RESET, BOLD, RESET);
    printf("    %s%s$%s %sfuncheck%s %s./your_program%s [params]\n\n", CYAN, BOLD, RESET, BOLD, RESET, UNDERLINE, RESET);

    printf("    By default, %sfuncheck%s will test all the functions it can find\n", CYAN, RESET);
    printf("    in the program. You can specify the functions to test with\n");
    printf("    the %s%s--test-functions%s option or the functions to ignore with\n", BOLD, UNDERLINE, RESET);
    printf("    the %s%s--ignore-functions%s option.\n\n", BOLD, UNDERLINE, RESET);

    printf("  %s%sOPTIONS%s\n\n", WHITE, BOLD, RESET);
    for (int i = 0; options[i].name != NULL; i++)
    {
        printf("     -%c, --%-20s", options[i].letter, options[i].name);
        write_desc(options[i].description);
    }
}

/**
 * @brief Search an option by its letter
 * 
 * @param arg the pointer to the argument to parse
 * @param options the list of options
 * @return const option_t* the option found or NULL
 */
static const option_t *search_option_by_letter(char **arg, const option_t *options)
{
    for (int i = 0; options[i].letter != 0; i++)
    {
        if (options[i].letter == **arg)
        {
            (*arg)++;
            return &options[i];
        }
    }
    return NULL;
}

/**
 * @brief Search an option by its name
 * 
 * @param arg the pointer to the argument to parse
 * @param options the list of options
 * @return const option_t* the option found or NULL
 */
static const option_t *search_option_by_name(char **arg, const option_t *options)
{
    for (int i = 0; options[i].name != NULL; i++)
    {
        // compare till we reach \0 or =
        size_t j = 0;
        while (
            options[i].name[j] != '\0' && options[i].name[j] == (*arg)[j] && (*arg)[j] != '=')
        {
            j++;
        }
        if (options[i].name[j] == '\0' && ((*arg)[j] == '=' || (*arg)[j] == '\0'))
        {
            *arg += j;
            return &options[i];
        }
    }
    return NULL;
}

/**
 * @brief Parse an argument by its letter
 * 
 * @param argv_pointer the pointer to argv so we can increment it
 * @param argc_pointer the pointer to argc so we can decrement it
 * @param config the config to update
 */
static void parse_arg_by_letter(char ***argv_pointer, int *argc_pointer, config_t *config)
{
    char *arg = (**argv_pointer) + 1; // we skip the '-'
    
    while (*arg != '\0')
    {
        const option_t *option = search_option_by_letter(&arg, options);
        if (option == NULL)
        {
            printf("Unknown option: %c\n", *arg);
            exit(1);
        }
        set_option(option->mask, config);
        if (option->callback != NULL)
        {
            if (*arg == '\0')
            {
                (*argv_pointer)++;
                (*argc_pointer)--;
                arg = **argv_pointer;
            }
            option->callback(arg, config);
            break;
        }
    }
    (*argv_pointer)++;
    (*argc_pointer)--;
}

/**
 * @brief Parse an argument by its name
 * 
 * @param argv_pointer the pointer to argv so we can increment it
 * @param argc_pointer the pointer to argc so we can decrement it
 * @param config the config to update
 */
static void parse_arg_by_name(char ***argv_pointer, int *argc_pointer, config_t *config)
{
    char *arg = (**argv_pointer) + 2; // we skip the '--'
    const option_t *option = search_option_by_name(&arg, options);
    if (option == NULL)
    {
        printf("Unknown option: %s\n", arg);
        exit(1);
    }
    set_option(option->mask, config);
    if (option->callback != NULL)
    {
        if (*arg == '=')
        {
            arg++;
            option->callback(arg, config);
        }
        else
        {
            (*argv_pointer)++;
            (*argc_pointer)--;
            option->callback(**argv_pointer, config);
        }
    }
    (*argv_pointer)++;
    (*argc_pointer)--;
}

/**
 * @brief Parse the arguments and return the arguments remaining
 * 
 * @param argc count of arguments
 * @param argv the arguments
 * @return args_t the arguments remaining 
 */
args_t parse_args(int argc, char **argv)
{

    // we remove argv[0] since it is the name of the program
    argc--;
    argv++;
    config_t *config = get_config();

    while (argc && **argv == '-')
    {
        char *arg = *argv;
        arg++; // we skip the '-'
        if (*arg == '-')
            parse_arg_by_name(&argv, &argc, config);
        else
        {
            // if the arg is just '-' we do not count it as an option
            if (*arg == '\0')
                break;
            parse_arg_by_letter(&argv, &argc, config);
        }
    }

    args_t args = {argc, argv};
    return args;
}

/**
 * @brief return the config in static memory
 * 
 * @return config_t* the config 
 */
config_t *get_config()
{
    static config_t config = {0, NULL, NULL};
    return &config;
}

/**
 * @brief Get if an option is set
 * 
 * @param mask the mask of the option
 * @param config the config to check
 * @return char 1 if the option is set, 0 otherwise
 */
char is_option_set(int mask, const config_t *config)
{
    return (config->options & mask) != 0;
}