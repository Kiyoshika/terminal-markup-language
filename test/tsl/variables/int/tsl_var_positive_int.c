#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("int someVar123 = 12;");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.\n");
    return -1;
  }

  struct instruction_create_var_t* create_var
    = &global_scope->instruction_list[0].instructions->instruction.create_var;

  char* name = create_var->variable->name;
  if (strcmp(name, "someVar123") != 0)
  {
    fprintf(stderr, "Incorrect variable name.\n");
    return -1;
  }

  int32_t value = *(int32_t*)create_var->variable->value;
  if (value != 12)
  {
    fprintf(stderr, "Incorrect variable value.\n");
    return -1;
  }

  tsl_global_scope_free(&global_scope);
  
  return 0;
}
