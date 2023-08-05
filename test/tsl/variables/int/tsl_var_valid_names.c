#include "tsl.h"
#include "parser.h"
#include "instructions.h"
#include <stdio.h>
#include <string.h>

int main()
{
  int status = -1;

  struct tsl_global_scope_t* var1 = tsl_parser_parse("int var = 1;");
  struct tsl_global_scope_t* var2 = NULL;
  struct tsl_global_scope_t* var3 = NULL;
  struct tsl_global_scope_t* var4 = NULL;

  if (!var1)
  {
    fprintf(stderr, "'var' should be a valid name.\n");
    goto err;
  }

  var2 = tsl_parser_parse("int var123 = 1;");
  if (!var2)
  {
    fprintf(stderr, "'var123' should be a valid name.\n");
    goto err;
  }

  var3 = tsl_parser_parse("int _var = 1;");
  if (!var3)
  {
    fprintf(stderr, "'_var' should be a valid name.\n");
    goto err;
  }

  var4 = tsl_parser_parse("int my_var = 1;");
  if (!var4)
  {
    fprintf(stderr, "'my_var' should be a valid name.\n");
    goto err;
  }

  status = 0;
err:
  tsl_global_scope_free(&var1);
  tsl_global_scope_free(&var2);
  tsl_global_scope_free(&var3);
  tsl_global_scope_free(&var4);

  return status;
}
