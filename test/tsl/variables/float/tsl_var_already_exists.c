#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("float x = 1.0; float x = 2.0;");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Expected parsing to fail");
    return -1;
  }

  return 0;
}
