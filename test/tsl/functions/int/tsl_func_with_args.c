#include "tsl.h"
#include "parser.h"
#include "variable.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
  struct tsl_global_scope_t* global_scope 
    = tsl_parser_parse("int _myFunc123_(int _my_Var1_, float _my_Var2_, bool _my_Var3_, string _my_Var4_) { return 0; }");

  if (!global_scope)
  {
    fprintf(stderr, "Failed to parse.");
    return -1;
  }

  struct function_t* function = &global_scope->function_list->functions[0];
  
  if (strcmp(function->name, "_myFunc123_") != 0)
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

  if (function->variable_list->n_variables != 4)
  {
    fprintf(stderr, "Expected 4 variables.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  struct variable_t* _my_Var1_ = &function->variable_list->variables[0];
  if (strcmp(_my_Var1_->name, "_my_Var1_") != 0)
  {
    fprintf(stderr, "Variable '_my_Var1_' has wrong name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (_my_Var1_->type != VAR_TYPE_INT)
  {
    fprintf(stderr, "Variable '_my_Var1_' has wrong datatype.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (_my_Var1_->value.as_int != 0)
  {
    fprintf(stderr, "Variable '_my_Var1_' has wrong value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  struct variable_t* _my_Var2_ = &function->variable_list->variables[1];
  if (strcmp(_my_Var2_->name, "_my_Var2_") != 0)
  {
    fprintf(stderr, "Variable '_my_Var2_' has wrong name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (_my_Var2_->type != VAR_TYPE_FLOAT)
  {
    fprintf(stderr, "Variable '_my_Var2_' has wrong data type.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (fabsf(_my_Var2_->value.as_float - 0.0f) > 0.000001f)
  {
    fprintf(stderr, "Variable '_my_Var2_' has wrong value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  struct variable_t* _my_Var3_ = &function->variable_list->variables[2];
  if (strcmp(_my_Var3_->name, "_my_Var3_") != 0)
  {
    fprintf(stderr, "Variable '_my_Var3_' has wrong name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (_my_Var3_->type != VAR_TYPE_BOOL)
  {
    fprintf(stderr, "Variable '_my_Var3_' has wrong data type.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (_my_Var3_->value.as_bool != false)
  {
    fprintf(stderr, "Variable '_my_Var3_' has wrong value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  struct variable_t* _my_Var4_ = &function->variable_list->variables[3];
  if (strcmp(_my_Var4_->name, "_my_Var4_") != 0)
  {
    fprintf(stderr, "Variable '_my_Var4_' has wrong name.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (_my_Var4_->type != VAR_TYPE_STRING)
  {
    fprintf(stderr, "Variable '_my_Var4_' has wrong data type.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  if (strcmp(_my_Var4_->value.as_string, "") != 0)
  {
    fprintf(stderr, "Variable '_my_Var4_' has wrong value.");
    tsl_global_scope_free(&global_scope);
    return -1;
  }

  return 0;
}
