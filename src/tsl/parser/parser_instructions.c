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

  // TODO: finish me, need to add an instruction in instructions.c for creating a new function
  // which adds a new object to the global scope. Note that we will also need to start updating
  // reference functions when creating variables after this functionality. Currently we're passing
  // a bunch of NULL parameters manually to those functions.
}
