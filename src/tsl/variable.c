#include "variable.h"

struct variable_t*
var_create_int(
  const char* const name,
  const int32_t value)
{
  struct variable_t* new_var = malloc(sizeof(*new_var));
  if (!new_var)
    return NULL;

  new_var->name = strdup(name);
  if (!new_var->name)
  {
    free(new_var);
    return NULL;
  }

  new_var->type = VAR_TYPE_INT;
  new_var->value.as_int = value;

  return new_var;

}

struct variable_t*
var_create_float(
  const char* const name,
  const float value)
{
  struct variable_t* new_var = malloc(sizeof(*new_var));
  if (!new_var)
    return NULL;

  new_var->name = strdup(name);
  if (!new_var->name)
  {
    free(new_var);
    return NULL;
  }

  new_var->type = VAR_TYPE_FLOAT;
  new_var->value.as_float = value;

  return new_var;

}

struct variable_t*
var_create_bool(
  const char* const name,
  const bool value)
{
  struct variable_t* new_var = malloc(sizeof(*new_var));
  if (!new_var)
    return NULL;

  new_var->name = strdup(name);
  if (!new_var->name)
  {
    free(new_var);
    return NULL;
  }

  new_var->type = VAR_TYPE_BOOL;
  new_var->value.as_bool = value;

  return new_var;

}

struct variable_t*
var_create_string(
  const char* const name,
  const char* const value)
{
  struct variable_t* new_var = malloc(sizeof(*new_var));
  if (!new_var)
    return NULL;

  new_var->name = strdup(name);
  if (!new_var->name)
  {
    free(new_var);
    return NULL;
  }

  new_var->type = VAR_TYPE_STRING;
  new_var->value.as_string = strdup(value);

  if (!new_var->value.as_string)
  {
    free(new_var->name);
    free(new_var);
    return NULL;
  }

  return new_var;

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

struct variable_t*
var_list_find(
  const struct variable_list_t* const variable_list,
  const char* const variable_name)
{
  for (size_t i = 0; i < variable_list->n_variables; ++i)
    if (strcmp(variable_list->variables[i].name, variable_name) == 0)
      return &variable_list->variables[i];

  return NULL;
}
