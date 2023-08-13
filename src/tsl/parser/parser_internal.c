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

enum variable_type_e
_tsl_parser_get_literal_type(
  const char* const literal_value)
{
  const size_t len = strlen(literal_value);
  if (len  == 0)
    return VAR_TYPE_STRING;

  if ((len >= 2 && literal_value[0] == '-' && isdigit(literal_value[1]))
      || isdigit(literal_value[0]))
  {
    for (size_t i = 0; i < len; ++i)
      if (literal_value[i] == '.')
        return VAR_TYPE_FLOAT;
    return VAR_TYPE_INT;
  }

  if (strcmp(literal_value, "true") == 0
      || strcmp(literal_value, "false") == 0)
    return VAR_TYPE_BOOL;

  return VAR_TYPE_STRING;
}
