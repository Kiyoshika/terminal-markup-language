#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("int someVar123 = -12; float y = someVar123;");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Expected to fail parsing.");
    return -1;
  }

  return 0;
}
