#include "execute_instructions.h"
#include "tsl.h"
#include "variable.h"
#include "function.h"
#include "instructions.h"
#include "parser_internal.h"
#include <stdio.h>

bool
tsl_execute_instructions_create_var(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx)
{
  struct instruction_create_var_t* create_var
    = &global_scope->instruction_list->instructions[instruction_idx].instruction.create_var;

  if (_tsl_parser_check_variable_exists_global(global_scope, create_var->variable_name))
  {
    printf("Variable '%s' already exists.\n", create_var->variable_name);
    return false;
  }

  switch (create_var->variable_type)
  {
    case VAR_TYPE_INT:
    {
      struct variable_t* var_int = var_create_int(create_var->variable_name, 0);
      var_list_add_variable(global_scope->variable_list, var_int);
      break;
    }

    case VAR_TYPE_FLOAT:
    {
      struct variable_t* var_float = var_create_float(create_var->variable_name, 0.00f);
      var_list_add_variable(global_scope->variable_list, var_float);
      break;
    }

    case VAR_TYPE_BOOL:
    {
      struct variable_t* var_bool = var_create_bool(create_var->variable_name, false);
      var_list_add_variable(global_scope->variable_list, var_bool);
      break;
    }

    case VAR_TYPE_STRING:
    {
      struct variable_t* var_string = var_create_string(create_var->variable_name, "");
      var_list_add_variable(global_scope->variable_list, var_string);
      break;
    }
  }

  return true;
}

bool
tsl_execute_instructions_store_literal(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx)
{
  struct instruction_store_literal_t* store_literal
    = &global_scope->instruction_list->instructions[instruction_idx].instruction.store_literal;

  struct variable_t* variable 
    = var_list_find(global_scope->variable_list, store_literal->variable_name);

  if (!variable)
  {
    printf("Variable '%s' doesn't exist.\n", store_literal->variable_name);
    return false;
  }

  switch (store_literal->variable_type)
  {
    case VAR_TYPE_INT:
      variable->value.as_int = store_literal->variable_value.as_int;
      break;

    case VAR_TYPE_FLOAT:
      variable->value.as_float = store_literal->variable_value.as_float;
      break;

    case VAR_TYPE_BOOL:
      variable->value.as_bool = store_literal->variable_value.as_bool;
      break;

    case VAR_TYPE_STRING:
      variable->value.as_string = store_literal->variable_value.as_string;
      break;
  }

  return true;

}

bool
tsl_execute_instructions_store_var(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx)
{
  struct instruction_store_var_t* store_var
    = &global_scope->instruction_list->instructions[instruction_idx].instruction.store_var;

  struct variable_t* dest_var
    = var_list_find(global_scope->variable_list, store_var->dest_variable_name);

  if (!dest_var)
  {
    printf("Variable '%s' doesn't exist.\n", store_var->dest_variable_name);
    return false;
  }

  struct variable_t* src_var
    = var_list_find(global_scope->variable_list, store_var->src_variable_name);

  if (!src_var)
  {
    printf("Variable '%s' doesn't exist.\n", store_var->src_variable_name);
    return false;
  }

  if (dest_var->type != src_var->type)
  {
    printf("Mismatched types.\n");
    return false;
  }

  switch (dest_var->type)
  {
    case VAR_TYPE_INT:
      dest_var->value.as_int = src_var->value.as_int;
      break;

    case VAR_TYPE_FLOAT:
      dest_var->value.as_float = src_var->value.as_float;
      break;

    case VAR_TYPE_BOOL:
      dest_var->value.as_bool = src_var->value.as_bool;
      break;

    case VAR_TYPE_STRING:
      dest_var->value.as_string = strdup(src_var->value.as_string); // dangerously assumining no error
      break;
  }

  return true;
}

