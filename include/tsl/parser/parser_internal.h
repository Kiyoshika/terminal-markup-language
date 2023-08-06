#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include <stddef.h>
#include <stdbool.h>

bool
_tsl_parser_check_variable_exists_global(
  const struct tsl_global_scope_t* const global_scope,
  const char* const variable_name);

#endif
