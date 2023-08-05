#include "parser_instructions_internal.h"
  
bool
_parser_instructions_create_var_int(
  struct parse_context_t* const context)
{
  char* endptr = NULL;
  int32_t value = 0;
  size_t object_value_len = strlen(context->object_value);

  for (size_t i = 0; i < object_value_len; ++i)
  {
    if (context->object_value[i] == '.')
    {
      printf("Cannot assign float literal to an int.\n");
      return false;
    }
  }

  if (object_value_len  > 0)
    value = strtol(context->object_value, &endptr, 10);
  else if (object_value_len == 0 && context->assigning_value)
  {
    printf("assigning empty value.\n");
    return false;
  }

  if (endptr && strlen(endptr) > 0)
  {
    printf("invalid integer value.\n");
    return false;
  }

  struct variable_t* new_variable 
    = var_create(
        context->object_name,
        VAR_TYPE_INT,
        &value,
        sizeof(int32_t));

  struct instruction_t create_var;
  inst_create_var(&create_var, NULL, &new_variable);
  tsl_global_scope_add_instruction(context->global_scope, &create_var);

  return true;
}

bool
_parser_instructions_create_var_float(
  struct parse_context_t* const context)
{
  char* endptr = NULL;
  float value = 0.0f;
  size_t object_value_len = strlen(context->object_value);

  bool contains_decimal = false;
  for (size_t i = 0; i < object_value_len; ++i)
  {
    if (context->object_value[i] == '.')
    {
      contains_decimal = true;
      break;
    }
  }

  if (!contains_decimal && context->assigning_value)
  {
    printf("Float must contain decimal '.'.\n");
    return false;
  }

  if (strlen(context->object_value) > 0)
    value = strtof(context->object_value, &endptr);
  else if (strlen(context->object_value) == 0 && context->assigning_value)
  {
    printf("assigning empty value.\n");
    return false;
  }

  if (endptr && strlen(endptr) > 0)
  {
    printf("invalid float value.\n");
    return false;
  }

  struct variable_t* new_variable
    = var_create(
        context->object_name,
        VAR_TYPE_FLOAT,
        &value,
        sizeof(float));

  struct instruction_t create_var;
  inst_create_var(&create_var, NULL, &new_variable);
  tsl_global_scope_add_instruction(context->global_scope, &create_var);

  return true;
}
