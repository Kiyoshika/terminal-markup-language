#ifndef INSTRUCTION_ADD_FUNCTION_ARG_H
#define INSTRUCTION_ADD_FUNCTION_ARG_H

#include "variable.h"
#include "parser.h" // TSL_MAX_TOKEN_LEN

// forward declaration
struct instruction_t;
struct function_t;

// same as create_var_t but during the TSL_STATE_ADD_FUNCTION_ARG state
struct instruction_add_function_arg_t
{
  char reference_function_name[TSL_MAX_TOKEN_LEN];
  char* argument_name;
  enum variable_type_e argument_type;
};

void
inst_add_function_arg(
  struct instruction_t* dest_instruction,
  const char* const reference_function_name,
  const char* const argument_name,
  const enum variable_type_e argument_type);

#endif
