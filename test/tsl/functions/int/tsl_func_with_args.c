#include "tsl.h"
#include "parser.h"
#include "variable.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
  struct tsl_global_scope_t* global_scope = tsl_parser_parse("int myFunc123(int x, float y) {}");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.");
    return -1;
  }

  struct function_t* function = &global_scope->function_list->functions[0];
  
  if (strcmp(function->name, "myFunc123") != 0)
  {
    fprintf(stderr, "Incorrect function name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (function->return_type != VAR_TYPE_INT)
  {
    fprintf(stderr, "Incorrect return type.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (function->variable_list->n_variables != 2)
  {
    fprintf(stderr, "Expected 2 variables.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  struct variable_t* x = &function->variable_list->variables[0];
  if (strcmp(x->name, "x") != 0)
  {
    fprintf(stderr, "Variable 'x' has wrong name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (x->type != VAR_TYPE_INT)
  {
    fprintf(stderr, "Variable 'x' has wrong datatype.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (x->value.as_int != 0)
  {
    fprintf(stderr, "Variable 'x' has wrong value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  struct variable_t* y = &function->variable_list->variables[1];
  if (strcmp(y->name, "y") != 0)
  {
    fprintf(stderr, "Variable 'y' has wrong name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (y->type != VAR_TYPE_FLOAT)
  {
    fprintf(stderr, "Variable 'y' has wrong datatype.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (fabsf(y->value.as_float - 0.0f) > 0.000001f)
  {
    fprintf(stderr, "Variable 'y' has wrong value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }


  return 0;
}
