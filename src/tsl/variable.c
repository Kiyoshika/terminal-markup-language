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

  new_variable->value = calloc(1, value_size);
  if (!new_variable->value)
  {
    free(new_variable->name);
    free(new_variable);
    return NULL;
  }

  memcpy(new_variable->value, value, value_size);

  return new_variable;
}

struct variable_list_t*
var_list_create()
{
  struct variable_list_t* variable_list = malloc(sizeof(*variable_list));
  if (!variable_list)
    return NULL;

  variable_list->capacity = 10;
  variable_list->n_variables = 0;
  variable_list->variables = calloc(variable_list->capacity, sizeof(struct variable_t));

  if (!variable_list->variables)
  {
    free(variable_list);
    return NULL;
  }

  return variable_list;
}

bool
var_list_add_variable(
  struct variable_list_t* const variable_list,
  const struct variable_t* const variable)
{
  memcpy(
    &variable_list->variables[variable_list->n_variables++],
    variable,
    sizeof(*variable));

  if (variable_list->n_variables == variable_list->capacity)
  {
    size_t new_capacity = variable_list->capacity * 2;
    void* alloc = realloc(variable_list->variables, new_capacity * sizeof(*variable));
    if (!alloc)
      return false;
    variable_list->variables = alloc;
    variable_list->capacity = new_capacity;
  }

  return true;
}
