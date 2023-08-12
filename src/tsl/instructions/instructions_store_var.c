#include "instructions.h"
#include "function.h"
#include "instructions_store_var.h"

void
inst_store_var(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const dest_variable_name,
  const char* const src_variable_name)
{
  dest_instruction->instruction_type = INST_TYPE_STORE_VAR;

  struct instruction_store_var_t store_var = {
    .reference_function = NULL,
    .dest_variable_name = strdup(dest_variable_name), // dangerously assumining no allocation error
    .src_variable_name = strdup(src_variable_name) // dangerously assumining no allocation error
  };

  if (reference_function)
    store_var.reference_function = *reference_function;

  dest_instruction->instruction.store_var = store_var;
}
