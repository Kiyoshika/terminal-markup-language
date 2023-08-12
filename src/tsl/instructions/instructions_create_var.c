#include "instructions.h"
#include "instructions_create_var.h"
#include "function.h"

void
inst_create_var(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const enum variable_type_e variable_type)
{
  dest_instruction->instruction_type = INST_TYPE_CREATE_VAR;

  struct instruction_create_var_t create_var = {
    .reference_function = NULL,
    .variable_name = strdup(variable_name), // dangerously assuming no allocation error
    .variable_type = variable_type 
  };

  if (reference_function)
    create_var.reference_function = *reference_function;

  dest_instruction->instruction.create_var = create_var;
}
