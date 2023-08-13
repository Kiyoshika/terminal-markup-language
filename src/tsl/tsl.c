#include "tsl.h"
#include "function.h"
#include "variable.h"
#include "instructions.h"
#include "parser_internal.h"
#include "execute_instructions.h"
#include <stdio.h>

struct tsl_global_scope_t*
tsl_global_scope_create()
{
  struct tsl_global_scope_t* global_scope = malloc(sizeof(*global_scope));
  if (!global_scope)
    return NULL;

  global_scope->variable_list = var_list_create();
  if (!global_scope->variable_list)
  {
    free(global_scope);
    return NULL;
  }

  global_scope->function_list = func_list_create();
  if (!global_scope->function_list)
  {
    free(global_scope->variable_list);
    free(global_scope);
    return NULL;
  }

  global_scope->instruction_list = inst_list_create();
  if (!global_scope->instruction_list)
  {
    free(global_scope->function_list);
    free(global_scope->variable_list);
    free(global_scope);
    return NULL;
  }

  return global_scope;
}

bool
tsl_global_scope_add_instruction(
  struct tsl_global_scope_t* const global_scope,
  const struct instruction_t* const instruction)
{
  return inst_list_add_instruction(global_scope->instruction_list, instruction);
}

bool
tsl_global_scope_execute_instructions(
  struct tsl_global_scope_t* const global_scope)
{
  for (size_t i = 0; i < global_scope->instruction_list->n_instructions; ++i)
  {
    switch (global_scope->instruction_list->instructions[i].instruction_type)
    {
      case INST_TYPE_CREATE_VAR:
        if (!tsl_execute_instructions_create_var(global_scope, i))
          return false;
        break;

      case INST_TYPE_STORE_LITERAL:
        if (!tsl_execute_instructions_store_literal(global_scope, i))
          return false;
        break;

      case INST_TYPE_STORE_VAR:
        if (!tsl_execute_instructions_store_var(global_scope, i))
          return false;
        break;

      case INST_TYPE_CREATE_FUNCTION:
        if (!tsl_execute_instructions_create_function(global_scope, i))
          return false;
        break;

      case INST_TYPE_ADD_FUNCTION_ARG:
        if (!tsl_execute_instructions_add_function_arg(global_scope, i))
          return false;
        break;

      case INST_TYPE_CREATE_RETURN_VALUE:
        if (!tsl_execute_instructions_create_return_value(global_scope, i))
          return false;
        break;
    }
  }

  return true;
}

void
tsl_global_scope_free(
  struct tsl_global_scope_t** global_scope)
{
  if (!*global_scope)
    return;

  // TODO:
}
