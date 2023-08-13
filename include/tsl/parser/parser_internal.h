#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include <stddef.h>
#include <stdbool.h>

#include "variable.h"

bool
_tsl_parser_check_variable_exists_global(
  const struct tsl_global_scope_t* const global_scope,
  const char* const variable_name);

enum variable_type_e
_tsl_parser_get_literal_type(
  const char* const literal_value);

#endif
