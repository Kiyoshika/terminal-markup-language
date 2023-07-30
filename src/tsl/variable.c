#include "variable.h"

struct variable_t*
var_create(
  const char* const name,
  const enum variable_type_e type,
  const void* const value,
  const size_t value_size)
{
  struct variable_t* new_variable
    = malloc(sizeof(*new_variable));

  if (!new_variable)
    return NULL;

  new_variable->name = strdup(name);
  if (!new_variable->name)
  {
    free(new_variable);
    return NULL;
  }

  new_variable->type = type;

  new_variable->value = calloc(value_size, 1);
  if (!new_variable->value)
  {
    free(new_variable->name);
    free(new_variable);
    return NULL;
  }

  memcpy(&new_variable->value, value, value_size);

  return new_variable;
}
