#ifndef PARSER_INSTRUCTIONS_H
#define PARSER_INSTRUCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

bool
tsl_parser_instructions_create_var(
  struct parse_context_t* const context);

bool
tsl_parser_instructions_create_function(
  struct parse_context_t* const context);

bool
tsl_parser_instructions_add_function_arg(
  struct parse_context_t* const context);

bool
tsl_parser_instructions_create_return_value(
  struct parse_context_t* const context);

#endif
