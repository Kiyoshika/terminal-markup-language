#include "function.h"

struct function_t*
func_create(
  const enum variable_type_e return_type,
  const char* const name)
{
  struct function_t* function = malloc(sizeof(*function));
  if (!function)
    return NULL;

  function->contains_return_value = false;
  function->return_type = return_type;

  memset(function->name, 0, FUNC_NAME_MAX_LEN);
  strncat(function->name, name, FUNC_NAME_MAX_LEN);

  function->variable_list = var_list_create();
  if (!function->variable_list)
  {
    free(function);
    return NULL;
  }

  function->instruction_list = inst_list_create();
  if (!function->instruction_list)
  {
    free(function->variable_list);
    free(function);
    return NULL;
  }

  return function;
}

bool
func_add_variable(
  struct function_t* const function,
  const struct variable_t* const variable)
{
  return var_list_add_variable(function->variable_list, variable);
}

bool
func_add_instruction(
  struct function_t* const function,
  const struct instruction_t* const instruction)
{
  return inst_list_add_instruction(function->instruction_list, instruction);
}

struct function_list_t*
func_list_create()
{
  struct function_list_t* function_list = malloc(sizeof(*function_list));
  if (!function_list)
    return NULL;

  function_list->n_functions = 0;
  function_list->capacity = 10;
  function_list->functions = calloc(function_list->capacity, sizeof(*function_list->functions));
  if (!function_list->functions)
  {
    free(function_list);
    return NULL;
  }

  return function_list;
}

bool
func_list_add_function(
  struct function_list_t* const function_list,
  const struct function_t* const function)
{
  memcpy(
    &function_list->functions[function_list->n_functions++],
    function,
    sizeof(*function));

  if (function_list->n_functions == function_list->capacity)
  {
    size_t new_capacity = function_list->capacity * 2;
    void* alloc = realloc(function_list->functions, new_capacity * sizeof(*function));
    if (!alloc)
      return false;
    function_list->functions = alloc;
    function_list->capacity = new_capacity;
  }

  return true;
}

struct function_t*
func_list_find(
  const struct function_list_t* const function_list,
  const char* const function_name)
{
  for (size_t i = 0; i < function_list->n_functions; ++i)
    if (strcmp(function_list->functions[i].name, function_name) == 0)
      return &function_list->functions[i];

  return NULL;
}
