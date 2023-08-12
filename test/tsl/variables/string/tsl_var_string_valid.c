#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

int main()
{
  // string with text and a bunch of other tokens that need to be handled properly
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("string someVar123 = \"hello, there!@#$%^&*()-=_+{}[];.?<>/\";");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.\n");
    return -1;
  }

  struct variable_t* variable = &global_scope->variable_list->variables[0];

  if (strcmp(variable->name, "someVar123") != 0)
  {
    fprintf(stderr, "Incorrect variable name.\n");
    return -1;
  }

  if (strcmp(variable->value.as_string, "hello, there!@#$%^&*()-=_+{}[];.?<>/") != 0)
  {
    fprintf(stderr, "Incorrect variable value.\n");
    return -1;
  }

  tsl_global_scope_free(&global_scope);
  
  return 0;
}
