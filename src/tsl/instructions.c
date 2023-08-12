#include "instructions.h"
#include "function.h"

struct instruction_list_t*
inst_list_create()
{
  struct instruction_list_t* instruction_list
    = malloc(sizeof(*instruction_list));

  if (!instruction_list)
    return NULL;

  instruction_list->n_instructions = 0;
  instruction_list->capacity = 10;
  instruction_list->instructions 
    = calloc(
        instruction_list->capacity, 
        sizeof(*instruction_list->instructions));
  if (!instruction_list->instructions)
  {
    free(instruction_list);
    return NULL;
  }

  return instruction_list;
}

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

bool
inst_list_add_instruction(
  struct instruction_list_t* instruction_list,
  const struct instruction_t* const instruction)
{
  memcpy(
    &instruction_list->instructions[instruction_list->n_instructions++],
    instruction,
    sizeof(*instruction));
  
  if (instruction_list->n_instructions == instruction_list->capacity)
  {
    size_t new_capacity = instruction_list->capacity * 2;
    void* alloc = realloc(instruction_list->instructions, new_capacity * sizeof(*instruction_list->instructions));
    if (!alloc)
      return false;
    instruction_list->instructions = alloc;
    instruction_list->capacity = new_capacity;
  }

  return true;
}
