#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("bool x = true; bool x = false;");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Expected parsing to fail");
    return -1;
  }

  return 0;
}