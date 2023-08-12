#include "instructions.h"
#include "function.h"
#include "instructions_add_function_arg.h"

void
inst_add_function_arg(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const argument_name,
  const enum variable_type_e argument_type)
{
  // reference_function in this case is mandatory and will be checked
  // prior to this function being called

  dest_instruction->instruction_type = INST_TYPE_ADD_FUNCTION_ARG;

  struct instruction_add_function_arg_t add_function_arg = {
    .reference_function = *reference_function,
    .argument_name = strdup(argument_name), // dangerously assumining no allocation errors
    .argument_type = argument_type
  };

  dest_instruction->instruction.add_function_arg = add_function_arg;
}
