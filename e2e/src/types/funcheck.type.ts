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

export type Allocation = {
    address: string;
    size: number;
};

export type Backtrace = {
    address: string;
    function: string;
    file: string;
    line: number;
    column: number;
};

export type AllocationNotFreed = {
    'function-name': string;
    'call-count': number;
    backtrace: Backtrace[];
    allocations: Allocation[];
};

export type Crash = {
    name: string;
    backtrace: Backtrace[];
};

export type FunctionFetch = {
    output: string;
    'return-code': number;
    crash?: Crash;
    'allocations-not-freed'?: AllocationNotFreed[];
    'function-detected-count': number;
};

export type FunctionTest = {
    'function-name': string;
    'return-code': number;
    'allocations-not-freed'?: AllocationNotFreed[];
    'function-backtrace': Backtrace[];
    output?: string;
    crash?: Crash;
};

export type FuncheckType = {
    "funcheck-version": string;
    program: string;
    arguments: string[];
    "function-fetch": FunctionFetch;
    "function-tests": FunctionTest[];
};