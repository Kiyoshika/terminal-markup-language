#include "tsl.h"
#include "function.h"
#include "variable.h"
#include "instructions.h"

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
tsl_global_add_instruction(
  struct tsl_global_scope_t* const global_scope,
  const enum instruction_type_e instruction_type,
  const struct instruction_t* const instruction)
{
  return inst_list_add_instruction(global_scope->instruction_list, instruction_type, instruction);
}
