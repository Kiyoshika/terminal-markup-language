#include "parser.h"
#include "parser_internal.h"
#include "tsl.h"

bool
_tsl_parser_check_variable_exists_global(
  const struct tsl_global_scope_t* const global_scope,
  const char* const variable_name)
{
  struct variable_list_t* variable_list
    = global_scope->variable_list;

  for (size_t i = 0; i < variable_list->n_variables; ++i)
    if (strcmp(variable_list->variables[i].name, variable_name) == 0)
      return true;

  return false;
}
