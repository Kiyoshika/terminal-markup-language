#include "tsl.h"
#include "parser.h"
#include "variable.h"
#include "function.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("int myFunc123() {}");

  if (global_scope)
  {
    fprintf(stderr, "Expected parser to fail.");
    return -1;
  }

  return 0;
}
