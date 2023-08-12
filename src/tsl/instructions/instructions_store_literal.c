#include "instructions.h"
#include "function.h"
#include "instructions_store_literal.h"

void
inst_store_literal_int(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const int32_t variable_value)
{
  dest_instruction->instruction_type = INST_TYPE_STORE_LITERAL;

  struct instruction_store_literal_t store_literal = {
    .reference_function = NULL,
    .variable_name = strdup(variable_name), // dangerously assumining no allocation error
    .variable_type = VAR_TYPE_INT,
    .variable_value.as_int = variable_value
  };

  if (reference_function)
    store_literal.reference_function = *reference_function;

  dest_instruction->instruction.store_literal = store_literal;
}

void
inst_store_literal_float(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const float variable_value)
{
  dest_instruction->instruction_type = INST_TYPE_STORE_LITERAL;

  struct instruction_store_literal_t store_literal = {
    .reference_function = NULL,
    .variable_name = strdup(variable_name), // dangerously assumining no allocation error
    .variable_type = VAR_TYPE_FLOAT,
    .variable_value.as_float = variable_value
  };

  if (reference_function)
    store_literal.reference_function = *reference_function;

  dest_instruction->instruction.store_literal = store_literal;
}

void
inst_store_literal_bool(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const bool variable_value)
{
  dest_instruction->instruction_type = INST_TYPE_STORE_LITERAL;

  struct instruction_store_literal_t store_literal = {
    .reference_function = NULL,
    .variable_name = strdup(variable_name), // dangerously assumining no allocation error
    .variable_type = VAR_TYPE_BOOL,
    .variable_value.as_bool = variable_value
  };

  if (reference_function)
    store_literal.reference_function = *reference_function;

  dest_instruction->instruction.store_literal = store_literal;
}

void
inst_store_literal_string(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const char* const variable_value)
{
  dest_instruction->instruction_type = INST_TYPE_STORE_LITERAL;

  struct instruction_store_literal_t store_literal = {
    .reference_function = NULL,
    .variable_name = strdup(variable_name), // dangerously assumining no allocation error
    .variable_type = VAR_TYPE_STRING,
    .variable_value.as_string = strdup(variable_value) // dangerously assumining no allocation error
  };

  if (reference_function)
    store_literal.reference_function = *reference_function;

  dest_instruction->instruction.store_literal = store_literal;
}
