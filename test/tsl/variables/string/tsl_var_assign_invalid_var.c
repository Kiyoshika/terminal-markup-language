#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("string someVar123 = nonExistant;");

  if (global_scope)
  {
    tsl_global_scope_free(&global_scope);
    fprintf(stderr, "Failed to parse.\n");
    return -1;
  }

  return 0;
}
