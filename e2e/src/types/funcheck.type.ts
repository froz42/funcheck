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