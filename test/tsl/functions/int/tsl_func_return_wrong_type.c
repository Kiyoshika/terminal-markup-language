#include "tsl.h"
#include "parser.h"
#include "variable.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("int myFunc123() { return true; }");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Expected parser to fail.");
    return -1;
  }

  return 0;
}
