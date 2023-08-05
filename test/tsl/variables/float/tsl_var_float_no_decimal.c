#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int main()
{
  // floats cannot be assigned an integer literal
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("float myVariable123 = 1;");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Expected parsing to fail");
    return -1;
  }

  return 0;
}
