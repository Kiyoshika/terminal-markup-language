#include "instructions.h"
#include "function.h"
#include "instructions_create_function.h"

void
inst_create_function(
  struct instruction_t* dest_instruction,
  const char* const function_name,
  const enum variable_type_e return_type)
{
  dest_instruction->instruction_type = INST_TYPE_CREATE_FUNCTION;

  struct instruction_create_function_t create_function = {
    .function_name = strdup(function_name), // dangerously assuming no allocation error
    .return_type = return_type
  };

  dest_instruction->instruction.create_function = create_function;
}
