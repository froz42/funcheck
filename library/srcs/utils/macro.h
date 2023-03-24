/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MACRO_H
#define MACRO_H

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

// those macro allow to get the last argument name of a function
#define M_NARGS(...) M_NARGS_(__VA_ARGS__, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
// #define M_NARGS_(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N
#define M_NARGS_(_14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N

// utility (concatenation)
#define M_CONC(A, B) M_CONC_(A, B)
#define M_CONC_(A, B) A##B

#define M_GET_ELEM(N, ...) M_CONC(M_GET_ELEM_, N)(__VA_ARGS__)
#define M_GET_ELEM_0(_0, ...) _0
#define M_GET_ELEM_1(_0, _1, ...) _1
#define M_GET_ELEM_2(_0, _1, _2, ...) _2
#define M_GET_ELEM_3(_0, _1, _2, _3, ...) _3
#define M_GET_ELEM_4(_0, _1, _2, _3, _4, ...) _4
#define M_GET_ELEM_5(_0, _1, _2, _3, _4, _5, ...) _5
#define M_GET_ELEM_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define M_GET_ELEM_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define M_GET_ELEM_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define M_GET_ELEM_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define M_GET_ELEM_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define M_GET_ELEM_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define M_GET_ELEM_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define M_GET_ELEM_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define M_GET_ELEM_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14

// Get last argument - placeholder decrements by one
#define M_GET_LAST(...) M_GET_ELEM(M_NARGS(__VA_ARGS__), _, __VA_ARGS__, , , , , , , , , , , , , , , )

#endif