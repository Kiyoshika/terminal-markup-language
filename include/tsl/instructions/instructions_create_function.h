#ifndef INSTRUCTION_CREATE_FUNCTION_H
#define INSTRUCTION_CREATE_FUNCTION_H

#include "variable.h"

// forward declaration
struct instruction_t;

// e.g., int myfunc()
struct instruction_create_function_t
{
  char* function_name;
  enum variable_type_e return_type;
};

void
inst_create_function(
  struct instruction_t* dest_instruction,
  const char* const function_name,
  const enum variable_type_e return_type);

#endif
