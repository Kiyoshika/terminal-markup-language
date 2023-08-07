#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("bool someVar123 = true; bool y = someVar123;");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.\n");
    return -1;
  }

  struct variable_t* variable = &global_scope->variable_list->variables[1];

  if (strcmp(variable->name, "y") != 0)
  {
    fprintf(stderr, "Incorrect variable name.\n");
    return -1;
  }

  if (variable->value.as_bool != true) // should be copied from someVar123
  {
    fprintf(stderr, "Incorrect variable value.\n");
    return -1;
  }

  tsl_global_scope_free(&global_scope);
  
  return 0;
}