bool
tsl_execute_instructions_create_function(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx)
{
  struct instruction_create_function_t* create_function
    = &global_scope->instruction_list->instructions[instruction_idx].instruction.create_function;

  struct function_t* function
    = func_list_find(global_scope->function_list, create_function->function_name);

  if (function)
  {
    printf("Function '%s' already exists.\n", function->name);
    return false;
  }

  struct function_t* new_function 
    = func_create(create_function->return_type, create_function->function_name);
  
  func_list_add_function(
    global_scope->function_list,
    new_function);

  return true;
}

bool
tsl_execute_instructions_add_function_arg(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx)
{
  struct instruction_add_function_arg_t* add_function_arg
    = &global_scope->instruction_list->instructions[instruction_idx].instruction.add_function_arg;

  struct function_t* reference_function
    = func_list_find(global_scope->function_list, add_function_arg->reference_function_name);

  if (!reference_function)
  {
    printf("function '%s' does not exist.\n", add_function_arg->reference_function_name);
    return false;
  }

  struct variable_t* find_variable = var_list_find(reference_function->variable_list, add_function_arg->argument_name);
  if (find_variable)
  {
    printf("variable '%s' already exists.\n", add_function_arg->argument_name);
    return false;
  }

  struct variable_t* arg_variable = NULL;

  switch (add_function_arg->argument_type)
  {
    case VAR_TYPE_INT:
      arg_variable = var_create_int(add_function_arg->argument_name, 0);
      break;

    case VAR_TYPE_FLOAT:
      arg_variable = var_create_float(add_function_arg->argument_name, 0.0f);
      break;

    case VAR_TYPE_BOOL:
      arg_variable = var_create_bool(add_function_arg->argument_name, false);
      break;

    case VAR_TYPE_STRING:
      arg_variable = var_create_string(add_function_arg->argument_name, "");
      break;
  }

  // function arguments are essentially treated like stack variables; they're created first
  // before user-defined stack variables
  func_add_variable(reference_function, arg_variable);

  return true;
}

bool
tsl_execute_instructions_create_return_value(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx)
{
  struct instruction_create_return_value_t* create_return_value
    = &global_scope->instruction_list->instructions[instruction_idx].instruction.create_return_value;

  struct function_t* reference_function
    = func_list_find(global_scope->function_list, create_return_value->reference_function_name);

  struct variable_t* return_variable
    = var_list_find(reference_function->variable_list, create_return_value->value);

  if (return_variable)
  {
    if (return_variable->type != reference_function->return_type)
    {
      printf("returning variable with incorrect type.\n");
      return false;
    }

    switch (reference_function->return_type)
    {
      case VAR_TYPE_INT:
        reference_function->return_value.as_int = return_variable->value.as_int;
        break;

      case VAR_TYPE_FLOAT:
        reference_function->return_value.as_float = return_variable->value.as_float;
        break;

      case VAR_TYPE_BOOL:
        reference_function->return_value.as_bool = return_variable->value.as_bool;
        break;

      case VAR_TYPE_STRING:
        reference_function->return_value.as_string = strdup(return_variable->value.as_string);
        break;
    }
  }
  else
  {
    enum variable_type_e literal_type = _tsl_parser_get_literal_type(create_return_value->value);
    if (literal_type != reference_function->return_type)
    {
      printf("returning literal with incorrect type.\n");
      return false;
    }
    char* endptr = NULL;
    switch (literal_type)
    {
      case VAR_TYPE_INT:
        reference_function->return_value.as_int = strtol(create_return_value->value, &endptr, 10);
        break;

      case VAR_TYPE_FLOAT:
        reference_function->return_value.as_float = strtod(create_return_value->value, &endptr);
        break;

      case VAR_TYPE_BOOL:
        reference_function->return_value.as_bool = create_return_value->value[0] == 't' ? true : false;
        break;

      case VAR_TYPE_STRING:
        reference_function->return_value.as_string = strdup(create_return_value->value);
        break;
    }
  }

  reference_function->contains_return_value = true;

  return true;
}
