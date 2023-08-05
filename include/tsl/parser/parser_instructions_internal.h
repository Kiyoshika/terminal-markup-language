#ifndef PARSER_INSTRUCTIONS_INTERNAL_H
#define PARSER_INSTRUCTIONS_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "instructions.h"
#include "tsl.h"

bool
_parser_instructions_create_var_int(
  struct parse_context_t* const context);

bool
_parser_instructions_create_var_float(
  struct parse_context_t* const context);

#endif
