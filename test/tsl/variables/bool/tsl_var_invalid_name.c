#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int main()
{
  // variable names cannot start with numbers
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("bool 23dd = true;");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Expected parsing to fail");
    return -1;
  }

  return 0;
}
