#include "parser_instructions.h"
#include "parser_instructions_internal.h"
#include "parser.h"
#include "parser_internal.h"

bool
tsl_parser_instructions_create_var(
  struct parse_context_t* const context)
{
  if (strlen(context->object_name) == 0)
  {
    printf("empty variable name.\n");
    return false;
  }

  if (!(isalpha(context->object_name[0]) || context->object_name[0] == '_'))
  {
    printf("variable name must start with letter or underscore.\n");
    return false;
  }

  switch (context->datatype)
  {
    case TSL_TOKEN_INT:
      return _parser_instructions_create_var_int(context);
      break;

    case TSL_TOKEN_FLOAT:
      return _parser_instructions_create_var_float(context);
      break;

    case TSL_TOKEN_BOOL:
      return _parser_instructions_create_var_bool(context);
      break;

    case TSL_TOKEN_STRING:
      return _parser_instructions_create_var_string(context);
      break;
  }

  return true;
}

bool
tsl_parser_instructions_create_function(
  struct parse_context_t* const context)
{
  if (strlen(context->object_name) == 0)
  {
    printf("empty function name.\n");
    return false;
  }

  if (!(isalpha(context->object_name[0]) || context->object_name[0] == '_'))
  {
    printf("variable name must start with letter or underscore.\n");
    return false;
  }

  struct instruction_t create_function;
  enum variable_type_e return_type = 0;
  switch (context->datatype)
  {
    case TSL_TOKEN_INT:
      return_type = VAR_TYPE_INT;
      break;

    case TSL_TOKEN_FLOAT:
      return_type = VAR_TYPE_FLOAT;
      break;

    case TSL_TOKEN_BOOL:
      return_type = VAR_TYPE_BOOL;
      break;

    case TSL_TOKEN_STRING:
      return_type = VAR_TYPE_STRING;
      break;

    default:
      printf("unknown function return type.\n");
      return false;
  }
  inst_create_function(&create_function, context->object_name, return_type);
  tsl_global_scope_add_instruction(context->global_scope, &create_function);

  return true;
}
