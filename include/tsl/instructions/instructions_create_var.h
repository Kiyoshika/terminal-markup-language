#ifndef INSTRUCTIONS_CREATE_VAR_H
#define INSTRUCTIONS_CREATE_VAR_H

#include "variable.h"

// forward declaration
struct instruction_t;
struct function_t;

// e.g., int myVar;
struct instruction_create_var_t
{
  struct function_t* reference_function;

  char* variable_name;
  enum variable_type_e variable_type;
};

void
inst_create_var(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const enum variable_type_e variable_type);

#endif
