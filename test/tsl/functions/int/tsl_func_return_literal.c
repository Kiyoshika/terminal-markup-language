#include "tsl.h"
#include "parser.h"
#include "variable.h"
#include "function.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("int myFunc123() { return 12; }");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.");
    return -1;
  }

  struct function_t* function = &global_scope->function_list->functions[0];
  
  if (function->return_type != VAR_TYPE_INT)
  {
    fprintf(stderr, "Incorrect return type.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (function->return_value.as_int != 12)
  {
    fprintf(stderr, "Incorrect return value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }
  
  tsl_global_scope_free(&global_scope);

  global_scope = tsl_parser_parse("int myFunc123() { return -12; }");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.");
    return -1;
  }

  function = &global_scope->function_list->functions[0];
  
  if (function->return_type != VAR_TYPE_INT)
  {
    fprintf(stderr, "Incorrect return type.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (function->return_value.as_int != -12)
  {
    fprintf(stderr, "Incorrect return value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }
  
  return 0;
}
