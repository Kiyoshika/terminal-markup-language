#include "instructions_create_return_value.h"
#include "instructions.h"

void
inst_create_return_value(
  struct instruction_t* dest_instruction,
  const char* const reference_function_name,
  const char* const return_value)
{
  dest_instruction->instruction_type = INST_TYPE_CREATE_RETURN_VALUE;

  struct instruction_create_return_value_t create_return_value = {
    .reference_function_name = strdup(reference_function_name), // dangerously assuming no allocation error
    .value = strdup(return_value) // dangerously assuming no allocation error
  };

  dest_instruction->instruction.create_return_value = create_return_value;
}
