#include "parser_instructions_internal.h"
  
bool
_parser_instructions_create_var_int(
  struct parse_context_t* const context)
{
  struct instruction_t create_var;
  inst_create_var(&create_var, NULL, context->object_name, VAR_TYPE_INT);
  tsl_global_scope_add_instruction(context->global_scope, &create_var);

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

  // literal
  if (endptr && strlen(endptr) == 0)
  {
    struct instruction_t store_literal;
    inst_store_literal_int(&store_literal, NULL, context->object_name, value);
    tsl_global_scope_add_instruction(context->global_scope, &store_literal);
  }
  // variable name
  else if (context->assigning_value)
  {
    struct instruction_t store_var;
    inst_store_var(&store_var, NULL, context->object_name, context->object_value);
    tsl_global_scope_add_instruction(context->global_scope, &store_var);
  }

  return true;
}

bool
_parser_instructions_create_var_float(
  struct parse_context_t* const context)
{
  struct instruction_t create_var;
  inst_create_var(&create_var, NULL, context->object_name, VAR_TYPE_FLOAT);
  tsl_global_scope_add_instruction(context->global_scope, &create_var);

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

  if (strlen(context->object_value) > 0)
    value = strtof(context->object_value, &endptr);
  else if (strlen(context->object_value) == 0 && context->assigning_value)
  {
    printf("assigning empty value.\n");
    return false;
  }

  // literal
  if (endptr && strlen(endptr) == 0)
  {
    if (!contains_decimal && context->assigning_value)
    {
      printf("Float must contain decimal '.'.\n");
      return false;
    }
    struct instruction_t store_literal;
    inst_store_literal_float(&store_literal, NULL, context->object_name, value);
    tsl_global_scope_add_instruction(context->global_scope, &store_literal);
  }
  // variable name
  else if (context->assigning_value)
  {
    struct instruction_t store_var;
    inst_store_var(&store_var, NULL, context->object_name, context->object_value);
    tsl_global_scope_add_instruction(context->global_scope, &store_var);
  }

  return true;
}
