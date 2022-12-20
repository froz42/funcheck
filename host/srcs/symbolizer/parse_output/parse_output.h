#ifndef PARSER_OUTPUT_H
#define PARSER_OUTPUT_H

#include <stdio.h>
#include "../../function_footprint/function_footprint.h"

void parse_llvm_symbolizer_output(FILE *file, t_address_info *address_info_ptr);
void parse_addr2line_output(FILE *file, t_address_info *address_info_ptr);

#endif