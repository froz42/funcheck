#ifndef HOOK_H
#define HOOK_H

#include <stdlib.h>

#include "../backtrace/backtrace.h"
#include "../utils/bool.h"
#include "../events/event_sender.h"
#include "../function_search/function_search.h"
#include "../utils/export.h"
#include <errno.h>
#include <unistd.h>

void set_backtrace_to_block(ptr_address *backtrace, size_t iteration_to_block);

void disable_hooks(void);
void enable_hooks(void);
bool_t is_hooks_enabled(void);

bool_t should_block(ptr_address *backtrace);

void setup_exit_hook(void);
void setup_signals(void);

void setup_hooks(void);

#include <stdarg.h>

/**
 * @brief This macro is used to escape any ',' in a macro expansion
 *
 * @param ... the macro expansion
 */
#define ESC(...) __VA_ARGS__

/**
 * @brief will apply if we have 0 arguments
 * e will be the empty value
 */
#define APPLYTWOJOIN_0(f, j, e) ESC e

/*
 * The below macros are used to apply a function to each argument of a function
 * each macros will call the precedent:
 * APPLYTWOJOIN_8 will call APPLYTWOJOIN_6 that will call APPLYTWOJOIN_4 etc...
 * That way we can have a macro that can handle any number of arguments up to 14
 */
#define APPLYTWOJOIN_2(f, j, e, t, v) f(t, v)
#define APPLYTWOJOIN_4(f, j, e, t, v, ...) f(t, v) ESC j \
APPLYTWOJOIN_2(f, j, e, __VA_ARGS__)
#define APPLYTWOJOIN_6(f, j, e, t, v, ...) f(t, v) ESC j \
APPLYTWOJOIN_4(f, j, e, __VA_ARGS__)
#define APPLYTWOJOIN_8(f, j, e, t, v, ...) f(t, v) ESC j \
APPLYTWOJOIN_6(f, j, e, __VA_ARGS__)
#define APPLYTWOJOIN_10(f, j, e, t, v, ...) f(t, v) ESC j \
APPLYTWOJOIN_8(f, j, e, __VA_ARGS__)
#define APPLYTWOJOIN_12(f, j, e, t, v, ...) f(t, v) ESC j \
APPLYTWOJOIN_10(f, j, e, __VA_ARGS__)
#define APPLYTWOJOIN_14(f, j, e, t, v, ...) f(t, v) ESC j \
APPLYTWOJOIN_12(f, j, e, __VA_ARGS__)

#define APPLYTWOJOIN_N(_14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0, N, ...) \
    APPLYTWOJOIN##N

/**
 * @brief This macro is used to apply a function to each argument of a function
 *
 * @param f the function to apply (macro)
 * @param join the joiner to use between each application of the function
 * @param empty the empty value to use if there is no argument
 */
#define APPLYTWOJOIN(f, j, e, ...)                                                                     \
    APPLYTWOJOIN_N(_0, ##__VA_ARGS__, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) \
    (f, j, e, ##__VA_ARGS__)

#define PASS(t, v) v
#define ARGS(t, v) t v

/**
 * @brief The hook function definition
 *
 * @param type the return type of the function
 * @param name the name of the function
 * @param error_no the error number to set
 * @param error_return the value to return in case of error
 * @param ... the arguments of the function
 *
 * @example DEFINE_HOOK_FUNCTION(int, open, EACCES, -1, char*, pathname, int, flags, mode_t, mode)
 */
#define DEFINE_HOOK_FUNCTION(type, name, error_no, error_return, ...)            \
    type EXPORT name(APPLYTWOJOIN(ARGS, (, ), (void), ##__VA_ARGS__))            \
    {                                                                            \
        typeof(&name) name##_original = get_original_function();                 \
        if (!is_hooks_enabled())                                                 \
            return name##_original(APPLYTWOJOIN(PASS, (, ), (), ##__VA_ARGS__)); \
                                                                                 \
        disable_hooks();                                                         \
        t_shared_info *shared_memory = get_shared_memory();                      \
        get_backtrace(shared_memory->backtrace);                                 \
        if (should_block(shared_memory->backtrace))                              \
        {                                                                        \
            errno = error_no;                                                    \
            enable_hooks();                                                      \
            return error_return;                                                 \
        }                                                                        \
        send_function_call_event(shared_memory);                                 \
        enable_hooks();                                                          \
        return name##_original(APPLYTWOJOIN(PASS, (, ), (), ##__VA_ARGS__));     \
    }

#endif